#include <Arduino.h>
#include "../SAPO_Proyecto/almacenamiento_local.h"
#include "../SAPO_Proyecto/servidor_web.h"
#include "../SAPO_Proyecto/configuracion_pines.h"

/**
 * INTEGRATION TEST - SISTEMA SAPO
 * 
 * Flujo: Comando de Limpieza -> Consistencia de Almacenamiento y Web
 * 
 * Este test asegura que al borrar el historial, tanto los archivos internos
 * como la salida JSON del servidor web se limpien simultáneamente.
 */

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n>>> TEST DE INTEGRACIÓN: LIMPIEZA TOTAL DEL SISTEMA <<<");

    // 1. Inicialización y carga de datos basura para probar
    inicializar_sistema_archivos();
    Serial.println("[PASO 1] Generando datos de prueba...");
    for(int i=0; i<5; i++) {
        guardar_registro_historico(100 + i, 200 + i);
        registrar_activacion_alarma();
    }

    // 2. Verificación de que hay datos
    uint32_t alarmas_antes = obtener_contador_alarmas();
    String json_antes = obtener_historial_json();
    Serial.print(" -> Estado inicial: ");
    Serial.print(alarmas_antes);
    Serial.println(" alarmas registradas.");

    // 3. Ejecución de la limpieza
    Serial.println("[PASO 2] Ejecutando comando de limpieza (borrar_historial)...");
    uint8_t res = borrar_historial();

    // 4. Verificación de consistencia post-limpieza
    Serial.println("[PASO 3] Verificando que todo esté en cero...");
    uint32_t alarmas_despues = obtener_contador_alarmas();
    String json_despues = obtener_historial_json();

    bool exito = true;

    if (res != ESTADO_OK) {
        Serial.println("❌ ERROR: La función de borrado retornó error.");
        exito = false;
    }

    if (alarmas_despues != 0) {
        Serial.println("❌ ERROR: El contador de alarmas no se reseteó.");
        exito = false;
    }

    if (json_despues != "[]") {
        Serial.println("❌ ERROR: El historial JSON no está vacío.");
        Serial.print("Recibido: ");
        Serial.println(json_despues);
        exito = false;
    }

    if (exito) {
        Serial.println("\n✅ TEST DE INTEGRACIÓN COMPLETADO CON ÉXITO");
        Serial.println("La limpieza de archivos y la salida web están perfectamente sincronizadas.");
    } else {
        Serial.println("\n❌ TEST DE INTEGRACIÓN FALLIDO");
    }
}

void loop() {
    // Fin del test
}
