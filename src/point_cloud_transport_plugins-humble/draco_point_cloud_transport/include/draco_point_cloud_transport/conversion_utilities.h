// Copyright (c) 2023, Open Source Robotics Foundation, Inc.
// Copyright (c) 2023, Czech Technical University in Prague
// Copyright (c) 2019, paplhjak
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the copyright holder nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef DRACO_POINT_CLOUD_TRANSPORT__CONVERSION_UTILITIES_H_
#define DRACO_POINT_CLOUD_TRANSPORT__CONVERSION_UTILITIES_H_

namespace draco_point_cloud_transport
{
//! copies header, width, ... between clouds
  template < typename PC1, typename PC2 >
  void copyCloudMetadata(PC1 & target, const PC2 & source)
  {
    target.header = source.header;
    target.height = source.height;
    target.width = source.width;
    target.fields = source.fields;
    target.is_bigendian = source.is_bigendian;
    target.point_step = source.point_step;
    target.row_step = source.row_step;
    target.is_dense = source.is_dense;
  }

}  // namespace draco_point_cloud_transport

#endif  // DRACO_POINT_CLOUD_TRANSPORT__CONVERSION_UTILITIES_H_
