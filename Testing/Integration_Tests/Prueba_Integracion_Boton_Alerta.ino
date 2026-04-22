#include <Arduino.h>
#include "../SAPO_Proyecto/boton_interrupcion.h"
#include "../SAPO_Proyecto/actuadores_alerta.h"
#include "../SAPO_Proyecto/configuracion_pines.h"

/**
 * INTEGRATION TEST - SISTEMA SAPO
 * 
 * Flujo: Interrupción por Botón -> Silenciado de Actuadores
 * 
 * Este test verifica que la pulsación del botón físico detenga correctamente
 * las alertas sonoras y visuales.
 */

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("\n>>> TEST DE INTEGRACIÓN: BOTÓN Y SILENCIADO <<<");

    // 1. Inicialización
    if (inicializar_boton() == ESTADO_OK && inicializar_actuadores() == ESTADO_OK) {
        Serial.println("[PASO 1] Hardware de entrada/salida listo.");
    } else {
        Serial.println("[ERROR] Fallo en inicialización.");
        return;
    }

    // 2. Activamos alerta para probar el silenciado
    Serial.println("[PASO 2] Activando alerta física (LED/Buzzer)...");
    activar_alerta_fisica();
    delay(500);

    // 3. Simulación de pulsación (o espera de pulsación real)
    Serial.println("[PASO 3] Esperando pulsación del botón para silenciar...");
    Serial.println("(Si estás probando con hardware real, presiona el botón ahora)");

    // Simulamos que el botón fue presionado después de 2 segundos si no se presiona físicamente
    unsigned long tiempo_inicio = millis();
    bool presionado = false;

    while (millis() - tiempo_inicio < 5000) { // 5 segundos de espera
        if (verificar_boton_presionado()) {
            presionado = true;
            break;
        }
        delay(100);
    }

    if (presionado) {
        Serial.println(" -> ¡Botón detectado!");
        
        // 4. Ejecución del silenciado
        desactivar_alerta_fisica();
        limpiar_bandera_boton();
        
        Serial.println("[PASO 4] Alerta desactivada y bandera de interrupción limpia.");
        Serial.println("\n✅ TEST DE INTEGRACIÓN COMPLETADO CON ÉXITO");
    } else {
        Serial.println("\n❌ TEST FALLIDO: No se detectó la pulsación del botón en el tiempo esperado.");
    }
}

void loop() {
    // Fin del test
}
