#ifndef ALMACENAMIENTO_LOCAL_H
#define ALMACENAMIENTO_LOCAL_H

#include <stdint.h>
#include <Arduino.h>

#define ARCHIVO_HISTORIAL "/historial.csv"
#define ARCHIVO_ALARMAS   "/alarmas.txt"

// Muestra el limite máximo de lineas para 7 dias, ej. guardando cada hora = 24 * 7 = 168
#define MAX_REGISTROS_HISTORIAL 168

// Inicializa el sistema de archivos LittleFS
uint8_t inicializar_sistema_archivos(void);

// Guarda un nuevo registro, controlando que no exceda los MAX_REGISTROS_HISTORIAL
uint8_t guardar_registro_historico(uint16_t humo, uint16_t calidad_aire);

// Lee el archivo CSV y lo transfiere como cadena JSON
String obtener_historial_json(void);

// Incrementa y guarda el contador de alarmas activadas
uint8_t registrar_activacion_alarma(void);

// Obtiene el número de alarmas activadas en total
uint32_t obtener_contador_alarmas(void);

// Borra todo el historial y contadores
uint8_t borrar_historial(void);

#endif // ALMACENAMIENTO_LOCAL_H
