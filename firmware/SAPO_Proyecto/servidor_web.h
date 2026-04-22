#ifndef SERVIDOR_WEB_H
#define SERVIDOR_WEB_H

#include <stdint.h>
#include <Arduino.h>

// Definiciones de la Red (Modo AP)
#define AP_SSID       "Red SAPO"
#define AP_PASSWORD   "sapo12345" // Clave de 8+ caracteres. Puedes cambiarla.

// Inicializa el WiFi en modo AP y arranca el servidor asíncrono
uint8_t inicializar_servidor_web(void);

// Permite inyectar las variables globales del main al servidor para que este armé el JSON.
// Se llamará cada vez que se necesite proporcionar los datos de lectura actuales.
void actualizar_datos_servidor(uint16_t humo, uint16_t aire, uint8_t alarma);

#endif // SERVIDOR_WEB_H
