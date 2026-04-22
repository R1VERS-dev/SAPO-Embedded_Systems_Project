#ifndef ACTUADORES_ALERTA_H
#define ACTUADORES_ALERTA_H

#include <stdint.h>

// Función para inicializar los pines del LED y Buzzer
// Retorna ESTADO_OK si se inicializan correctamente.
uint8_t inicializar_actuadores(void);

// Enciende el LED y el Buzzer
uint8_t activar_alerta_fisica(void);

// Apaga el LED y el Buzzer
uint8_t desactivar_alerta_fisica(void);

#endif // ACTUADORES_ALERTA_H
