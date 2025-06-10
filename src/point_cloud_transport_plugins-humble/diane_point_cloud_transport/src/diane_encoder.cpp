// src/diane_encoder.cpp
// C++ implementation of DIANE multinomial i16 encoder using LibTorch

#include <torch/torch.h>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cstring>
#include <array>
#include <limits>
#include <cuda_runtime.h>
#include <functional>            // std::bind
#include <rclcpp/rclcpp.hpp>     // logger
#include <diane_point_cloud_transport/diane_encoder.hpp>
#include <chrono>
#include <rcl_interfaces/msg/parameter_descriptor.hpp>
// Utility: pack RGBA floats (0–1) into a single float via bit reinterpretation
inline float color_to_float(const std::array<float,4>& color) {
    uint32_t r = static_cast<uint32_t>(color[0] * 255.0f) & 0xFF;
    uint32_t g = static_cast<uint32_t>(color[1] * 255.0f) & 0xFF;
    uint32_t b = static_cast<uint32_t>(color[2] * 255.0f) & 0xFF;
    uint32_t a = static_cast<uint32_t>(color[3] * 255.0f) & 0xFF;
    uint32_t packed = (r << 24) | (g << 16) | (b << 8) | a;
    float out;
    std::memcpy(&out, &packed, sizeof(out));
    return out;
}

int64_t diane_multinomial_i16(
    const torch::Tensor& points,    // [H, W, 4], float32
    torch::Tensor& output,          // preallocated [N*4], int16: 3*int16 XYZ + int16 color
   // torch::Tensor& points_reshaped, // preallocated [N, 4]
   // torch::Tensor& x,               // prealloc [N]
   // torch::Tensor& y,               // prealloc [N]
   // torch::Tensor& z,               // prealloc [N]
   // torch::Tensor& c,               // prealloc [N]
    float bw,                       // bandwidth bits/sec
    float fps,                      // frames per second
    float dist_guarant,             // guaranteed distance
    float default_color,            // packed float
    float nan_color,                // packed float (unused)
    int64_t n_points,
    float multiplier                // scale factor
) {
 
  /*    // Constants
  const float PSZ = 4 * 16.0f;
    const float MFS = bw / fps;
    int64_t max_pts = std::min<int64_t>(MFS / PSZ, n_points);
    

 points_reshaped.copy_(points.view({-1,4}));
    x.copy_(points_reshaped.select(1,0));
    y.copy_(points_reshaped.select(1,1));
    z.copy_(points_reshaped.select(1,2));
    c.copy_(points_reshaped.select(1,3));
*/
        // Calcola max_pts in modo robusto
    const float PSZ = 4 * 16.0f;
    const float MFS = bw / fps;
    int64_t max_pts = static_cast<int64_t>(MFS / PSZ);
    
    // Garantisce valori entro limiti sicuri
    max_pts = std::max<int64_t>(1, std::min(max_pts, n_points));
    auto points_reshaped = points.contiguous().view({-1, 4});
    auto x = points_reshaped.select(1, 0);
    auto y = points_reshaped.select(1, 1);
    auto z = points_reshaped.select(1, 2);
    auto c = points_reshaped.select(1, 3);
    auto nan_mask = c.isnan();
    c.masked_fill_(nan_mask, default_color);

    auto invalid = x.isnan() | x.isinf() | (z.mul(multiplier) > std::numeric_limits<int16_t>::max());
    auto valid_mask = invalid.logical_not().to(torch::kFloat32);

auto valid_z = z.masked_select(valid_mask.to(torch::kBool));
float max_z = valid_z.numel() > 0 ? valid_z.max().item<float>() : 1e-6f; // Evita divisione per zero con valore piccolo
auto invdist = 1.0f / (z.div(max_z + 1e-6f).add(1e-6f));
auto priority = torch::nan_to_num(invdist.mul(valid_mask), 0.0f);


    float top = priority.max().item<float>();
    auto close = z.lt(dist_guarant);
    priority.masked_fill_(close, top);
    auto indices = priority.multinomial(max_pts, false);
    int64_t num_valid = indices.numel();
    
    
    if (num_valid == 0) return 0;

    
        auto x_sel = x.index_select(0, indices).mul(multiplier).to(torch::kInt16);
    auto y_sel = y.index_select(0, indices).mul(multiplier).to(torch::kInt16);
    auto z_sel = z.index_select(0, indices).mul(multiplier).to(torch::kInt16);
    auto c_sel = c.index_select(0, indices).contiguous();
int64_t M  = c_sel.size(0);
// Bit-cast float32 → int32 SENZA passare per la CPU ---------------------
auto c_int = torch::empty({M},
              torch::dtype(torch::kInt32).device(c_sel.device()));  // [M] i32 GPU
cudaError_t err = cudaMemcpy(                       // device → device
        c_int.data_ptr<int32_t>(),                  // dst
        c_sel.data_ptr<float>(),                    // src
        M * sizeof(float),
        cudaMemcpyDeviceToDevice);
if (err != cudaSuccess)
    throw std::runtime_error("cudaMemcpyDeviceToDevice fallita in diane_multinomial_i16!");

// -----------------------------------------------------------------------
  
    // std::memcpy(c_int.data_ptr<int32_t>(), c_sel.data_ptr<float>(), M * sizeof(float));
    // Extract bytes
    auto r_byte = torch::bitwise_and(torch::bitwise_right_shift(c_int,24), 0xFF).to(torch::kInt16);
    auto g_byte = torch::bitwise_and(torch::bitwise_right_shift(c_int,16), 0xFF).to(torch::kInt16);
    auto b_byte = torch::bitwise_and(torch::bitwise_right_shift(c_int,8),  0xFF).to(torch::kInt16);
    // Reduce bits to R6 G6 B4
    auto r6 = torch::bitwise_right_shift(r_byte,2);
    auto g6 = torch::bitwise_right_shift(g_byte,2);
    auto b4 = torch::bitwise_right_shift(b_byte,4);
    auto rgb664 = torch::bitwise_or(torch::bitwise_or(
        torch::bitwise_left_shift(r6,10),
        torch::bitwise_left_shift(g6, 4)
    ), b4).to(torch::kInt16);

    // Write output: interleaved XYZ then colors
    // XYZ (3*int16) at strides of 4, color after
    
        int64_t required_size = num_valid * 4;
    if (output.numel() < required_size) {
        output = torch::empty({required_size}, output.options());
    }
    
    auto output_view = output.view({num_valid, 4});
    output_view.select(1,0) = x_sel;
        output_view.select(1,1) = y_sel;
            output_view.select(1,2) = z_sel;
                output_view.select(1,3) = rgb664;
               
    
  //  for (int64_t i = 0; i < num_valid; ++i) {
   //     output[i*4 + 0] = x_sel[i];
     //   output[i*4 + 1] = y_sel[i];
       // output[i*4 + 2] = z_sel[i];
       // output[i*4 + 3] = rgb664[i];
    //}

    return num_valid * sizeof(int16_t) * 4; // bytes
}

// Public API: encode function
std::vector<uint8_t> encode_diane_multinomial_i16(
    const torch::Tensor& input_pc,
    float bw,
    float fps,
    float dist_guarant,
    const std::array<float,4>& default_col,
    const std::array<float,4>& nan_col
) {
    torch::Device device = torch::kCUDA;

    auto pc = input_pc.to(device);
    int64_t H = pc.size(0), W = pc.size(1), N = H * W;
    torch::Tensor out = torch::empty({1}, torch::dtype(torch::kInt16).device(device));
    
    float dflt = color_to_float(default_col);
    float nanf = color_to_float(nan_col);
    
    int64_t bufSize = diane_multinomial_i16(
        pc, out, bw, fps, dist_guarant, dflt, nanf, N, 10.0f
    );



if (bufSize <= 0 || bufSize > out.numel() * sizeof(int16_t)) {
    throw std::runtime_error("bufSize non valido in diane_multinomial_i16!");
}
cudaDeviceSynchronize();
    auto out_cpu = out.to(torch::kCPU).contiguous();
    std::vector<uint8_t> result(bufSize);
    std::memcpy(result.data(), out_cpu.data_ptr(), bufSize);

    return result;
}

