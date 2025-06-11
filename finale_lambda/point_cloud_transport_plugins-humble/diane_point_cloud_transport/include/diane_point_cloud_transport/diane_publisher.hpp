// include/diane_point_cloud_transport/diane_publisher.hpp

#pragma once

#include <memory>
#include <string>
#include <vector>

#include <torch/torch.h>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <point_cloud_transport/simple_publisher_plugin.hpp>
#include <point_cloud_interfaces/msg/compressed_point_cloud2.hpp>
#include <point_cloud_interfaces/msg/compressed_point_cloud_wrapper2.hpp>

namespace diane_point_cloud_transport
{

class DianePublisher
  : public point_cloud_transport::SimplePublisherPlugin<
      point_cloud_interfaces::msg::CompressedPointCloudWrapper2
    >
{
public:
  DianePublisher();

  std::string getTransportName() const override
  {
    return "diane";
  }
  

  void declareParameters(const std::string & base_topic) override;

  std::string getDataType() const override
  {
    return "point_cloud_interfaces/msg/CompressedPointCloudWrapper2";
  }

  // qui avviene la compressione
  TypedEncodeResult encodeTyped(
    const sensor_msgs::msg::PointCloud2 & raw_msg) const override;
protected:
  void advertiseImpl(
      rclcpp::Node::SharedPtr node,
      const std::string & base_topic,
      rmw_qos_profile_t custom_qos,
      const rclcpp::PublisherOptions & options) override;
private:
  float bandwidth_;
  float fps_;
  float lambda_;
    // callback su cambio parametri
  rcl_interfaces::msg::SetParametersResult
  onParams(const std::vector<rclcpp::Parameter>&);
};

}  // namespace diane_point_cloud_transport
