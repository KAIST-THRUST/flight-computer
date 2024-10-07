#include "util.h"

void updateServoFromSerial(NonBlockingServo &serv) {
  static int degree = 0;
  // Read data from serial if sent
  if (Serial.available()) {
    Serial.readBytes((char *)&degree, 1);
  }
  // Rotate servo if it is not rotating
  if (!serv.isrotating()) {
    serv.rotate(degree);
  }
}

void printErrorMessageToSerial(String s) {
  Serial.print("Error: ");
  Serial.println(s);
}

void printSensorDataToSerial(const Sensor &sensor) {
  Serial.println(sensor.toString());
}

float dotProduct(float v_A[3], float v_B[3]) {
  return v_A[0] * v_B[0] + v_A[1] * v_B[1] + v_A[2] * v_B[2];
}

float calcNorm(float v[3]) { return sqrt(dotProduct(v, v)); }

void crossProduct(float v_A[3], float v_B[3], float res[3]) {
  res[0] = v_A[1] * v_B[2] - v_A[2] * v_B[1];
  res[1] = -(v_A[0] * v_B[2] - v_A[2] * v_B[0]);
  res[2] = v_A[0] * v_B[1] - v_A[1] * v_B[0];
  return;
}

void matrixVecMult(float T[3][3], float b[3], float res[3]) {
  res[0] = T[0][0] * b[0] + T[0][1] * b[1] + T[0][2] * b[2];
  res[1] = T[1][0] * b[0] + T[1][1] * b[1] + T[1][2] * b[2];
  res[2] = T[2][0] * b[0] + T[2][1] * b[1] + T[2][2] * b[2];
  return;
}

void matrixTranspose(float A[3][3], float res[3][3]) {
  res[0][0] = A[0][0];
  res[0][1] = A[1][0];
  res[0][2] = A[2][0];
  res[1][0] = A[0][1];
  res[1][1] = A[1][1];
  res[1][2] = A[2][1];
  res[2][0] = A[0][2];
  res[2][1] = A[1][2];
  res[2][2] = A[2][2];
  return;
}


void quat_to_DCM(float quat[4], float dcm[3][3]) {
  float q0, q1, q2, q3;
  float q0q0, q1q1, q2q2, q3q3, q0q1, q0q2, q0q3, q1q2, q1q3, q2q3;

  q0 = quat[3];
  q1 = quat[0];
  q2 = quat[1];
  q3 = quat[2];

  // for calculation
  q0q0 = q0 * q0;
  q1q1 = q1 * q1;
  q2q2 = q2 * q2;
  q3q3 = q3 * q3;
  q0q1 = q0 * q1;
  q0q2 = q0 * q2;
  q0q3 = q0 * q3;
  q1q2 = q1 * q2;
  q1q3 = q1 * q3;
  q2q3 = q2 * q3;

  // construct elements of DCM
  dcm[0][0] = q0q0 + q1q1 - q2q2 - q3q3;
  dcm[0][1] = 2 * (q1q2 + q0q3);
  dcm[0][2] = 2 * (q1q3 - q0q2);

  dcm[1][0] = 2 * (q1q2 - q0q3);
  dcm[1][1] = q0q0 - q1q1 + q2q2 - q3q3;
  dcm[1][2] = 2 * (q2q3 + q0q1);

  dcm[2][0] = 2 * (q1q3 + q0q2);
  dcm[2][1] = 2 * (q2q3 - q0q1);
  dcm[2][2] = q0q0 - q1q1 - q2q2 + q3q3;

  return;
}

void lla_to_ECEF(float lat_deg, float lon_deg,
                             float alt_wgs84, float r_ECEF_m[3]) {
  
  const float a_e = 6378137; // m, equatorial radius of Earth
  const float e_2 = 0.006694379990141; // eccentricity squared
  
  float phi, lam, h;
  phi = lat_deg * DEG_TO_RAD; // rad, latitude
  lam = lon_deg * DEG_TO_RAD; // rad, longitude
  h = alt_wgs84;              // m, ellipsoidal altitude (wgs84)

  float sinLam, cosLam, sinPhi, cosPhi;
  sinLam = sin(lam);
  cosLam = cos(lam);
  sinPhi = sin(phi);
  cosPhi = cos(phi);

  float N;
  N = a_e / sqrt(1 - e_2 * sinLam * sinLam);

  r_ECEF_m[0] = (N + h) * cosPhi * cosLam;    // m
  r_ECEF_m[1] = (N + h) * cosPhi * sinLam;    // m
  r_ECEF_m[2] = ((1 - e_2) * N + h) * sinPhi; // m

  return;
}

void get_dcm_ECEF_to_ENU(float lat_deg, float lon_deg,
                                     float dcm_ECEF_to_ENU[3][3]) {
  float phi = lat_deg * DEG_TO_RAD;
  float lam = lon_deg * DEG_TO_RAD;

  float sLam = sin(lam);
  float cLam = cos(lam);
  float sPhi = sin(phi);
  float cPhi = cos(phi);

  dcm_ECEF_to_ENU[0][0] = -sLam;
  dcm_ECEF_to_ENU[0][1] = cLam;
  dcm_ECEF_to_ENU[0][2] = 0;
  dcm_ECEF_to_ENU[1][0] = -sPhi * cLam;
  dcm_ECEF_to_ENU[1][1] = -sPhi * sLam;
  dcm_ECEF_to_ENU[1][2] = cPhi;
  dcm_ECEF_to_ENU[2][0] = cPhi * cLam;
  dcm_ECEF_to_ENU[2][1] = cPhi * sLam;
  dcm_ECEF_to_ENU[2][2] = sPhi;

  return;
}