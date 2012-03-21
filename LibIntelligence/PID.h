#pragma once
#ifndef PID_H
#define PID_H

struct CONTROLLER_S
{
	CONTROLLER_S(double Kp, double Ki, double Kd, double saidaMax, double integMax);
    // controls coefficients
    double Kp;             // proportional gain
    double Ki;             // integral gain
    double Kd;             // derivative gain
    // state variables
    volatile double erro;          // error (input-feedback)
    volatile double erro_anterior;     // previous error
    volatile double erro_integ;    // integral of error
    volatile double erro_dif;     // derivative of error
    // limits
    double integMax;      // integral wind-up limit
    double saidaMax;      // motor output clamp (limit)
    // input and output
    volatile double saida;         // motor output
    volatile double entrada;          // user input
    volatile double entrada_des;          // user input
    volatile double realimentacao;       // motor feedback
};

void pidService(CONTROLLER_S& Controller);


#endif // PID_H