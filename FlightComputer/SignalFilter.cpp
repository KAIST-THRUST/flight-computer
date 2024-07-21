#include <cmath>
#include "SignalFilter.h"

LPF::LPF(float f_cutoff_Hz, float dt) : cutoffFrequency_(f_cutoff_Hz), prevOutput_(0.0f), dt_(dt) {
  float tau = 1 / (2 * M_PI * cutoffFrequency_); // sec, time constant of the transfer function of the lpf
  alpha_ = dt_ / (tau + dt_); // y(n) = alpha * x(n) + (1-a) * y(n-1)
}

float LPF::filter(float input) {
  float output = (1 - alpha_) * prevOutput_ + alpha_ * input;
  prevOutput_ = output;
  return output;
}