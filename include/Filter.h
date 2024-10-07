#ifndef FILTER_H
#define FILTER_H

#include <math.h>

class QuadraticFilter
{
public:
    void setconfig(float alpha, float beta, float zeta, int batch_size);
    void initFilter();
    void runFilter(float time, float input[3], float output[3], float diff[3]);

private:
    float tempMatrix_AA[3][3] = {0};
    float tempMatrix_AAI[3][3] = {0};
    float tempMatrix_AA2[2][2] = {0};
    float tempMatrix_AA2I[2][2] = {0};
    float tempMatrix_AB[3][3] = {0};
    float tempMatrix_AB2[2][3] = {0};
    float resultMatrix_Q[3][3] = {0};
    float resultMatrix_L[2][3] = {0};
    float sample[4][100] = {0};
    int batch_size, sample_number;
    float prevData[3] = {0};
    float alpha, beta, zeta;
    // alpha is for LPF after QF
    // beta is mix rate of QF and LF
    // zeta is mix rate of sample, raw of filtered
};

class LPF
{ // Low Pass Filter
public:
    LPF(float f_cutoff_Hz, float dt); // Constructor
    float filter(float input);        // filter the input signal and get the filtered output signal

private:
    float cutoffFrequency_; // Cut-off frequency of the filter
    float prevOutput_;      // Previous output of the filter
    float dt_;              // Time step for the filter
    float alpha_;           // Coefficient for the filter
    int isInitialized_ = 0;   // Initialization flag
};

#endif