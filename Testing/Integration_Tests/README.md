# Reporte de Pruebas de Integración - Proyecto SAPO

En este apartado, nos enfocamos en verificar que todos los componentes que hemos desarrollado para el sistema SAPO trabajen en armonía. A diferencia de las pruebas unitarias donde revisamos funciones por separado, aquí validamos que el "camino completo" de los datos y las acciones funcione sin errores.

## Nuestras Pruebas de Integración

Hemos diseñado cinco escenarios clave para garantizar que SAPO funciona como un todo robusto:

### 1. Flujo de Alerta Crítica (`Prueba_Integracion_Alerta.ino`)
Es nuestra prueba principal. Simula una detección de gas que supera los umbrales y verifica la respuesta en cadena: identificación, activación de alarmas físicas, incremento del contador persistente y guardado en el historial.

### 2. Control y Silenciado Manual (`Prueba_Integracion_Boton_Alerta.ino`)
Aquí validamos la interacción entre nuestras interrupciones de hardware y los actuadores. Nos aseguramos de que la pulsación del botón físico detenga inmediatamente el sonido y la luz, limpiando las banderas de estado.

### 3. Sincronización y Limpieza Total (`Prueba_Integracion_Limpieza_Sistema.ino`)
Comprobamos la integridad de nuestra gestión de datos, verificando que al borrar el historial se reseteen simultáneamente la memoria flash, los contadores y la salida JSON de la web.

### 4. Estrés de Almacenamiento (`Prueba_Integracion_Estres_Almacenamiento.ino`)
Sometemos al sistema a una carga intensiva de datos para verificar que gestione correctamente el límite de registros establecido (`MAX_REGISTROS_HISTORIAL`) sin corromper la memoria flash ni colapsar el sistema.

### 5. Lectura Dual de Sensores (`Prueba_Integracion_Dual_Sensor.ino`)
Validamos que las lecturas de los sensores MQ-2 y MQ-135 se capturen de forma sincronizada y se integren correctamente en un único registro unificado dentro del historial de datos.

### Pasos para replicar nuestros resultados

Para ejecutar cualquiera de estas pruebas nosotros mismos, seguimos estos pasos:
*   Abrimos el archivo `.ino` deseado en el Arduino IDE.
*   Cargamos el código en el ESP32 (manteniendo la estructura de carpetas para que las rutas relativas a `SAPO_Proyecto` funcionen).
*   Monitoreamos la salida a través del Monitor Serial a **115200 bps**.

Al finalizar cada test, el sistema nos arroja un mensaje de éxito si todas las etapas se comunicaron correctamente, garantizando que SAPO es un sistema robusto y confiable en cualquier situación.

---
*Documentación generada por el equipo de desarrollo de Sistemas Embebidos - SAPO.*
