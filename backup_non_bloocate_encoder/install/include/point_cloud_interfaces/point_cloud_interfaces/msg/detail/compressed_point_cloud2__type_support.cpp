// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from point_cloud_interfaces:msg/CompressedPointCloud2.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "point_cloud_interfaces/msg/detail/compressed_point_cloud2__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace point_cloud_interfaces
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void CompressedPointCloud2_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) point_cloud_interfaces::msg::CompressedPointCloud2(_init);
}

void CompressedPointCloud2_fini_function(void * message_memory)
{
  auto typed_message = static_cast<point_cloud_interfaces::msg::CompressedPointCloud2 *>(message_memory);
  typed_message->~CompressedPointCloud2();
}

size_t size_function__CompressedPointCloud2__fields(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<sensor_msgs::msg::PointField> *>(untyped_member);
  return member->size();
}

const void * get_const_function__CompressedPointCloud2__fields(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<sensor_msgs::msg::PointField> *>(untyped_member);
  return &member[index];
}

void * get_function__CompressedPointCloud2__fields(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<sensor_msgs::msg::PointField> *>(untyped_member);
  return &member[index];
}

void fetch_function__CompressedPointCloud2__fields(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const sensor_msgs::msg::PointField *>(
    get_const_function__CompressedPointCloud2__fields(untyped_member, index));
  auto & value = *reinterpret_cast<sensor_msgs::msg::PointField *>(untyped_value);
  value = item;
}

void assign_function__CompressedPointCloud2__fields(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<sensor_msgs::msg::PointField *>(
    get_function__CompressedPointCloud2__fields(untyped_member, index));
  const auto & value = *reinterpret_cast<const sensor_msgs::msg::PointField *>(untyped_value);
  item = value;
}

void resize_function__CompressedPointCloud2__fields(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<sensor_msgs::msg::PointField> *>(untyped_member);
  member->resize(size);
}

size_t size_function__CompressedPointCloud2__compressed_data(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return member->size();
}

const void * get_const_function__CompressedPointCloud2__compressed_data(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void * get_function__CompressedPointCloud2__compressed_data(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  return &member[index];
}

void fetch_function__CompressedPointCloud2__compressed_data(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const uint8_t *>(
    get_const_function__CompressedPointCloud2__compressed_data(untyped_member, index));
  auto & value = *reinterpret_cast<uint8_t *>(untyped_value);
  value = item;
}

void assign_function__CompressedPointCloud2__compressed_data(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<uint8_t *>(
    get_function__CompressedPointCloud2__compressed_data(untyped_member, index));
  const auto & value = *reinterpret_cast<const uint8_t *>(untyped_value);
  item = value;
}

void resize_function__CompressedPointCloud2__compressed_data(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<uint8_t> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CompressedPointCloud2_message_member_array[10] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(point_cloud_interfaces::msg::CompressedPointCloud2, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "height",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(point_cloud_interfaces::msg::CompressedPointCloud2, height),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "width",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(point_cloud_interfaces::msg::CompressedPointCloud2, width),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "fields",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<sensor_msgs::msg::PointField>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(point_cloud_interfaces::msg::CompressedPointCloud2, fields),  // bytes offset in struct
    nullptr,  // default value
    size_function__CompressedPointCloud2__fields,  // size() function pointer
    get_const_function__CompressedPointCloud2__fields,  // get_const(index) function pointer
    get_function__CompressedPointCloud2__fields,  // get(index) function pointer
    fetch_function__CompressedPointCloud2__fields,  // fetch(index, &value) function pointer
    assign_function__CompressedPointCloud2__fields,  // assign(index, value) function pointer
    resize_function__CompressedPointCloud2__fields  // resize(index) function pointer
  },
  {
    "is_bigendian",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(point_cloud_interfaces::msg::CompressedPointCloud2, is_bigendian),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "point_step",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(point_cloud_interfaces::msg::CompressedPointCloud2, point_step),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "row_step",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(point_cloud_interfaces::msg::CompressedPointCloud2, row_step),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "compressed_data",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(point_cloud_interfaces::msg::CompressedPointCloud2, compressed_data),  // bytes offset in struct
    nullptr,  // default value
    size_function__CompressedPointCloud2__compressed_data,  // size() function pointer
    get_const_function__CompressedPointCloud2__compressed_data,  // get_const(index) function pointer
    get_function__CompressedPointCloud2__compressed_data,  // get(index) function pointer
    fetch_function__CompressedPointCloud2__compressed_data,  // fetch(index, &value) function pointer
    assign_function__CompressedPointCloud2__compressed_data,  // assign(index, value) function pointer
    resize_function__CompressedPointCloud2__compressed_data  // resize(index) function pointer
  },
  {
    "is_dense",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(point_cloud_interfaces::msg::CompressedPointCloud2, is_dense),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "format",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(point_cloud_interfaces::msg::CompressedPointCloud2, format),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CompressedPointCloud2_message_members = {
  "point_cloud_interfaces::msg",  // message namespace
  "CompressedPointCloud2",  // message name
  10,  // number of fields
  sizeof(point_cloud_interfaces::msg::CompressedPointCloud2),
  CompressedPointCloud2_message_member_array,  // message members
  CompressedPointCloud2_init_function,  // function to initialize message memory (memory has to be allocated)
  CompressedPointCloud2_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CompressedPointCloud2_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CompressedPointCloud2_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace point_cloud_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<point_cloud_interfaces::msg::CompressedPointCloud2>()
{
  return &::point_cloud_interfaces::msg::rosidl_typesupport_introspection_cpp::CompressedPointCloud2_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, point_cloud_interfaces, msg, CompressedPointCloud2)() {
  return &::point_cloud_interfaces::msg::rosidl_typesupport_introspection_cpp::CompressedPointCloud2_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
