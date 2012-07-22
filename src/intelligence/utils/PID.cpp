#include "PID.h"
#include "config.h"
#include <iostream>
using namespace std;

#define MIN(A,B)	((A)<(B)?(A):(B))
#define MAX(A,B)	((A)<(B)?(B):(A))
#define ABS(A)		((A)>(0)?(A):(-A))


CONTROLLER_S::CONTROLLER_S(double Kp, double Ki, double Kd, double saidaMax, double integMax)
	: Kp(Kp), Ki(Ki), Kd(Kd),
	erro(0.0), erro_anterior(0.0), erro_integ(0.0), erro_dif(0.0),
	saidaMax(saidaMax), integMax(integMax),
	saida(0.0), entrada(0.0), entrada_des(0.0), realimentacao(0.0) {}

void pidService(CONTROLLER_S& Controlador){
    // calcula o erro
    Controlador.erro = Controlador.entrada - Controlador.realimentacao;
    // calcula a integral do erro
    Controlador.erro_integ += Controlador.erro;
    // limita a integral do erro
    Controlador.erro_integ = MAX(Controlador.erro_integ, (-ABS(Controlador.integMax/Controlador.Ki)));
    Controlador.erro_integ = MIN(Controlador.erro_integ,  ABS(Controlador.integMax/Controlador.Ki));
    // calcula a derivada do erro
    Controlador.erro_dif = Controlador.erro - Controlador.erro_anterior;
    Controlador.erro_anterior = Controlador.erro;
    // calcula o PID
    Controlador.saida=	(Controlador.Kp*Controlador.erro)
                		+(Controlador.Ki*Controlador.erro_integ)
                		+(Controlador.Kd*Controlador.erro_dif);
    // limita a saída
    Controlador.saida = MAX(Controlador.saida, -Controlador.saidaMax);
    Controlador.saida = MIN(Controlador.saida, Controlador.saidaMax);
#ifdef OVER_SPEED_LIMIT
	if(Controlador.saida == Controlador.saidaMax)
		cout << "Warning: MAX LINEAR SPEED LIMIT REACHED." << endl;
#endif
 }