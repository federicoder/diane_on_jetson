// src/diane_subscriber.cpp

#include <diane_point_cloud_transport/diane_encoder.hpp>
#include <diane_point_cloud_transport/diane_subscriber.hpp>

#include <rcl_interfaces/msg/parameter_descriptor.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>  // giusto include
#include <sensor_msgs/msg/point_field.hpp>
#include <rclcpp/rclcpp.hpp>
#include <chrono>

#include <fstream>
#include <iomanip>  // std::setprecision
#include <filesystem> // C++17
#include <sstream>
namespace fs = std::filesystem;
namespace diane_point_cloud_transport
{

void DianeSubscriber::declareParameters()
{
declareParam<double>("bandwidth", 8000000000.0);

  //getParam<double>("bandwidth", config_.bandwidth);
  declareParam<double>("fps", 120.0);
  //getParam<double>("fps", config_.fps);

  auto cb = [this](std::vector<rclcpp::Parameter> ps) -> rcl_interfaces::msg::SetParametersResult {
    rcl_interfaces::msg::SetParametersResult r; r.successful = true;
    for (auto & p : ps) {
      if (p.get_name() == "bandwidth") config_.bandwidth = p.as_double();
      else if (p.get_name() == "fps")    config_.fps       = p.as_double();
    }
    return r;
  };
  setParamCallback(cb);
}

std::string DianeSubscriber::getTransportName() const
{
  return "diane";
}



#include <fstream>
#include <iomanip>
#include <filesystem>
#include <sstream>

namespace fs = std::filesystem;

void save_msg_to_csv(const point_cloud_interfaces::msg::CompressedPointCloudWrapper2 & msg, const std::string& path, const double latency) {
    // Crea nome file unico per coppia (bw, fps)
    std::stringstream filename;
    filename << "log_bw" << static_cast<uint64_t>(msg.bw)
             << "_fps" << static_cast<uint32_t>(msg.fps) << ".csv";
    std::string file_path = (fs::path(path) / filename.str()).string();

    // Verifica se il file esiste già (per header)
    bool file_exists = fs::exists(file_path);

    // Apri in append
    std::ofstream file(file_path, std::ios::app);
    if (!file.is_open()) return;

    // Scrivi header se file appena creato
    if (!file_exists) {
        file << "timestamp,latency,encoding_time_diane,numberpoints,height,width,bw,fps,is_dense,format\n";
    }
    file << std::fixed << std::setprecision(6)
         << msg.header.stamp.sec + msg.header.stamp.nanosec*1e-9 << ","
         << latency << ","
         << msg.encoding_time_diane << ","
         << msg.numberpoints << ","
         << msg.height << ","
         << msg.width << ","
         << msg.bw << ","
         << msg.fps << ","
         << (msg.is_dense ? "true" : "false") << ","
         << msg.format << "\n";
    file.close();
}




DianeSubscriber::DecodeResult DianeSubscriber::decodeTyped(
    const point_cloud_interfaces::msg::CompressedPointCloudWrapper2 & compressed) const
{
  auto logger = rclcpp::get_logger("diane_subscriber");
  auto t_pre_decode = std::chrono::high_resolution_clock::now();
    const auto & data = compressed.compressed_data;
    size_t n_pts = data.size() / 8; // 8 byte per punto compresso

    auto out = std::make_shared<sensor_msgs::msg::PointCloud2>();
    out->header = compressed.header;

    out->height = 1;
    out->width = static_cast<uint32_t>(n_pts);
    out->is_dense = false;
    out->is_bigendian = false;
    out->point_step = 16;  // XYZ (3 float32) + RGB (float32)
    out->row_step = out->point_step * out->width;
    //compressed.timestamp = compressed.timestamp - t_pre_decode
    uint64_t t_now_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
    t_pre_decode.time_since_epoch()
).count();

double latency_ms = (t_now_ns - compressed.timestamp) / 1e6;

RCLCPP_DEBUG(logger, "[decodeTyped] Latency (ms): %.2f", latency_ms);

    save_msg_to_csv(compressed, "/root/ros2_ws/data_csv", latency_ms); // Scegli il path che vuoi!
    sensor_msgs::msg::PointField f;
    out->fields.clear();

    f = sensor_msgs::msg::PointField();
    f.name = "x"; f.offset = 0; f.datatype = sensor_msgs::msg::PointField::FLOAT32; f.count = 1;
    out->fields.push_back(f);
    f.name = "y"; f.offset = 4; out->fields.push_back(f);
    f.name = "z"; f.offset = 8; out->fields.push_back(f);
    f.name = "rgb"; f.offset = 12; out->fields.push_back(f);

    out->data.resize(n_pts * out->point_step);

    // Conversione rapida usando puntatori vettorializzati
    const int16_t* ptr_int16 = reinterpret_cast<const int16_t*>(data.data());

    float multiplier_inv = 0.1f; // inverso del multiplier (10.0f)

    #pragma omp parallel for
    for (size_t i = 0; i < n_pts; ++i)
    {
        float xf = static_cast<float>(ptr_int16[i * 4 + 0]) * multiplier_inv;
        float yf = static_cast<float>(ptr_int16[i * 4 + 1]) * multiplier_inv;
        float zf = static_cast<float>(ptr_int16[i * 4 + 2]) * multiplier_inv;

        uint16_t rgb664 = static_cast<uint16_t>(ptr_int16[i * 4 + 3]);
        uint8_t R = ((rgb664 >> 10) & 0x3F) << 2;
        uint8_t G = ((rgb664 >> 4) & 0x3F) << 2;
        uint8_t B = (rgb664 & 0x0F) << 4;
    uint32_t rgb32 = (R << 16) | (G << 8) | B;
    union { uint32_t u; float f; } rgb_conv;
    rgb_conv.u = rgb32;

    // Cast output to float* directly for faster access (buffer is 16 bytes per point)
    float* out_floats = reinterpret_cast<float*>(&out->data[i * out->point_step]);
    out_floats[0] = xf;
    out_floats[1] = yf;
    out_floats[2] = zf;
    out_floats[3] = rgb_conv.f;
        /*uint32_t rgb32 = (R << 16) | (G << 8) | B;
        float rgb_f;
        std::memcpy(&rgb_f, &rgb32, sizeof(rgb_f));

        uint8_t* out_ptr = &out->data[i * out->point_step];
       memcpy(out_ptr, &xf, 4);
        memcpy(out_ptr + 4, &yf, 4);
        memcpy(out_ptr + 8, &zf, 4);
        memcpy(out_ptr + 12, &rgb_f, 4); */
    }
auto t_post_decode = std::chrono::high_resolution_clock::now();
double decode_ms = std::chrono::duration<double,std::milli>(t_post_decode - t_pre_decode).count();
RCLCPP_DEBUG(logger, "[decodeTyped] Tempo decode: %.2f ms", decode_ms);
    return out;
}


}  // namespace diane_point_cloud_transport

