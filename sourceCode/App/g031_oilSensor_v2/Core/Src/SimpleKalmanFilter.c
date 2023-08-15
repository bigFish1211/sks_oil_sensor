/*
 * SimpleKalmanFilter.c
 *
 *  Created on: Aug 23, 2022
 *      Author: icepe
 */


void SimpleKalmanFilter(float mea_e, float est_e, float q);
float updateEstimate(float mea);
void setMeasurementError(float mea_e);
void setEstimateError(float est_e);
void setProcessNoise(float q);
float getKalmanGain();
float getEstimateError();
