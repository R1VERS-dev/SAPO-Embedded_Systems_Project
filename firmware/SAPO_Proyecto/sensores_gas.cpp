#include "sensores_gas.h"
#include "configuracion_pines.h"
#include <Arduino.h>

uint8_t inicializar_sensores_gas(void) {
    // Configura la resolución del ADC del ESP32 (12 bits: 0 a 4095)
    analogReadResolution(12);
    
    // Opcional: Atenuación por defecto de 11db permite medir hasta ~3.3V
    analogSetAttenuation(ADC_11db);
    
    return ESTADO_OK;
}

uint16_t leer_sensor_humo(void) {
    // Para mitigar ruido (filtro simple), tomamos un promedio
    uint32_t suma_lecturas = 0;
    uint8_t iteraciones = 5;
    
    for(uint8_t i = 0; i < iteraciones; i++) {
        suma_lecturas += analogRead(PIN_SENSOR_MQ2);
        // Pequeña pausa de 2ms para estabilizar lectura del ADC (no afecta la asincronía general del loop)
        delay(2); 
    }
    
    return (uint16_t)(suma_lecturas / iteraciones);
}

uint16_t leer_sensor_calidad_aire(void) {
    uint32_t suma_lecturas = 0;
    uint8_t iteraciones = 5;
    
    for(uint8_t i = 0; i < iteraciones; i++) {
        suma_lecturas += analogRead(PIN_SENSOR_MQ135);
        delay(2);
    }
    
    return (uint16_t)(suma_lecturas / iteraciones);
}
