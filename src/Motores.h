#ifndef MOTORES_H
#define MOTORES_H

#include <Arduino.h>

class Motor {
  private:
    int pinPWM;
    int pinDIR;
    int canalPWM;
    int frecuenciaPWM;
    int resolucionPWM;

  public:
    Motor(int pwm, int dir, int canal, int frecuencia, int resolucion);

    void begin();

    void adelante(int velocidad);
    void atras(int velocidad);
    void parar();

    void setVelocidad(int velocidad);
};

#endif
