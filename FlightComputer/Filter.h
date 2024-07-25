#ifndef FILTER_H
#define FILTER_H

#include <math.h>


class QuadraticFilter{
    public:
    void setconfig(float alpha,float beta, float zeta, int batch_size);
    void initFilter();
    void runFilter(float time, float input[3],float output[3],float diff[3]);

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
    int batch_size,sample_number;
    float prevData[3] = {0};
    float alpha,beta,zeta;
    
    

};

#endif