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
    double erro;          // error (input-feedback)
    double erro_anterior;     // previous error
    double erro_integ;    // integral of error
    double erro_dif;     // derivative of error
    // limits
    double integMax;      // integral wind-up limit
    double saidaMax;      // motor output clamp (limit)
    // input and output
    double saida;         // motor output
    double entrada;          // user input
    double entrada_des;          // user input
    double realimentacao;       // motor feedback
};

void pidService(CONTROLLER_S& Controller);


#endif // PID_H