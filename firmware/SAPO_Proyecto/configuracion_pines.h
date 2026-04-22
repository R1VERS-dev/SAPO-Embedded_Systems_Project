#ifndef CONFIGURACION_PINES_H
#define CONFIGURACION_PINES_H

// Pines de Sensores (Entradas Analógicas)
#define PIN_SENSOR_MQ2     34
#define PIN_SENSOR_MQ135   35

// Pines de Actuadores (Salidas Digitales)
#define PIN_LED_ALERTA     25
#define PIN_BUZZER_ALERTA  26

// Pines de Entrada (Botón)
#define PIN_BOTON_APAGADO  27

// Umbrales de Alerta (Ajustar según calibración en sitio)
// El ADC de la ESP32 es de 12 bits (0 a 4095)
#define UMBRAL_MQ2_ALERTA   2000 
#define UMBRAL_MQ135_ALERTA 2000 

// Códigos de retorno MISRA
#define ESTADO_OK          0
#define ESTADO_ERROR       1

#endif // CONFIGURACION_PINES_H
