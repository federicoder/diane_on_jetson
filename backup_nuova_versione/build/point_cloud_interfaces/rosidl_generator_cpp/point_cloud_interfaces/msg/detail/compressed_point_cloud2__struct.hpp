// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from point_cloud_interfaces:msg/CompressedPointCloud2.idl
// generated code does not contain a copyright notice

#ifndef POINT_CLOUD_INTERFACES__MSG__DETAIL__COMPRESSED_POINT_CLOUD2__STRUCT_HPP_
#define POINT_CLOUD_INTERFACES__MSG__DETAIL__COMPRESSED_POINT_CLOUD2__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'fields'
#include "sensor_msgs/msg/detail/point_field__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__point_cloud_interfaces__msg__CompressedPointCloud2 __attribute__((deprecated))
#else
# define DEPRECATED__point_cloud_interfaces__msg__CompressedPointCloud2 __declspec(deprecated)
#endif

namespace point_cloud_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct CompressedPointCloud2_
{
  using Type = CompressedPointCloud2_<ContainerAllocator>;

  explicit CompressedPointCloud2_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->height = 0ul;
      this->width = 0ul;
      this->is_bigendian = false;
      this->point_step = 0ul;
      this->row_step = 0ul;
      this->is_dense = false;
      this->format = "";
    }
  }

  explicit CompressedPointCloud2_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    format(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->height = 0ul;
      this->width = 0ul;
      this->is_bigendian = false;
      this->point_step = 0ul;
      this->row_step = 0ul;
      this->is_dense = false;
      this->format = "";
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _height_type =
    uint32_t;
  _height_type height;
  using _width_type =
    uint32_t;
  _width_type width;
  using _fields_type =
    std::vector<sensor_msgs::msg::PointField_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<sensor_msgs::msg::PointField_<ContainerAllocator>>>;
  _fields_type fields;
  using _is_bigendian_type =
    bool;
  _is_bigendian_type is_bigendian;
  using _point_step_type =
    uint32_t;
  _point_step_type point_step;
  using _row_step_type =
    uint32_t;
  _row_step_type row_step;
  using _compressed_data_type =
    std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>>;
  _compressed_data_type compressed_data;
  using _is_dense_type =
    bool;
  _is_dense_type is_dense;
  using _format_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _format_type format;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__height(
    const uint32_t & _arg)
  {
    this->height = _arg;
    return *this;
  }
  Type & set__width(
    const uint32_t & _arg)
  {
    this->width = _arg;
    return *this;
  }
  Type & set__fields(
    const std::vector<sensor_msgs::msg::PointField_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<sensor_msgs::msg::PointField_<ContainerAllocator>>> & _arg)
  {
    this->fields = _arg;
    return *this;
  }
  Type & set__is_bigendian(
    const bool & _arg)
  {
    this->is_bigendian = _arg;
    return *this;
  }
  Type & set__point_step(
    const uint32_t & _arg)
  {
    this->point_step = _arg;
    return *this;
  }
  Type & set__row_step(
    const uint32_t & _arg)
  {
    this->row_step = _arg;
    return *this;
  }
  Type & set__compressed_data(
    const std::vector<uint8_t, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<uint8_t>> & _arg)
  {
    this->compressed_data = _arg;
    return *this;
  }
  Type & set__is_dense(
    const bool & _arg)
  {
    this->is_dense = _arg;
    return *this;
  }
  Type & set__format(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->format = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    point_cloud_interfaces::msg::CompressedPointCloud2_<ContainerAllocator> *;
  using ConstRawPtr =
    const point_cloud_interfaces::msg::CompressedPointCloud2_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<point_cloud_interfaces::msg::CompressedPointCloud2_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<point_cloud_interfaces::msg::CompressedPointCloud2_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      point_cloud_interfaces::msg::CompressedPointCloud2_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<point_cloud_interfaces::msg::CompressedPointCloud2_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      point_cloud_interfaces::msg::CompressedPointCloud2_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<point_cloud_interfaces::msg::CompressedPointCloud2_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<point_cloud_interfaces::msg::CompressedPointCloud2_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<point_cloud_interfaces::msg::CompressedPointCloud2_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__point_cloud_interfaces__msg__CompressedPointCloud2
    std::shared_ptr<point_cloud_interfaces::msg::CompressedPointCloud2_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__point_cloud_interfaces__msg__CompressedPointCloud2
    std::shared_ptr<point_cloud_interfaces::msg::CompressedPointCloud2_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const CompressedPointCloud2_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->height != other.height) {
      return false;
    }
    if (this->width != other.width) {
      return false;
    }
    if (this->fields != other.fields) {
      return false;
    }
    if (this->is_bigendian != other.is_bigendian) {
      return false;
    }
    if (this->point_step != other.point_step) {
      return false;
    }
    if (this->row_step != other.row_step) {
      return false;
    }
    if (this->compressed_data != other.compressed_data) {
      return false;
    }
    if (this->is_dense != other.is_dense) {
      return false;
    }
    if (this->format != other.format) {
      return false;
    }
    return true;
  }
  bool operator!=(const CompressedPointCloud2_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct CompressedPointCloud2_

// alias to use template instance with default allocator
using CompressedPointCloud2 =
  point_cloud_interfaces::msg::CompressedPointCloud2_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace point_cloud_interfaces

#endif  // POINT_CLOUD_INTERFACES__MSG__DETAIL__COMPRESSED_POINT_CLOUD2__STRUCT_HPP_
