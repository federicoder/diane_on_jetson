// include/diane_point_cloud_transport/diane_subscriber.hpp

#pragma once

#include <string>

#include <sensor_msgs/msg/point_cloud2.hpp>
#include <point_cloud_transport/simple_subscriber_plugin.hpp>
#include <point_cloud_interfaces/msg/compressed_point_cloud2.hpp>
#include <point_cloud_interfaces/msg/compressed_point_cloud_wrapper2.hpp>
#include <point_cloud_transport/transport_hints.hpp>

namespace diane_point_cloud_transport
{

class DianeSubscriber
    : public point_cloud_transport::SimpleSubscriberPlugin<
      point_cloud_interfaces::msg::CompressedPointCloudWrapper2>
{
public:
  // DianeSubscriber();

  std::string getTransportName() const override;

  
  void declareParameters() override;

  // void declareParameters(const std::string & base_topic) override;

  std::string getDataType() const override
  {
    return "point_cloud_interfaces/msg/CompressedPointCloudWrapper2";
  }
  
  DecodeResult decodeTyped(const point_cloud_interfaces::msg::CompressedPointCloudWrapper2 & compressed) const override;

  // std::string getDataType() const override
  // {
  //   return sensor_msgs::msg::PointCloud2().get_type_support()->data_type;
  // }

  // qui avviene la decompressione
  // void decodeTyped(
  //   const point_cloud_interfaces::msg::CompressedPointCloud2 & compressed,
  //   const DecodeCallback & callback) const override;
  struct DianeSubscriberConfig
  {
    double bandwidth = 5.0e5;
    double fps = 50.0;
  };

  DianeSubscriberConfig config_;
private:
  float bandwidth_;
  float fps_;
};

}  // namespace diane_point_cloud_transport
