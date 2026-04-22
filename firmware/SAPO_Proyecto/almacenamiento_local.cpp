#include "almacenamiento_local.h"
#include "configuracion_pines.h"
#include <LittleFS.h>

uint8_t inicializar_sistema_archivos(void) {
    if (!LittleFS.begin(true)) { // 'true' formatea si falla al montar
        Serial.println("Error montando LittleFS");
        return ESTADO_ERROR;
    }
    
    // Crear archivos si no existen
    if (!LittleFS.exists(ARCHIVO_HISTORIAL)) {
        File file = LittleFS.open(ARCHIVO_HISTORIAL, "w");
        if (file) {
            file.println("timestamp_ms,humo,aire"); // Cabecera
            file.close();
        }
    }
    if (!LittleFS.exists(ARCHIVO_ALARMAS)) {
        File file = LittleFS.open(ARCHIVO_ALARMAS, "w");
        if (file) {
            file.print("0");
            file.close();
        }
    }
    return ESTADO_OK;
}

uint8_t guardar_registro_historico(uint16_t humo, uint16_t calidad_aire) {
    // Si queremos un registro rotativo (borrar lo viejo si nos pasamos de lineas),
    // la opción mas ligera es leer a memoria y reescribir, o usar 2 archivos.
    // Para simplificar y cuidar RAM, contaremos líneas.
    File file = LittleFS.open(ARCHIVO_HISTORIAL, "r");
    if (!file) return ESTADO_ERROR;
    
    String contenido = "timestamp_ms,humo,aire\n";
    uint16_t lineas = 0;
    
    // Saltar la primera linea (cabecera)
    if(file.available()) file.readStringUntil('\n');

    // Leer el resto del archivo
    while (file.available()) {
        String linea = file.readStringUntil('\n');
        if (linea.length() > 0) {
            contenido += linea + "\n";
            lineas++;
        }
    }
    file.close();

    // Si llegamos al límite de líneas, borramos la primera (simulando cola FIFO de forma muy básica).
    // NOTA: Para producción real esto se hace con ring buffer binario.
    // Como son pocas lecturas, para mantenerlo manejable por ahora:
    if (lineas >= MAX_REGISTROS_HISTORIAL) {
        int idx = contenido.indexOf('\n') + 1; // Salto cabecera
        int idx2 = contenido.indexOf('\n', idx) + 1; // Salto primer dato
        String nueva_cola = "timestamp_ms,humo,aire\n" + contenido.substring(idx2);
        contenido = nueva_cola;
    }

    // Agregar nueva línea
    contenido += String(millis()) + "," + String(humo) + "," + String(calidad_aire) + "\n";

    // Sobrescribir
    file = LittleFS.open(ARCHIVO_HISTORIAL, "w");
    if (file) {
        file.print(contenido);
        file.close();
        return ESTADO_OK;
    }
    return ESTADO_ERROR;
}

String obtener_historial_json(void) {
    File file = LittleFS.open(ARCHIVO_HISTORIAL, "r");
    if (!file) return "[]";
    
    String json = "[";
    if(file.available()) file.readStringUntil('\n'); // Descarta cabecera CSV

    bool primera = true;
    while (file.available()) {
        String linea = file.readStringUntil('\n');
        if (linea.length() > 0) {
            int coma1 = linea.indexOf(',');
            int coma2 = linea.indexOf(',', coma1 + 1);
            if (coma1 > 0 && coma2 > 0) {
                String t = linea.substring(0, coma1);
                String h = linea.substring(coma1 + 1, coma2);
                String a = linea.substring(coma2 + 1);
                
                if (!primera) json += ",";
                json += "{\"time\":" + t + ",\"humo\":" + h + ",\"aire\":" + a + "}";
                primera = false;
            }
        }
    }
    file.close();
    json += "]";
    return json;
}

uint8_t registrar_activacion_alarma(void) {
    uint32_t alarmas = obtener_contador_alarmas();
    alarmas++;
    
    File file = LittleFS.open(ARCHIVO_ALARMAS, "w");
    if (file) {
        file.print(alarmas);
        file.close();
        return ESTADO_OK;
    }
    return ESTADO_ERROR;
}

uint32_t obtener_contador_alarmas(void) {
    File file = LittleFS.open(ARCHIVO_ALARMAS, "r");
    if (!file) return 0;
    
    String val = file.readStringUntil('\n');
    file.close();
    return val.toInt();
}

uint8_t borrar_historial(void) {
    File file1 = LittleFS.open(ARCHIVO_HISTORIAL, "w");
    if (file1) {
        file1.println("timestamp_ms,humo,aire");
        file1.close();
    }
    
    File file2 = LittleFS.open(ARCHIVO_ALARMAS, "w");
    if (file2) {
        file2.print("0");
        file2.close();
    }
    return ESTADO_OK;
}
