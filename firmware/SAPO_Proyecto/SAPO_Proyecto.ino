#include <Arduino.h>

#include "configuracion_pines.h"
#include "actuadores_alerta.h"
#include "boton_interrupcion.h"
#include "sensores_gas.h"
#include "almacenamiento_local.h"
#include "servidor_web.h"

// Variables de estado del sistema inicializadas
uint8_t estado_alarma_activa = 0; 
unsigned long ultimo_tiempo_lectura_sensores_ms = 0;
unsigned long ultimo_tiempo_guardado_historial_ms = 0;
bool alarma_en_cooldown = false;
unsigned long inicio_cooldown_ms = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("=========================================");
    Serial.println("Iniciando Sistema SAPO - Esp32 AP MODO");
    Serial.println("=========================================");

    // Inicialización de submódulos
    if (inicializar_actuadores() == ESTADO_OK) {
        Serial.println("Actuadores inicializados correctamente.");
    }
    
    if (inicializar_boton() == ESTADO_OK) {
        Serial.println("Boton e interrupciones inicializadas.");
    }

    if (inicializar_sensores_gas() == ESTADO_OK) {
        Serial.println("ADC de sensores configurado.");
    }

    if (inicializar_sistema_archivos() == ESTADO_OK) {
        Serial.println("Sistema de Archivos LittleFS montado.");
    }

    if(inicializar_servidor_web() == ESTADO_OK){
        Serial.println("Servidor Web Asíncrono Listo.");
    }
}

void loop() {
    unsigned long tiempo_actual_ms = millis();
    
    // 1. Lectura periódica de sensores (ej. cada 1 segundo) sin usar delay()
    if ((tiempo_actual_ms - ultimo_tiempo_lectura_sensores_ms) >= 1000) {
        ultimo_tiempo_lectura_sensores_ms = tiempo_actual_ms;
        
        uint16_t nivel_humo = leer_sensor_humo();
        uint16_t nivel_aire = leer_sensor_calidad_aire();
        
        Serial.print("Humo (MQ2): ");
        Serial.print(nivel_humo);
        // Compartir datos al modulo Web
        actualizar_datos_servidor(nivel_humo, nivel_aire, estado_alarma_activa);
        
        // Manejar el término del cooldown
        if (alarma_en_cooldown && ((tiempo_actual_ms - inicio_cooldown_ms) >= 30000)) {
            alarma_en_cooldown = false;
            Serial.println("Cooldown de 30s terminado. Alarma lista de nuevo.");
        }

        // Determinar condición de alerta
        if (nivel_humo >= UMBRAL_MQ2_ALERTA || nivel_aire >= UMBRAL_MQ135_ALERTA) {
            if (estado_alarma_activa == 0 && !alarma_en_cooldown) {
                Serial.println("¡ALERTA DETECTADA! Activando sistema de emergencia.");
                activar_alerta_fisica();
                registrar_activacion_alarma(); // Guadamos la estadística local.
                estado_alarma_activa = 1;
                actualizar_datos_servidor(nivel_humo, nivel_aire, estado_alarma_activa);
            }
        }
    }

    // 2. Guardado Rotativo en Histórico
    // En producción podrías querer guardarlo cada 1 hora.
    // Para propositos de demostración/evaluación, guardaremos cada 1 minuto (60000 ms)
    if ((tiempo_actual_ms - ultimo_tiempo_guardado_historial_ms) >= 60000) {
        ultimo_tiempo_guardado_historial_ms = tiempo_actual_ms;
        uint16_t h = leer_sensor_humo();
        uint16_t a = leer_sensor_calidad_aire();
        guardar_registro_historico(h, a);
        Serial.println("Registro histórico guardado en Flash.");
    }

    // 3. Verificar interrupciones del botón físico para apagar alarma
    if (verificar_boton_presionado() == 1) {
        Serial.println("Botón físico presionado. Apagando alarma...");
        desactivar_alerta_fisica();
        estado_alarma_activa = 0;
        
        // Iniciar Cooldown
        alarma_en_cooldown = true;
        inicio_cooldown_ms = millis();
        // Propagamos evento a web
        actualizar_datos_servidor(leer_sensor_humo(), leer_sensor_calidad_aire(), 0);

        // Restaurar estado de la bandera ISR
        limpiar_bandera_boton();
    }
}
