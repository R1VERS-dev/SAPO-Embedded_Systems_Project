#include "boton_interrupcion.h"
#include "configuracion_pines.h"
#include <Arduino.h>

// Variables volátiles para uso seguro dentro de las interrupciones
volatile bool bandera_boton_presionado_isr = false;
volatile unsigned long ultimo_tiempo_interrupcion_ms = 0;
const unsigned long TIEMPO_ANTIRREBOTE_MS = 250; 

// ISR (Interrupt Service Routine) alojada en RAM para ejecución rápida
void IRAM_ATTR isr_boton_presionado() {
    unsigned long tiempo_actual_ms = millis();
    // Lógica de Antirrebote (Debounce) por software
    if ((tiempo_actual_ms - ultimo_tiempo_interrupcion_ms) > TIEMPO_ANTIRREBOTE_MS) {
        bandera_boton_presionado_isr = true;
        ultimo_tiempo_interrupcion_ms = tiempo_actual_ms;
    }
}

uint8_t inicializar_boton(void) {
    // INPUT_PULLUP activa la resistencia interna. 
    // Por defecto lee HIGH. Al presionar cambiará a LOW.
    pinMode(PIN_BOTON_APAGADO, INPUT_PULLUP);
    
    // Configuramos la interrupción en el flanco de bajada (FALLING)
    attachInterrupt(digitalPinToInterrupt(PIN_BOTON_APAGADO), isr_boton_presionado, FALLING);
    
    return ESTADO_OK;
}

uint8_t verificar_boton_presionado(void) {
    if (bandera_boton_presionado_isr) {
        return 1; 
    }
    return 0; 
}

uint8_t limpiar_bandera_boton(void) {
    bandera_boton_presionado_isr = false;
    return ESTADO_OK;
}
