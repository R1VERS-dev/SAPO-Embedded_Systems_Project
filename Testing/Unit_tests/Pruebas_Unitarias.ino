#include <Arduino.h>
#include "../SAPO_Proyecto/almacenamiento_local.h"
#include "../SAPO_Proyecto/servidor_web.h"
#include "../SAPO_Proyecto/sensores_gas.h"
#include "../SAPO_Proyecto/actuadores_alerta.h"
#include "../SAPO_Proyecto/boton_interrupcion.h"

/**
 * UNIT TESTS - SISTEMA SAPO
 * 
 * Este archivo contiene las pruebas unitarias para 8 funciones core del sistema.
 * Para ejecutar: Cargar este sketch en el ESP32 y abrir el Monitor Serial a 115200 bps.
 */

int pruebas_totales = 0;
int pruebas_exitosas = 0;

void assert_test(bool condicion, String mensaje) {
    pruebas_totales++;
    Serial.print("[TEST " + String(pruebas_totales) + "] ");
    if (condicion) {
        Serial.println("✅ PASÓ: " + mensaje);
        pruebas_exitosas++;
    } else {
        Serial.println("❌ FALLÓ: " + mensaje);
    }
}

// --- 1. Test de inicializar_sistema_archivos ---
void test_inicializar_fs() {
    uint8_t res = inicializar_sistema_archivos();
    assert_test(res == ESTADO_OK, "inicializar_sistema_archivos() retorna OK y monta LittleFS.");
}

// --- 2. Test de borrar_historial ---
void test_borrar_datos() {
    uint8_t res = borrar_historial();
    uint32_t alarmas = obtener_contador_alarmas();
    assert_test(res == ESTADO_OK && alarmas == 0, "borrar_historial() limpia archivos y resetea contadores.");
}

// --- 3. Test de registrar_activacion_alarma ---
void test_registro_alarma() {
    uint32_t inicial = obtener_contador_alarmas();
    registrar_activacion_alarma();
    uint32_t final = obtener_contador_alarmas();
    assert_test(final == (inicial + 1), "registrar_activacion_alarma() incrementa correctamente el contador persistente.");
}

// --- 4. Test de guardar_registro_historico ---
void test_guardar_datos() {
    // Guardamos valores específicos para luego verificar
    uint8_t res = guardar_registro_historico(777, 888);
    assert_test(res == ESTADO_OK, "guardar_registro_historico() permite escribir nuevos registros de sensores.");
}

// --- 5. Test de obtener_historial_json ---
void test_generar_json() {
    String json = obtener_historial_json();
    // Verificamos que el JSON contenga los valores que acabamos de guardar
    bool contiene_valores = json.indexOf("777") > 0 && json.indexOf("888") > 0;
    assert_test(json.startsWith("[") && json.endsWith("]") && contiene_valores, 
                "obtener_historial_json() genera un formato JSON válido con los datos correctos.");
}

// --- 6. Test de sensores_gas ---
void test_inicializar_sensores() {
    uint8_t res = inicializar_sensores_gas();
    assert_test(res == ESTADO_OK, "inicializar_sensores_gas() configura los pines analógicos correctamente.");
}

// --- 7. Test de actuadores_alerta ---
void test_actuadores() {
    uint8_t res_act = activar_alerta_fisica();
    uint8_t res_des = desactivar_alerta_fisica();
    assert_test(res_act == ESTADO_OK && res_des == ESTADO_OK, "activar/desactivar_alerta_fisica() operan sobre los pines de salida.");
}

// --- 8. Test de boton_interrupcion ---
void test_boton() {
    uint8_t res = limpiar_bandera_boton();
    assert_test(res == ESTADO_OK, "limpiar_bandera_boton() restablece el estado de la interrupción.");
}

void setup() {
    Serial.begin(115200);
    while(!Serial); // Esperar a que el monitor serial esté listo
    delay(1000);

    Serial.println("\n===========================================");
    Serial.println("   CORRIENDO PRUEBAS UNITARIAS - SAPO    ");
    Serial.println("===========================================");

    // Ejecución de pruebas
    test_inicializar_fs();
    test_borrar_datos();
    test_registro_alarma();
    test_guardar_datos();
    test_generar_json();
    test_inicializar_sensores();
    test_actuadores();
    test_boton();

    Serial.println("===========================================");
    Serial.print("RESULTADO FINAL: ");
    Serial.print(pruebas_exitosas);
    Serial.print("/");
    Serial.println(pruebas_totales);
    
    if (pruebas_exitosas == pruebas_totales) {
        Serial.println("ESTADO: TODO CORRECTO ✨");
    } else {
        Serial.println("ESTADO: REVISAR FALLOS ⚠️");
    }
    Serial.println("===========================================");
}

void loop() {
    // Fin de las pruebas
}
