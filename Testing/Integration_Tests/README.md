# Reporte de Pruebas de Integración - Proyecto SAPO

En este apartado, nos enfocamos en verificar que todos los componentes que hemos desarrollado para el sistema SAPO trabajen en armonía. A diferencia de las pruebas unitarias donde revisamos funciones por separado, aquí validamos que el "camino completo" de los datos y las acciones funcione sin errores.

## Nuestras Pruebas de Integración

Hemos diseñado tres escenarios principales para validar que SAPO funciona como un todo robusto:

### 1. Flujo de Alerta Crítica (`Prueba_Integracion_Alerta.ino`)
Es nuestra prueba más importante. Simula una detección de gas que supera los umbrales de seguridad y verifica la respuesta en cadena: identificación del riesgo, activación de alarmas físicas (LED/Buzzer), incremento del contador persistente y guardado en el historial CSV/JSON.

### 2. Control y Silenciado Manual (`Prueba_Integracion_Boton_Alerta.ino`)
Aquí validamos la interacción entre nuestras interrupciones de hardware y los actuadores. Nos aseguramos de que, ante una alerta activa, la pulsación del botón físico detenga inmediatamente el sonido y la luz, limpiando correctamente las banderas de estado del sistema.

### 3. Sincronización y Limpieza Total (`Prueba_Integracion_Limpieza_Sistema.ino`)
En este test comprobamos la integridad de nuestra gestión de datos. Verificamos que al ejecutar un comando de limpieza, se borren los archivos físicos de la flash, se reseteen los contadores de alarmas y, lo más importante, que la interfaz web (JSON) refleje inmediatamente que el sistema está vacío.

### Pasos para replicar nuestros resultados

Para ejecutar cualquiera de estas pruebas nosotros mismos, seguimos estos pasos:
*   Abrimos el archivo `.ino` deseado en el Arduino IDE.
*   Cargamos el código en el ESP32 (manteniendo la estructura de carpetas para que las rutas relativas a `SAPO_Proyecto` funcionen).
*   Monitoreamos la salida a través del Monitor Serial a **115200 bps**.

Al finalizar cada test, el sistema nos arroja un mensaje de éxito si todas las etapas se comunicaron correctamente, garantizando que SAPO es un sistema robusto y confiable en cualquier situación.

---
*Documentación generada por el equipo de desarrollo de Sistemas Embebidos - SAPO.*
