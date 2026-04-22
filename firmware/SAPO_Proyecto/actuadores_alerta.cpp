#include "actuadores_alerta.h"
#include "configuracion_pines.h"
#include <Arduino.h>

uint8_t inicializar_actuadores(void) {
    pinMode(PIN_LED_ALERTA, OUTPUT);
    pinMode(PIN_BUZZER_ALERTA, OUTPUT);
    
    // Asegurar que inician apagados (estado seguro inicializado)
    digitalWrite(PIN_LED_ALERTA, LOW);
    digitalWrite(PIN_BUZZER_ALERTA, LOW);
    
    return ESTADO_OK;
}

uint8_t activar_alerta_fisica(void) {
    digitalWrite(PIN_LED_ALERTA, HIGH);
    digitalWrite(PIN_BUZZER_ALERTA, HIGH);
    return ESTADO_OK;
}

uint8_t desactivar_alerta_fisica(void) {
    digitalWrite(PIN_LED_ALERTA, LOW);
    digitalWrite(PIN_BUZZER_ALERTA, LOW);
    return ESTADO_OK;
}
