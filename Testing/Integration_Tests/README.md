# Reporte de Pruebas de Integración - Proyecto SAPO

En este apartado, nos enfocamos en verificar que todos los componentes que hemos desarrollado para el sistema SAPO trabajen en armonía. A diferencia de las pruebas unitarias donde revisamos funciones por separado, aquí validamos que el "camino completo" de los datos y las acciones funcione sin errores.

## Nuestra Prueba de Integración: Flujo de Alerta Crítica

Hemos implementado el test principal en el archivo `Prueba_Integracion_Alerta.ino`. Este archivo simula una situación real de riesgo para observar cómo responden los módulos de sensores, actuadores y almacenamiento en conjunto.

### ¿Cómo funciona este test?

Diseñamos esta prueba para recorrer cuatro etapas fundamentales de nuestra arquitectura:

1.  **Detección y Lógica:** Simulamos una entrada de gas que supera los umbrales de seguridad que definimos en `configuracion_pines.h`. Aquí comprobamos que el módulo de sensores identifique correctamente la condición de alerta.
2.  **Respuesta Física:** Verificamos que, al detectarse el gas, el sistema dé la orden inmediata de encender el LED de advertencia y el Buzzer, asegurando que los actuadores respondan en tiempo real.
3.  **Persistencia en Memoria:** Validamos que el evento no se pierda. El sistema debe incrementar automáticamente nuestro contador de alarmas persistente en la flash del ESP32.
4.  **Generación de Historial:** Finalmente, nos aseguramos de que el valor detectado se guarde en el archivo CSV y pueda ser recuperado en formato JSON. Si el valor aparece en el JSON final, confirmamos que la integración entre el almacenamiento y la interfaz web es correcta.

### Pasos para replicar nuestros resultados

Para ejecutar esta prueba nosotros mismos, seguimos estos pasos:
*   Abrimos el archivo `Prueba_Integracion_Alerta.ino` en el Arduino IDE.
*   Cargamos el código en el ESP32 (manteniendo la estructura de carpetas para que las rutas relativas a `SAPO_Proyecto` funcionen).
*   Monitoreamos la salida a través del Monitor Serial a **115200 bps**.

Al finalizar, el sistema nos arroja un mensaje de éxito si todas las etapas se comunicaron correctamente, garantizando que SAPO es un sistema robusto y confiable.

---
*Documentación generada por el equipo de desarrollo de Sistemas Embebidos - SAPO.*
