#include "Filter.h"

void QuadraticFilter::setconfig(float alpha, float beta, float zeta, int batch_size)
{
    this->alpha = alpha;
    this->beta = beta;
    this->zeta = zeta;
    if (batch_size < 10)
    {
        this->batch_size = 10;
    }
    else
    {
        this->batch_size = batch_size;
    }
}

void QuadraticFilter::initFilter()
{
    this->sample[0][1] = -1;
    this->sample[0][2] = -2;
    this->sample_number = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            this->tempMatrix_AA[i][j] = pow(-1, i + j) + pow(-2, i + j);
        }
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            this->tempMatrix_AA2[i][j] = pow(-1, i + j) + pow(-2, i + j);
        }
    }
}

void QuadraticFilter::runFilter(float time, float input[3], float output[3], float diff[3])
{
    float legacyTime;
    float legacyData[3];
    float det3, det2;
    legacyTime = this->sample[0][this->sample_number];
    legacyData[0] = this->sample[1][this->sample_number];
    legacyData[1] = this->sample[2][this->sample_number];
    legacyData[2] = this->sample[3][this->sample_number];
    this->sample[0][this->sample_number] = time;
    this->sample[1][this->sample_number] = input[0];
    this->sample[2][this->sample_number] = input[1];
    this->sample[3][this->sample_number] = input[2];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            this->tempMatrix_AA[i][j] += pow(time, i + j) - pow(legacyTime, i + j);
            this->tempMatrix_AB[i][j] += pow(time, i) * input[j] - pow(legacyTime, i) * legacyData[j];
        }
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            this->tempMatrix_AA2[i][j] += pow(time, i + j) - pow(legacyTime, i + j);
        }
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            this->tempMatrix_AB2[i][j] += pow(time, i) * input[j] - pow(legacyTime, i) * legacyData[j];
        }
    }
    det3 = 0;
    for (int i = 0; i < 3; i++)
    {
        det3 += tempMatrix_AA[0][i] * (tempMatrix_AA[1][(i + 1) % 3] * tempMatrix_AA[2][(i + 2) % 3] - tempMatrix_AA[1][(i + 2) % 3] * tempMatrix_AA[2][(i + 1) % 3]);
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            this->tempMatrix_AAI[j][i] = (tempMatrix_AA[(i + 1) % 3][(j + 1) % 3] * tempMatrix_AA[(i + 2) % 3][(j + 2) % 3] - tempMatrix_AA[(i + 2) % 3][(j + 1) % 3] * tempMatrix_AA[(i + 1) % 3][(j + 2) % 3]) / det3;
        }
    }
    det2 = tempMatrix_AA2[0][0] * tempMatrix_AA2[1][1] - tempMatrix_AA2[0][1] * tempMatrix_AA2[1][0];
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            this->tempMatrix_AA2I[j][i] = tempMatrix_AA2[1 - i][1 - j] * pow(-1, i + j) / det2;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            resultMatrix_Q[i][j] = 0;
            for (int k = 0; k < 3; k++)
            {
                resultMatrix_Q[i][j] += tempMatrix_AAI[i][k] * tempMatrix_AB[k][j];
            }
        }
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            resultMatrix_L[i][j] = 0;
            for (int k = 0; k < 2; k++)
            {
                resultMatrix_L[i][j] += tempMatrix_AA2I[i][k] * tempMatrix_AB2[k][j];
            }
        }
    }
    for (int i = 0; i < 3; i++)
    {
        diff[i] = resultMatrix_L[1][i];
    }
    for (int i = 0; i < 3; i++)
    {
        output[i] = 0;
        for (int j = 0; j < 3; j++)
        {
            output[i] += resultMatrix_Q[j][i] * pow(time, j) * (1 - alpha) * (1 - beta);
        }
        for (int j = 0; j < 2; j++)
        {
            output[i] += resultMatrix_L[j][i] * pow(time, j) * (1 - alpha) * beta;
        }
        output[i] += prevData[i] * alpha;
    }
    for (int i = 0; i < 3; i++)
    {
        prevData[i] = output[i];
    }
    for (int i = 0; i < 3; i++)
    {
        this->sample[i + 1][this->sample_number] = input[i] * zeta + output[i] * (1 - zeta);
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            this->tempMatrix_AB[i][j] += pow(time, i) * (output[j] - input[j]) * (1 - zeta);
        }
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            this->tempMatrix_AB2[i][j] += pow(time, i) * (output[j] - input[j]) * (1 - zeta);
        }
    }
    this->sample_number++;
    if (this->sample_number >= this->batch_size)
    {
        this->sample_number = 0;
    }
}

LPF::LPF(float f_cutoff_Hz, float dt) : cutoffFrequency_(f_cutoff_Hz), prevOutput_(0.0f), dt_(dt)
{
    float tau = 1 / (2 * M_PI * cutoffFrequency_); // sec, time constant of the transfer function of the lpf
    alpha_ = dt_ / (tau + dt_);                    // y(n) = alpha * x(n) + (1-a) * y(n-1)
}

float LPF::filter(float input)
{
    float output = (1 - alpha_) * prevOutput_ + alpha_ * input;
    prevOutput_ = output;
    return output;
}