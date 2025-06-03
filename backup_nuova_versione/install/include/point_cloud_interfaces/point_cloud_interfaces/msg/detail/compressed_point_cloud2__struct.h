// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from point_cloud_interfaces:msg/CompressedPointCloud2.idl
// generated code does not contain a copyright notice

#ifndef POINT_CLOUD_INTERFACES__MSG__DETAIL__COMPRESSED_POINT_CLOUD2__STRUCT_H_
#define POINT_CLOUD_INTERFACES__MSG__DETAIL__COMPRESSED_POINT_CLOUD2__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'fields'
#include "sensor_msgs/msg/detail/point_field__struct.h"
// Member 'compressed_data'
#include "rosidl_runtime_c/primitives_sequence.h"
// Member 'format'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/CompressedPointCloud2 in the package point_cloud_interfaces.
/**
  * SPDX-License-Identifier: BSD-3-Clause
  * SPDX-FileCopyrightText: Czech Technical University in Prague .. 2019, paplhjak
 */
typedef struct point_cloud_interfaces__msg__CompressedPointCloud2
{
  std_msgs__msg__Header header;
  uint32_t height;
  uint32_t width;
  sensor_msgs__msg__PointField__Sequence fields;
  bool is_bigendian;
  uint32_t point_step;
  uint32_t row_step;
  rosidl_runtime_c__uint8__Sequence compressed_data;
  bool is_dense;
  /// compression format used (e.g. draco, zlib, etc.)
  rosidl_runtime_c__String format;
} point_cloud_interfaces__msg__CompressedPointCloud2;

// Struct for a sequence of point_cloud_interfaces__msg__CompressedPointCloud2.
typedef struct point_cloud_interfaces__msg__CompressedPointCloud2__Sequence
{
  point_cloud_interfaces__msg__CompressedPointCloud2 * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} point_cloud_interfaces__msg__CompressedPointCloud2__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // POINT_CLOUD_INTERFACES__MSG__DETAIL__COMPRESSED_POINT_CLOUD2__STRUCT_H_
