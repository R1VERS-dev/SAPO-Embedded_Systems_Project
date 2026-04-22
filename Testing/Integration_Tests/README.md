# Pruebas de Integración - Proyecto SAPO

Este directorio contiene las pruebas diseñadas para validar que los diferentes módulos del sistema (Sensores, Actuadores, Almacenamiento y Servidor Web) interactúan correctamente entre sí.

## Diferencia con Pruebas Unitarias
Mientras que las pruebas unitarias validan funciones individuales en aislamiento, estas pruebas de integración validan **flujos de trabajo completos**.

## Pruebas Incluidas

### 1. Flujo de Alerta Completo (`Prueba_Integracion_Alerta.ino`)
Valida la cadena de eventos más crítica del sistema:
1.  **Detección:** Lectura de niveles de gas que superan el umbral configurado.
2.  **Respuesta:** Activación inmediata de los actuadores físicos (LED y Buzzer).
3.  **Persistencia:** Registro del evento en la memoria flash mediante el contador de alarmas.
4.  **Historial:** Almacenamiento de los niveles detectados en el archivo CSV para su posterior consulta.

## Cómo ejecutar las pruebas
1.  Abre el archivo `.ino` correspondiente en el Arduino IDE.
2.  Asegúrate de que la carpeta `SAPO_Proyecto` esté en el mismo nivel de directorio (las rutas son relativas `../SAPO_Proyecto/`).
3.  Carga el sketch en tu ESP32.
4.  Abre el Monitor Serial (115200 baudios) para observar los resultados de cada paso del flujo.

---
*Desarrollado para el proyecto de Sistemas Embebidos - SAPO.*
