// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from point_cloud_interfaces:msg/CompressedPointCloud2.idl
// generated code does not contain a copyright notice

#ifndef POINT_CLOUD_INTERFACES__MSG__DETAIL__COMPRESSED_POINT_CLOUD2__BUILDER_HPP_
#define POINT_CLOUD_INTERFACES__MSG__DETAIL__COMPRESSED_POINT_CLOUD2__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "point_cloud_interfaces/msg/detail/compressed_point_cloud2__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace point_cloud_interfaces
{

namespace msg
{

namespace builder
{

class Init_CompressedPointCloud2_format
{
public:
  explicit Init_CompressedPointCloud2_format(::point_cloud_interfaces::msg::CompressedPointCloud2 & msg)
  : msg_(msg)
  {}
  ::point_cloud_interfaces::msg::CompressedPointCloud2 format(::point_cloud_interfaces::msg::CompressedPointCloud2::_format_type arg)
  {
    msg_.format = std::move(arg);
    return std::move(msg_);
  }

private:
  ::point_cloud_interfaces::msg::CompressedPointCloud2 msg_;
};

class Init_CompressedPointCloud2_is_dense
{
public:
  explicit Init_CompressedPointCloud2_is_dense(::point_cloud_interfaces::msg::CompressedPointCloud2 & msg)
  : msg_(msg)
  {}
  Init_CompressedPointCloud2_format is_dense(::point_cloud_interfaces::msg::CompressedPointCloud2::_is_dense_type arg)
  {
    msg_.is_dense = std::move(arg);
    return Init_CompressedPointCloud2_format(msg_);
  }

private:
  ::point_cloud_interfaces::msg::CompressedPointCloud2 msg_;
};

class Init_CompressedPointCloud2_compressed_data
{
public:
  explicit Init_CompressedPointCloud2_compressed_data(::point_cloud_interfaces::msg::CompressedPointCloud2 & msg)
  : msg_(msg)
  {}
  Init_CompressedPointCloud2_is_dense compressed_data(::point_cloud_interfaces::msg::CompressedPointCloud2::_compressed_data_type arg)
  {
    msg_.compressed_data = std::move(arg);
    return Init_CompressedPointCloud2_is_dense(msg_);
  }

private:
  ::point_cloud_interfaces::msg::CompressedPointCloud2 msg_;
};

class Init_CompressedPointCloud2_row_step
{
public:
  explicit Init_CompressedPointCloud2_row_step(::point_cloud_interfaces::msg::CompressedPointCloud2 & msg)
  : msg_(msg)
  {}
  Init_CompressedPointCloud2_compressed_data row_step(::point_cloud_interfaces::msg::CompressedPointCloud2::_row_step_type arg)
  {
    msg_.row_step = std::move(arg);
    return Init_CompressedPointCloud2_compressed_data(msg_);
  }

private:
  ::point_cloud_interfaces::msg::CompressedPointCloud2 msg_;
};

class Init_CompressedPointCloud2_point_step
{
public:
  explicit Init_CompressedPointCloud2_point_step(::point_cloud_interfaces::msg::CompressedPointCloud2 & msg)
  : msg_(msg)
  {}
  Init_CompressedPointCloud2_row_step point_step(::point_cloud_interfaces::msg::CompressedPointCloud2::_point_step_type arg)
  {
    msg_.point_step = std::move(arg);
    return Init_CompressedPointCloud2_row_step(msg_);
  }

private:
  ::point_cloud_interfaces::msg::CompressedPointCloud2 msg_;
};

class Init_CompressedPointCloud2_is_bigendian
{
public:
  explicit Init_CompressedPointCloud2_is_bigendian(::point_cloud_interfaces::msg::CompressedPointCloud2 & msg)
  : msg_(msg)
  {}
  Init_CompressedPointCloud2_point_step is_bigendian(::point_cloud_interfaces::msg::CompressedPointCloud2::_is_bigendian_type arg)
  {
    msg_.is_bigendian = std::move(arg);
    return Init_CompressedPointCloud2_point_step(msg_);
  }

private:
  ::point_cloud_interfaces::msg::CompressedPointCloud2 msg_;
};

class Init_CompressedPointCloud2_fields
{
public:
  explicit Init_CompressedPointCloud2_fields(::point_cloud_interfaces::msg::CompressedPointCloud2 & msg)
  : msg_(msg)
  {}
  Init_CompressedPointCloud2_is_bigendian fields(::point_cloud_interfaces::msg::CompressedPointCloud2::_fields_type arg)
  {
    msg_.fields = std::move(arg);
    return Init_CompressedPointCloud2_is_bigendian(msg_);
  }

private:
  ::point_cloud_interfaces::msg::CompressedPointCloud2 msg_;
};

class Init_CompressedPointCloud2_width
{
public:
  explicit Init_CompressedPointCloud2_width(::point_cloud_interfaces::msg::CompressedPointCloud2 & msg)
  : msg_(msg)
  {}
  Init_CompressedPointCloud2_fields width(::point_cloud_interfaces::msg::CompressedPointCloud2::_width_type arg)
  {
    msg_.width = std::move(arg);
    return Init_CompressedPointCloud2_fields(msg_);
  }

private:
  ::point_cloud_interfaces::msg::CompressedPointCloud2 msg_;
};

class Init_CompressedPointCloud2_height
{
public:
  explicit Init_CompressedPointCloud2_height(::point_cloud_interfaces::msg::CompressedPointCloud2 & msg)
  : msg_(msg)
  {}
  Init_CompressedPointCloud2_width height(::point_cloud_interfaces::msg::CompressedPointCloud2::_height_type arg)
  {
    msg_.height = std::move(arg);
    return Init_CompressedPointCloud2_width(msg_);
  }

private:
  ::point_cloud_interfaces::msg::CompressedPointCloud2 msg_;
};

class Init_CompressedPointCloud2_header
{
public:
  Init_CompressedPointCloud2_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CompressedPointCloud2_height header(::point_cloud_interfaces::msg::CompressedPointCloud2::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_CompressedPointCloud2_height(msg_);
  }

private:
  ::point_cloud_interfaces::msg::CompressedPointCloud2 msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::point_cloud_interfaces::msg::CompressedPointCloud2>()
{
  return point_cloud_interfaces::msg::builder::Init_CompressedPointCloud2_header();
}

}  // namespace point_cloud_interfaces

#endif  // POINT_CLOUD_INTERFACES__MSG__DETAIL__COMPRESSED_POINT_CLOUD2__BUILDER_HPP_
