#include "servidor_web.h"
#include "interfaz_usuario.h"
#include "almacenamiento_local.h"
#include "actuadores_alerta.h"
#include "configuracion_pines.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

extern bool alarma_en_cooldown;
extern unsigned long inicio_cooldown_ms;

// Instancia global del servidor asíncrono en el puerto 80 (HTTP)
AsyncWebServer servidor(80);

// Variables locales copiadas para no bloquear peticiones 
// de lectura hacia la máquina de estados principal.
static uint16_t web_humo_actual = 0;
static uint16_t web_aire_actual = 0;
static uint8_t web_estado_alarma = 0;

// Referencia externa a variables de SAPO_Proyecto.ino para apagar alarma
extern uint8_t estado_alarma_activa; 

void actualizar_datos_servidor(uint16_t humo, uint16_t aire, uint8_t alarma) {
    web_humo_actual = humo;
    web_aire_actual = aire;
    web_estado_alarma = alarma;
}

uint8_t inicializar_servidor_web(void) {
    // 1. Configurar y arrancar WiFi Modo AP
    WiFi.mode(WIFI_AP);
    if (!WiFi.softAP(AP_SSID, AP_PASSWORD)) {
        Serial.println("Falla al iniciar Punto de Acceso WiFi.");
        return ESTADO_ERROR;
    }
    
    Serial.println("===============================");
    Serial.print("WiFi AP Iniciado: ");
    Serial.println(AP_SSID);
    Serial.print("IP del Servidor: ");
    Serial.println(WiFi.softAPIP());
    Serial.println("===============================");

    // 2. Definir Rutas (Endpoints) del Servidor

    // Ruta de la UI Principal
    servidor.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", PAGINA_HTML_PRINCIPAL);
    });

    // Endpoint -> Datos en Tiempo Real (Retorna JSON)
    servidor.on("/api/datos", HTTP_GET, [](AsyncWebServerRequest *request){
        String json = "{";
        json += "\"humo\":" + String(web_humo_actual) + ",";
        json += "\"aire\":" + String(web_aire_actual) + ",";
        json += "\"alarma_activa\":" + String(web_estado_alarma) + ",";
        json += "\"alarmas\":" + String(obtener_contador_alarmas());
        json += "}";
        request->send(200, "application/json", json);
    });

    // Endpoint -> Historial para Gráficas
    servidor.on("/api/historial", HTTP_GET, [](AsyncWebServerRequest *request){
        String historico_json = obtener_historial_json();
        request->send(200, "application/json", historico_json);
    });

    // Endpoint -> Apagar Alarma mediante botón Web
    servidor.on("/api/apagar", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.println("Solicitud WEB recibida: Apagar Alarma");
        desactivar_alerta_fisica();
        
        // Modificamos la variable global de bandera general (definida en el main)
        estado_alarma_activa = 0;  
        web_estado_alarma = 0;
        
        alarma_en_cooldown = true;
        inicio_cooldown_ms = millis();

        request->send(200, "text/plain", "Alarma Apagada");
    });

    // Endpoint -> Borrar Historial Web
    servidor.on("/api/borrar_historial", HTTP_POST, [](AsyncWebServerRequest *request){
        borrar_historial();
        Serial.println("Solicitud WEB: Historial y contadores borrados.");
        request->send(200, "text/plain", "OK");
    });

    // Arrancar el Servidor
    servidor.begin();
    
    return ESTADO_OK;
}
