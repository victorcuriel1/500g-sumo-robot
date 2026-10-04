#include <Arduino.h>
#include "Motores.h"

const int PIN_PWM_L = 22;
const int PIN_DIR_L = 18;

const int PIN_PWM_R = 23;
const int PIN_DIR_R = 19;

const int CH_L = 0;
const int CH_R = 1;

const int FRECUENCIA = 20000;
const int RESOLUCION = 8;

const int SENSOR_FRONT_LEFT  = 32;
const int SENSOR_FRONT_RIGHT = 35;
const int SENSOR_LEFT        = 33;
const int SENSOR_RIGHT       = 34;

const int MicroStartPin = 27;

Motor motorIzq(PIN_PWM_L, PIN_DIR_L, CH_L, FRECUENCIA, RESOLUCION);
Motor motorDer(PIN_PWM_R, PIN_DIR_R, CH_R, FRECUENCIA, RESOLUCION);

enum Estado {
  ATAQUE,
  GIRO_IZQ,
  GIRO_DER,
  BUSQUEDA,
  PARADO
};

Estado estadoActual = PARADO;
Estado estadoPrev   = PARADO;

const int SEARCH_SPEED = 180;

struct RampaAtaque {
  bool activa = false;
  int v = 0;
  int vmax = 200;
  int paso = 75;
  unsigned long delayMs = 100;
  unsigned long tLast = 0;
} rampa;

void iniciarAtaqueConRampa(
  int v_ini = 50,
  int vmax = 200,
  int paso = 75,
  unsigned long delayMs = 500
) {
  rampa.activa  = true;
  rampa.v       = v_ini;
  rampa.vmax    = vmax;
  rampa.paso    = paso;
  rampa.delayMs = delayMs;
  rampa.tLast   = millis();

  motorIzq.adelante(rampa.v);
  motorDer.adelante(rampa.v);
}

void cancelarRampa() {
  rampa.activa = false;
}

void actualizarAtaqueConRampa() {
  if (!rampa.activa) return;

  unsigned long ahora = millis();

  if (ahora - rampa.tLast >= rampa.delayMs) {
    rampa.tLast = ahora;
    rampa.v += rampa.paso;

    if (rampa.v >= rampa.vmax) {
      rampa.v = rampa.vmax;
      rampa.activa = false;
    }

    motorIzq.adelante(rampa.v);
    motorDer.adelante(rampa.v);
  }
}

enum Side {
  SIDE_LEFT,
  SIDE_RIGHT
};

static Side lastSide = SIDE_RIGHT;

void setup() {
  motorIzq.begin();
  motorDer.begin();

  pinMode(SENSOR_FRONT_LEFT, INPUT);
  pinMode(SENSOR_FRONT_RIGHT, INPUT);
  pinMode(SENSOR_LEFT, INPUT);
  pinMode(SENSOR_RIGHT, INPUT);

  pinMode(MicroStartPin, INPUT);
}

void loop() {
  bool frontLeft  = digitalRead(SENSOR_FRONT_LEFT);
  bool frontRight = digitalRead(SENSOR_FRONT_RIGHT);
  bool left       = digitalRead(SENSOR_LEFT);
  bool right      = digitalRead(SENSOR_RIGHT);

  if (digitalRead(MicroStartPin) == LOW) {
    motorIzq.parar();
    motorDer.parar();

    cancelarRampa();
    estadoPrev = PARADO;

    return;
  }

  if (frontLeft && frontRight) {
    estadoActual = ATAQUE;
  }
  else if ((frontLeft && left) || left) {
    lastSide = SIDE_LEFT;
    estadoActual = GIRO_IZQ;
  }
  else if ((frontRight && right) || right) {
    lastSide = SIDE_RIGHT;
    estadoActual = GIRO_DER;
  }
  else {
    estadoActual = BUSQUEDA;
  }

  switch (estadoActual) {

    case ATAQUE: {

      if (estadoPrev != ATAQUE) {
        iniciarAtaqueConRampa(40, 255, 50, 45);
      }

      actualizarAtaqueConRampa();

      if (!rampa.activa) {
        motorIzq.adelante(255);
        motorDer.adelante(255);
      }

    } break;

    case GIRO_IZQ:

      cancelarRampa();

      motorIzq.atras(128);
      motorDer.adelante(128);

      break;

    case GIRO_DER:

      cancelarRampa();

      motorIzq.adelante(128);
      motorDer.atras(128);

      break;

    case BUSQUEDA:

      cancelarRampa();

      if (frontLeft && frontRight) {
        estadoActual = ATAQUE;
      }
      else {
        if (lastSide == SIDE_LEFT) {
          motorIzq.atras(SEARCH_SPEED);
          motorDer.adelante(SEARCH_SPEED);
        }
        else {
          motorIzq.adelante(SEARCH_SPEED);
          motorDer.atras(SEARCH_SPEED);
        }
      }

      break;

    default:

      cancelarRampa();

      motorIzq.parar();
      motorDer.parar();

      break;
  }

  estadoPrev = estadoActual;
}
