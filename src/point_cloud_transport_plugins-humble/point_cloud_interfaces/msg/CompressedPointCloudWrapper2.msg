# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: Czech Technical University in Prague .. 2019, paplhjak

std_msgs/Header header

uint32 height
uint32 width

sensor_msgs/PointField[] fields

bool is_bigendian

uint32 point_step

uint32 row_step

uint8[] compressed_data

float64 encoding_time_diane

uint64 timestamp

uint32 numberpoints

uint64 bw

uint32 fps

uint32 lambdaparam

bool is_dense

# compression format used (e.g. draco, zlib, etc.)
string format
