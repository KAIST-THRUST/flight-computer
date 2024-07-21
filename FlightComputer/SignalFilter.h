#ifndef SIGNALFILTER_H
#define SIGNALFILTER_H

class LPF {
public:
  LPF(float f_cutoff_Hz, float dt);
  float filter(float input); // filter the input signal and get the filtered output signal

private:
  float cutoffFrequency_;
  float prevOutput_;
  float dt_; // Time step for the filter
  float alpha_;
};

#endif // SIGNALFILTER_H