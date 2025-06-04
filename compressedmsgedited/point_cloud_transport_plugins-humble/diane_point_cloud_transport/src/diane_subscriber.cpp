// src/diane_subscriber.cpp

#include <diane_point_cloud_transport/diane_encoder.hpp>
#include <diane_point_cloud_transport/diane_subscriber.hpp>

#include <rcl_interfaces/msg/parameter_descriptor.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>  // giusto include
#include <sensor_msgs/msg/point_field.hpp>
#include <rclcpp/rclcpp.hpp>
#include <chrono>
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
        float rgb_f;
        std::memcpy(&rgb_f, &rgb32, sizeof(rgb_f));

        uint8_t* out_ptr = &out->data[i * out->point_step];
       memcpy(out_ptr, &xf, 4);
        memcpy(out_ptr + 4, &yf, 4);
        memcpy(out_ptr + 8, &zf, 4);
        memcpy(out_ptr + 12, &rgb_f, 4);
    }
auto t_post_decode = std::chrono::high_resolution_clock::now();
double decode_ms = std::chrono::duration<double,std::milli>(t_post_decode - t_pre_decode).count();
RCLCPP_DEBUG(logger, "[decodeTyped] Tempo decode: %.2f ms", decode_ms);
    return out;
}


}  // namespace diane_point_cloud_transport

