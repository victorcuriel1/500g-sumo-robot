# Robot Sumo de 500 g

Robot Sumo de 500 g desarrollado con ESP32, sensores de detección de oponente y control independiente de motores DC mediante PWM.

El sistema utiliza una máquina de estados para definir el comportamiento del robot durante la competencia, incluyendo ataque, giro, búsqueda y detención.

## Hardware principal

- ESP32
- 2 motores DC
- Driver de motores
- 4 sensores de detección de oponente
- Microstart
- Estructura mecánica para categoría Sumo de 500 g

## Estrategia de control

El comportamiento del robot se organiza mediante los siguientes estados:

- `ATAQUE`
- `GIRO_IZQ`
- `GIRO_DER`
- `BUSQUEDA`
- `PARADO`

Cuando ambos sensores frontales detectan al oponente, el robot inicia un ataque frontal.

Si el oponente es detectado por uno de los sensores laterales, el robot gira hacia ese lado para alinearse.

Cuando no existe detección, el robot entra en modo de búsqueda y gira en función del último lado donde fue detectado el oponente.

## Rampa de ataque

El ataque utiliza una rampa de aceleración no bloqueante basada en `millis()`.

Esto permite incrementar progresivamente la velocidad de los motores sin detener la ejecución del programa.

## Control de motores

Los motores son controlados mediante PWM y una señal de dirección.

La clase `Motor` incluye funciones para:

- Avanzar
- Retroceder
- Detener
- Ajustar velocidad

## Sensores

El sistema utiliza cuatro sensores de detección:

- Frontal izquierdo
- Frontal derecho
- Lateral izquierdo
- Lateral derecho

## Microstart

El robot permanece detenido mientras el Microstart se encuentra en estado `LOW`.

Cuando la señal de inicio es habilitada, comienza la ejecución de la estrategia de combate.

## Archivos

Código principal:

`src/main.ino`

Control de motores:

`src/Motores.h`

`src/Motores.cpp`

## Nota

Los archivos `Motores.h` y `Motores.cpp` fueron reconstruidos posteriormente para mantener la interfaz utilizada por el código principal original.

## Autor

Victor Gabriel Curiel Gonzalez

Universidad Nacional de Asunción  
Facultad de Ingeniería  
Ingeniería Mecatrónica

## Licencia

Este proyecto está distribuido bajo la licencia MIT.
