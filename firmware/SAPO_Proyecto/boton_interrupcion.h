#ifndef BOTON_INTERRUPCION_H
#define BOTON_INTERRUPCION_H

#include <stdint.h>

// Inicializa el pin del botón y vincula la interrupción de hardware.
uint8_t inicializar_boton(void);

// Verifica si el botón fue presionado.
// Retorna 1 si fue presionado, 0 si no.
uint8_t verificar_boton_presionado(void);

// Reinicia la bandera del botón a su estado normal.
uint8_t limpiar_bandera_boton(void);

#endif // BOTON_INTERRUPCION_H
