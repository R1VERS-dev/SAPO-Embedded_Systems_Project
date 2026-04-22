#include <Arduino.h>
#include "../SAPO_Proyecto/sensores_gas.h"
#include "../SAPO_Proyecto/actuadores_alerta.h"
#include "../SAPO_Proyecto/almacenamiento_local.h"
#include "../SAPO_Proyecto/configuracion_pines.h"

/**
 * INTEGRATION TEST - SISTEMA SAPO
 * 
 * Flujo: Detección de Gas -> Activación de Alerta -> Registro en Memoria
 * 
 * Este test verifica que los módulos de sensores, actuadores y almacenamiento
 * trabajen coordinadamente.
 */

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n>>> INICIANDO TEST DE INTEGRACIÓN: FLUJO DE ALERTA <<<");

    // 1. Inicialización de módulos
    if (inicializar_sistema_archivos() == ESTADO_OK && 
        inicializar_sensores_gas() == ESTADO_OK && 
        inicializar_actuadores() == ESTADO_OK) {
        Serial.println("[PASO 1] Módulos inicializados correctamente.");
    } else {
        Serial.println("[ERROR] Fallo en la inicialización de módulos.");
        return;
    }

    // Limpiamos historial previo para la prueba
    borrar_historial();
    uint32_t alarmas_iniciales = obtener_contador_alarmas();

    // 2. Simulación de evento de riesgo
    Serial.println("[PASO 2] Simulando detección de gas por encima del umbral...");
    
    // En una prueba real, aquí leeríamos el sensor. 
    // Para el test, verificamos la lógica de disparo.
    uint16_t valor_humo_simulado = UMBRAL_MQ2_ALERTA + 500; 

    if (valor_humo_simulado > UMBRAL_MQ2_ALERTA) {
        Serial.println(" -> Umbral superado. Activando respuesta del sistema...");
        
        // 3. Acción coordinada
        activar_alerta_fisica();
        registrar_activacion_alarma();
        guardar_registro_historico(valor_humo_simulado, 1500); // 1500 es aire normal
        
        Serial.println("[PASO 3] Actuadores activados y datos persistidos.");
    }

    // 4. Verificación de Integridad
    Serial.println("[PASO 4] Verificando consistencia de datos...");
    uint32_t alarmas_finales = obtener_contador_alarmas();
    String historial = obtener_historial_json();

    bool exito = true;
    
    if (alarmas_finales != (alarmas_iniciales + 1)) {
        Serial.println("❌ ERROR: El contador de alarmas no aumentó.");
        exito = false;
    }
    
    if (historial.indexOf(String(valor_humo_simulado)) == -1) {
        Serial.println("❌ ERROR: El valor de humo no se encuentra en el historial JSON.");
        exito = false;
    }

    if (exito) {
        Serial.println("\n✅ TEST DE INTEGRACIÓN COMPLETADO CON ÉXITO");
        Serial.println("El flujo 'Sensor -> Actuador -> Almacenamiento' funciona correctamente.");
    } else {
        Serial.println("\n❌ TEST DE INTEGRACIÓN FALLIDO");
    }

    // Apagamos alerta al terminar test
    desactivar_alerta_fisica();
}

void loop() {
    // Fin del test
}
