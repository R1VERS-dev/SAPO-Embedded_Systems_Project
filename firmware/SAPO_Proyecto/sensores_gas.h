#ifndef SENSORES_GAS_H
#define SENSORES_GAS_H

#include <stdint.h>

// Inicializa las configuraciones analógicas.
uint8_t inicializar_sensores_gas(void);

// Lee y filtra el valor del sensor de humo (MQ-2)
uint16_t leer_sensor_humo(void);

// Lee y filtra el valor del sensor de calidad de aire (MQ-135)
uint16_t leer_sensor_calidad_aire(void);

#endif // SENSORES_GAS_H
