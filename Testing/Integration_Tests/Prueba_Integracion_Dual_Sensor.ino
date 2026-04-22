#include <Arduino.h>
#include "../SAPO_Proyecto/sensores_gas.h"
#include "../SAPO_Proyecto/almacenamiento_local.h"
#include "../SAPO_Proyecto/configuracion_pines.h"

/**
 * INTEGRATION TEST - SISTEMA SAPO
 * 
 * Flujo: Lectura Dual (Humo + Calidad Aire) -> Registro Unificado
 * 
 * Este test valida que los datos de ambos sensores se capturen simultáneamente
 * y se guarden como un único registro coherente en el historial.
 */

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n>>> TEST DE INTEGRACIÓN: LECTURA DUAL DE SENSORES <<<");

    // 1. Inicialización
    if (inicializar_sensores_gas() == ESTADO_OK && inicializar_sistema_archivos() == ESTADO_OK) {
        Serial.println("[PASO 1] Sensores y Almacenamiento listos.");
    } else {
        Serial.println("[ERROR] Fallo en la preparación del hardware.");
        return;
    }

    borrar_historial();

    // 2. Captura de datos
    Serial.println("[PASO 2] Realizando lectura sincronizada de MQ-2 y MQ-135...");
    uint16_t v_humo = leer_sensor_humo();
    uint16_t v_aire = leer_sensor_calidad_aire();
    
    Serial.print(" -> Humo detectado: "); Serial.println(v_humo);
    Serial.print(" -> Aire detectado: "); Serial.println(v_aire);

    // 3. Integración en almacenamiento
    Serial.println("[PASO 3] Guardando lectura dual en el historial...");
    guardar_registro_historico(v_humo, v_aire);

    // 4. Verificación en el JSON de salida
    Serial.println("[PASO 4] Verificando consistencia en la salida JSON...");
    String json = obtener_historial_json();

    // Verificamos que ambos valores aparezcan en la misma cadena JSON
    bool ambos_presentes = (json.indexOf(String(v_humo)) != -1) && (json.indexOf(String(v_aire)) != -1);

    if (ambos_presentes) {
        Serial.println("\n✅ TEST DE INTEGRACIÓN COMPLETADO CON ÉXITO");
        Serial.println("Los sensores están perfectamente integrados con el historial de datos.");
    } else {
        Serial.println("\n❌ TEST FALLIDO: Los datos no se guardaron correctamente o están incompletos.");
        Serial.print("JSON resultante: "); Serial.println(json);
    }
}

void loop() {
}
