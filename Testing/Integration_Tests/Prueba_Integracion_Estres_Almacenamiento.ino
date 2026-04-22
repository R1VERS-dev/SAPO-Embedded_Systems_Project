#include <Arduino.h>
#include "../SAPO_Proyecto/almacenamiento_local.h"
#include "../SAPO_Proyecto/configuracion_pines.h"

/**
 * INTEGRATION TEST - SISTEMA SAPO
 * 
 * Flujo: Llenado de Historial -> Límite de Almacenamiento
 * 
 * Este test verifica que el sistema gestione correctamente el límite de 
 * registros (MAX_REGISTROS_HISTORIAL) sin colapsar la memoria flash.
 */

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n>>> TEST DE INTEGRACIÓN: ESTRÉS DE ALMACENAMIENTO <<<");

    // 1. Inicialización
    inicializar_sistema_archivos();
    borrar_historial();
    Serial.print("[PASO 1] Iniciando llenado intensivo. Límite: ");
    Serial.println(MAX_REGISTROS_HISTORIAL);

    // 2. Intentamos guardar más registros del límite permitido
    Serial.println("[PASO 2] Escribiendo registros...");
    int registros_a_intentar = MAX_REGISTROS_HISTORIAL + 10;
    int exitos = 0;

    for (int i = 0; i < registros_a_intentar; i++) {
        if (guardar_registro_historico(i, i) == ESTADO_OK) {
            exitos++;
        }
        if (i % 50 == 0) Serial.print("."); 
    }
    Serial.println("\nEstructura de archivos procesada.");

    // 3. Verificación de integridad
    Serial.println("[PASO 3] Verificando que el sistema se mantuvo estable...");
    String json = obtener_historial_json();
    
    // Contamos cuántos registros hay realmente en el JSON (contando las comas o llaves)
    int conteo_real = 0;
    int index = 0;
    while ((index = json.indexOf("{", index)) != -1) {
        conteo_real++;
        index++;
    }

    Serial.print(" -> Registros intentados: "); Serial.println(registros_a_intentar);
    Serial.print(" -> Registros en memoria: "); Serial.println(conteo_real);

    if (conteo_real <= MAX_REGISTROS_HISTORIAL) {
        Serial.println("\n✅ TEST DE INTEGRACIÓN COMPLETADO CON ÉXITO");
        Serial.println("El sistema gestionó el desbordamiento de datos correctamente.");
    } else {
        Serial.println("\n❌ TEST FALLIDO: El sistema excedió el límite de seguridad.");
    }
}

void loop() {
}
