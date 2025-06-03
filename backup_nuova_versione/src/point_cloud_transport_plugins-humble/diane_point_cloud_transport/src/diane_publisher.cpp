// src/diane_publisher.cpp
#include <diane_point_cloud_transport/diane_encoder.hpp>
#include <diane_point_cloud_transport/diane_publisher.hpp>

#include <rcl_interfaces/msg/parameter_descriptor.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include <functional>            // std::bind
#include <rclcpp/rclcpp.hpp>     // logger
namespace diane_point_cloud_transport
{

DianePublisher::DianePublisher()
: bandwidth_(8e9f), fps_(30.0f)
{}

void DianePublisher::declareParameters(const std::string & base_topic)
{
/*
  rcl_interfaces::msg::ParameterDescriptor bw_desc;
  bw_desc.name = "bandwidth";
  bw_desc.type = rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE;
  bw_desc.description = "Available bandwidth in bits per second";
  declareParam("bandwidth", bandwidth_, bw_desc);
  getParam("bandwidth", bandwidth_);

  rcl_interfaces::msg::ParameterDescriptor fps_desc;
  fps_desc.name = "fps";
  fps_desc.type = rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE;
  fps_desc.description = "Frame rate in frames per second";
  declareParam("fps", fps_, fps_desc);
  getParam("fps", fps_);
  */
   rcl_interfaces::msg::ParameterDescriptor d;
  d.type        = rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE;

  d.description = "Available bandwidth (bit/s)";
  declareParam("bandwidth", bandwidth_, d);     // →  out.diane.bandwidth
  getParam   ("bandwidth", bandwidth_);

  d.description = "Frame-rate (fps)";
  declareParam("fps", fps_, d);                 // →  out.diane.fps
  getParam   ("fps", fps_);

  /* Parametri mutabili: registriamo la callback una sola volta */
  setParamCallback(std::bind(&DianePublisher::onParams, this, std::placeholders::_1));
}

/* ---------- callback dinamica ---------- */
rcl_interfaces::msg::SetParametersResult
DianePublisher::onParams(const std::vector<rclcpp::Parameter>& v)
{
  auto log = rclcpp::get_logger("diane_publisher");

  for (const auto& p : v)          // <-- serve il for!
  {
    const std::string& name = p.get_name();

    /* --- bandwidth --- */
    if (name.size() >= 9 &&         // lunghezza minima ".bandwidth"
        name.compare(name.size() - 9, 9, "bandwidth") == 0)
    {
      bandwidth_ = static_cast<float>(p.as_double());
      RCLCPP_INFO(log, "Bandwidth → %.0f", bandwidth_);
    }
    /* --- fps --- */
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
  point_cloud_interfaces::msg::CompressedPointCloud2
>::TypedEncodeResult
DianePublisher::encodeTyped(
  const sensor_msgs::msg::PointCloud2 & raw_msg) const
{
  // estrazione H,W,N come prima...
  auto options = torch::TensorOptions().dtype(torch::kFloat32).device(torch::kCUDA);
  auto cpu_tensor = torch::empty({static_cast<int64_t>(raw_msg.height),
                                  static_cast<int64_t>(raw_msg.width), 4},
                                 options);

  sensor_msgs::PointCloud2ConstIterator<float> itx(raw_msg, "x");
  sensor_msgs::PointCloud2ConstIterator<float> ity(raw_msg, "y");
  sensor_msgs::PointCloud2ConstIterator<float> itz(raw_msg, "z");
  sensor_msgs::PointCloud2ConstIterator<uint8_t> itr(raw_msg, "r");
  sensor_msgs::PointCloud2ConstIterator<uint8_t> itg(raw_msg, "g");
  sensor_msgs::PointCloud2ConstIterator<uint8_t> itb(raw_msg, "b");
float alpha = 1.0f; // default alpha

size_t idx = 0;
size_t total_size = raw_msg.height * raw_msg.width;
assert(cpu_tensor.numel() / 4 >= total_size); // Controllo preventivo
for (size_t i = 0; i < total_size; ++i, ++itx, ++ity, ++itz, ++itr, ++itg, ++itb) {
    size_t h = idx / raw_msg.width;
    size_t w = idx % raw_msg.width;
    assert(h < raw_msg.height && w < raw_msg.width); // Controllo preventivo

    cpu_tensor[h][w][0] = *itx;
    cpu_tensor[h][w][1] = *ity;
    cpu_tensor[h][w][2] = *itz;
    cpu_tensor[h][w][3] = color_to_float({
      (*itr)/255.0f, (*itg)/255.0f, (*itb)/255.0f, 1.0f
    });
    ++idx;
}


  auto compressed = encode_diane_multinomial_i16(cpu_tensor, bandwidth_, fps_, 15.0f);

  point_cloud_interfaces::msg::CompressedPointCloud2 out;
  out.header       = raw_msg.header;
  out.format       = "diane";
  out.height       = raw_msg.height;
  out.width        = raw_msg.width;
  out.compressed_data = std::move(compressed);

  return out;
}

}  // namespace diane_point_cloud_transport
