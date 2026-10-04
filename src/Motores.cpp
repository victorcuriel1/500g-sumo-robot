#include "Motores.h"

Motor::Motor(int pwm, int dir, int canal, int frecuencia, int resolucion) {
  pinPWM = pwm;
  pinDIR = dir;
  canalPWM = canal;
  frecuenciaPWM = frecuencia;
  resolucionPWM = resolucion;
}

void Motor::begin() {
  pinMode(pinDIR, OUTPUT);

  ledcSetup(canalPWM, frecuenciaPWM, resolucionPWM);
  ledcAttachPin(pinPWM, canalPWM);

  parar();
}

void Motor::adelante(int velocidad) {
  velocidad = constrain(velocidad, 0, 255);

  digitalWrite(pinDIR, HIGH);
  ledcWrite(canalPWM, velocidad);
}

void Motor::atras(int velocidad) {
  velocidad = constrain(velocidad, 0, 255);

  digitalWrite(pinDIR, LOW);
  ledcWrite(canalPWM, velocidad);
}

void Motor::parar() {
  ledcWrite(canalPWM, 0);
}

void Motor::setVelocidad(int velocidad) {
  velocidad = constrain(velocidad, -255, 255);

  if (velocidad > 0) {
    adelante(velocidad);
  }
  else if (velocidad < 0) {
    atras(-velocidad);
  }
  else {
    parar();
  }
}
