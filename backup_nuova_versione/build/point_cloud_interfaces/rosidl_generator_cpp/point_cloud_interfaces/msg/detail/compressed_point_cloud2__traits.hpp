// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from point_cloud_interfaces:msg/CompressedPointCloud2.idl
// generated code does not contain a copyright notice

#ifndef POINT_CLOUD_INTERFACES__MSG__DETAIL__COMPRESSED_POINT_CLOUD2__TRAITS_HPP_
#define POINT_CLOUD_INTERFACES__MSG__DETAIL__COMPRESSED_POINT_CLOUD2__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "point_cloud_interfaces/msg/detail/compressed_point_cloud2__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'fields'
#include "sensor_msgs/msg/detail/point_field__traits.hpp"

namespace point_cloud_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const CompressedPointCloud2 & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: height
  {
    out << "height: ";
    rosidl_generator_traits::value_to_yaml(msg.height, out);
    out << ", ";
  }

  // member: width
  {
    out << "width: ";
    rosidl_generator_traits::value_to_yaml(msg.width, out);
    out << ", ";
  }

  // member: fields
  {
    if (msg.fields.size() == 0) {
      out << "fields: []";
    } else {
      out << "fields: [";
      size_t pending_items = msg.fields.size();
      for (auto item : msg.fields) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: is_bigendian
  {
    out << "is_bigendian: ";
    rosidl_generator_traits::value_to_yaml(msg.is_bigendian, out);
    out << ", ";
  }

  // member: point_step
  {
    out << "point_step: ";
    rosidl_generator_traits::value_to_yaml(msg.point_step, out);
    out << ", ";
  }

  // member: row_step
  {
    out << "row_step: ";
    rosidl_generator_traits::value_to_yaml(msg.row_step, out);
    out << ", ";
  }

  // member: compressed_data
  {
    if (msg.compressed_data.size() == 0) {
      out << "compressed_data: []";
    } else {
      out << "compressed_data: [";
      size_t pending_items = msg.compressed_data.size();
      for (auto item : msg.compressed_data) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: is_dense
  {
    out << "is_dense: ";
    rosidl_generator_traits::value_to_yaml(msg.is_dense, out);
    out << ", ";
  }

  // member: format
  {
    out << "format: ";
    rosidl_generator_traits::value_to_yaml(msg.format, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const CompressedPointCloud2 & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: height
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "height: ";
    rosidl_generator_traits::value_to_yaml(msg.height, out);
    out << "\n";
  }

  // member: width
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "width: ";
    rosidl_generator_traits::value_to_yaml(msg.width, out);
    out << "\n";
  }

  // member: fields
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.fields.size() == 0) {
      out << "fields: []\n";
    } else {
      out << "fields:\n";
      for (auto item : msg.fields) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: is_bigendian
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "is_bigendian: ";
    rosidl_generator_traits::value_to_yaml(msg.is_bigendian, out);
    out << "\n";
  }

  // member: point_step
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "point_step: ";
    rosidl_generator_traits::value_to_yaml(msg.point_step, out);
    out << "\n";
  }

  // member: row_step
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "row_step: ";
    rosidl_generator_traits::value_to_yaml(msg.row_step, out);
    out << "\n";
  }

  // member: compressed_data
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.compressed_data.size() == 0) {
      out << "compressed_data: []\n";
    } else {
      out << "compressed_data:\n";
      for (auto item : msg.compressed_data) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: is_dense
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "is_dense: ";
    rosidl_generator_traits::value_to_yaml(msg.is_dense, out);
    out << "\n";
  }

  // member: format
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "format: ";
    rosidl_generator_traits::value_to_yaml(msg.format, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const CompressedPointCloud2 & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace point_cloud_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use point_cloud_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const point_cloud_interfaces::msg::CompressedPointCloud2 & msg,
  std::ostream & out, size_t indentation = 0)
{
  point_cloud_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use point_cloud_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const point_cloud_interfaces::msg::CompressedPointCloud2 & msg)
{
  return point_cloud_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<point_cloud_interfaces::msg::CompressedPointCloud2>()
{
  return "point_cloud_interfaces::msg::CompressedPointCloud2";
}

template<>
inline const char * name<point_cloud_interfaces::msg::CompressedPointCloud2>()
{
  return "point_cloud_interfaces/msg/CompressedPointCloud2";
}

template<>
struct has_fixed_size<point_cloud_interfaces::msg::CompressedPointCloud2>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<point_cloud_interfaces::msg::CompressedPointCloud2>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<point_cloud_interfaces::msg::CompressedPointCloud2>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // POINT_CLOUD_INTERFACES__MSG__DETAIL__COMPRESSED_POINT_CLOUD2__TRAITS_HPP_
