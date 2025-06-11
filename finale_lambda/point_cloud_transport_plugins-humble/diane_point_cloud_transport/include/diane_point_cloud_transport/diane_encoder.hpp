#pragma once

#include <torch/torch.h>
#include <vector>
#include <array>
#include <cstdint>

// dichiara le funzioni
int64_t diane_multinomial_i16(
    const torch::Tensor& points,
    torch::Tensor& output,
    float bw,
    float fps,
    float lambda,
    float dist_guarant,
    float default_color,
    float nan_color,
    int64_t n_points,
    float multiplier = 1.0f
);

std::vector<uint8_t> encode_diane_multinomial_i16(
    const torch::Tensor& input_pc,
    float bw,
    float fps,
    float lambda,
    float dist_guarant,
    const std::array<float,4>& default_col = {0.9f,0.9f,0.9f,1.0f},
    const std::array<float,4>& nan_col     = {1.0f,1.0f,1.0f,1.0f}
);

float color_to_float(const std::array<float,4>& color);
