// --- file: src/diane_publisher.cpp ---

#include <diane_point_cloud_transport/diane_encoder.hpp>
#include <diane_point_cloud_transport/diane_publisher.hpp>

#include <rcl_interfaces/msg/parameter_descriptor.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include <functional>            // std::bind
#include <rclcpp/rclcpp.hpp>     // logger

// Per misurare i tempi
#include <chrono>
#include <rclcpp/qos.hpp>
namespace diane_point_cloud_transport
{

DianePublisher::DianePublisher()
: bandwidth_(8e9f), fps_(30.0f)
{}


// dentro la tua classe publisher, override della funzione
void DianePublisher::advertiseImpl(
    rclcpp::Node::SharedPtr node,
    const std::string & base_topic,
    rmw_qos_profile_t custom_qos,
    const rclcpp::PublisherOptions & options)
{
    // Forza sempre BEST_EFFORT
    custom_qos.reliability = RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT;
    custom_qos.durability  = RMW_QOS_POLICY_DURABILITY_VOLATILE;
    custom_qos.history     = RMW_QOS_POLICY_HISTORY_KEEP_LAST;
    custom_qos.depth     = 10;
    // CONVERTI custom_qos in rclcpp::QoS!
    rclcpp::QoS qos_profile(rclcpp::QoSInitialization::from_rmw(custom_qos));
    qos_profile.get_rmw_qos_profile() = custom_qos;
    SimplePublisherPlugin<point_cloud_interfaces::msg::CompressedPointCloudWrapper2>::advertiseImpl(
        node, base_topic, custom_qos, options
    );
/*
    this->publisher_ = node->create_publisher<point_cloud_interfaces::msg::CompressedPointCloudWrapper2>(
        base_topic, custom_qos, options);
        */
}

void DianePublisher::declareParameters(const std::string & base_topic)
{
  rcl_interfaces::msg::ParameterDescriptor d;
  d.type        = rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE;

  d.description = "Available bandwidth (bit/s)";
  declareParam("bandwidth", bandwidth_, d);
  getParam   ("bandwidth", bandwidth_);

  d.description = "Frame-rate (fps)";
  declareParam("fps", fps_, d);
  getParam   ("fps", fps_);

  setParamCallback(std::bind(&DianePublisher::onParams, this, std::placeholders::_1));
}

rcl_interfaces::msg::SetParametersResult
DianePublisher::onParams(const std::vector<rclcpp::Parameter>& v)
{
  auto log = rclcpp::get_logger("diane_publisher");
  for (const auto& p : v) {
    const std::string& name = p.get_name();
    if (name.size() >= 9 &&
        name.compare(name.size() - 9, 9, "bandwidth") == 0)
    {
      bandwidth_ = static_cast<float>(p.as_double());
      RCLCPP_INFO(log, "Bandwidth → %.0f", bandwidth_);
    }
    else if (name.size() >= 3 &&
             name.compare(name.size() - 3, 3, "fps") == 0)
    {
      fps_ = static_cast<float>(p.as_double());
      RCLCPP_INFO(log, "FPS → %.0f", fps_);
    }
  }
  rcl_interfaces::msg::SetParametersResult res;
  res.successful = true;
  return res;
}


point_cloud_transport::SimplePublisherPlugin<
  point_cloud_interfaces::msg::CompressedPointCloudWrapper2
>::TypedEncodeResult
DianePublisher::encodeTyped(
  const sensor_msgs::msg::PointCloud2 & raw_msg) const
{
  auto logger = rclcpp::get_logger("diane_publisher");
  // inizio misurazione
  auto t0 = std::chrono::high_resolution_clock::now();

  // 1) Recupero dimensioni H,W,N
  const int64_t H = static_cast<int64_t>(raw_msg.height);
  const int64_t W = static_cast<int64_t>(raw_msg.width);
  const int64_t N = H * W;
  RCLCPP_DEBUG(logger,
    "[encodeTyped] Ingresso: height=%ld  width=%ld  → N_punti=%ld",
    (long)H, (long)W, (long)N
  );

  //
  // === 2. Creo un tensore HOST (CPU‐pinned) di forma [N,4] ===
  //
  auto host_opts = torch::TensorOptions()
                     .dtype(torch::kFloat32)
                     .pinned_memory(true)   // dà un H→D asincrono più veloce
                     .device(torch::kCPU);

  torch::Tensor host_tensor = torch::empty({N, 4}, host_opts);
  float *host_ptr = host_tensor.data_ptr<float>();

  //
  // === 3. Popolo host_tensor in RAM con un loop “LINEARE” su CPU ===
  //
  sensor_msgs::PointCloud2ConstIterator<float> itx(raw_msg, "x");
  sensor_msgs::PointCloud2ConstIterator<float> ity(raw_msg, "y");
  sensor_msgs::PointCloud2ConstIterator<float> itz(raw_msg, "z");
  sensor_msgs::PointCloud2ConstIterator<uint8_t> itr(raw_msg, "r");
  sensor_msgs::PointCloud2ConstIterator<uint8_t> itg(raw_msg, "g");
  sensor_msgs::PointCloud2ConstIterator<uint8_t> itb(raw_msg, "b");
  const float alpha = 1.0f;  // componente alpha fissa

  for (int64_t i = 0; i < N; ++i, ++itx, ++ity, ++itz, ++itr, ++itg, ++itb) {
    host_ptr[i*4 + 0] = *itx;
    host_ptr[i*4 + 1] = *ity;
    host_ptr[i*4 + 2] = *itz;
    host_ptr[i*4 + 3] = color_to_float({
      (*itr)/255.0f, (*itg)/255.0f, (*itb)/255.0f, alpha
    });
  }

  //
  // === 4. Bulk‐upload HOST→GPU (unica chiamata non blocking) ===
  //
  //   .to(torch::kCUDA, true) esegue internamente cudaMemcpyAsync su memoria “pinned”
  //
  auto gpu_blob = host_tensor.to(
                    torch::kCUDA,
                    /*non_blocking=*/true,
                    /*copy=*/true
                  );

  //
  // === 5. Risagomiamo [N,4] → [H,W,4] in GPU ===
  //
  auto gpu_tensor = gpu_blob.view({H, W, 4}).contiguous();
  // gpu_tensor ora è [H,W,4] in torch::kCUDA, pronto per il compressore

  //
  // === 6. Chiamo l’encoder custom (tutto in GPU) ===
  //
  auto t_pre_encode = std::chrono::high_resolution_clock::now();
  std::vector<uint8_t> compressed = encode_diane_multinomial_i16(
                                       gpu_tensor,
                                       bandwidth_,
                                       fps_,
                                       15.0f   // dist_guarant
                                     );
auto t_post_encode = std::chrono::high_resolution_clock::now();
double encode_ms = std::chrono::duration<double,std::milli>(t_post_encode - t_pre_encode).count();
RCLCPP_DEBUG(logger, "[encodeTyped] Tempo encode_diane_multinomial_i16: %.2f ms", encode_ms);
  RCLCPP_DEBUG(logger, "[encodeTyped] bufSize=%zu byte", compressed.size());  RCLCPP_DEBUG(logger, "[encodeTyped] number_of_points_obtained=%zu points", compressed.size()/1024);

  //
  // === 7. Compongo il messaggio compress -> publish ===
  //
  point_cloud_interfaces::msg::CompressedPointCloudWrapper2 out;
  out.header         = raw_msg.header;
  out.format         = "diane";
  out.height         = raw_msg.height;
  out.width          = raw_msg.width;
  out.compressed_data = std::move(compressed);
  out.encoding_time_diane = encode_ms;
  out.timestamp = t0.time_since_epoch().count();
  out.numberpoints = compressed.size()/8 ;
  out.bw = bandwidth_;
  out.fps = fps_;
  
  // fine misurazione
  auto t1 = std::chrono::high_resolution_clock::now();
  double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
  RCLCPP_DEBUG(logger,
    "[encodeTyped] Tempo totale encodeTyped: %.2f ms", ms
  );

  return out;
}

}  // namespace diane_point_cloud_transport

