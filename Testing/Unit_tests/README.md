# Reporte de Pruebas Unitarias - Proyecto SAPO

Este directorio contiene las pruebas unitarias diseñadas para verificar la integridad de las funciones críticas del sistema de monitoreo SAPO.

## Funciones Seleccionadas para el Test

Se seleccionaron **8 funciones** críticas de diferentes módulos para garantizar el correcto funcionamiento del hardware y software.

| Función | Descripción de la Prueba | Archivo de Origen |
| :--- | :--- | :--- |
| `inicializar_sistema_archivos()` | Verifica que LittleFS se monte correctamente. | `almacenamiento_local.cpp` |
| `borrar_historial()` | Asegura que los archivos se vacíen y los contadores vuelvan a cero. | `almacenamiento_local.cpp` |
| `registrar_activacion_alarma()` | Valida que el contador de alarmas se incremente persistencialmente. | `almacenamiento_local.cpp` |
| `guardar_registro_historico()` | Prueba la escritura de registros de sensores en el archivo CSV. | `almacenamiento_local.cpp` |
| `obtener_historial_json()` | Comprueba la generación de JSON válido para la web. | `almacenamiento_local.cpp` |
| `inicializar_sensores_gas()` | Verifica la configuración de pines analógicos para MQ-2 y MQ-135. | `sensores_gas.cpp` |
| `activar_alerta_fisica()` | Comprueba el encendido coordinado de LED y Buzzer. | `actuadores_alerta.cpp` |
| `limpiar_bandera_boton()` | Valida el reset de la bandera de interrupción por hardware. | `boton_interrupcion.cpp` |

## Detalles de la Implementación

### Archivo: `Pruebas_Unitarias.ino`
Es un sketch de Arduino que actúa como un **Test Runner**. Al cargarlo en el ESP32:
1. Inicializa la comunicación serial.
2. Ejecuta cada función de prueba de forma secuencial.
3. Utiliza una función `assert_test` para comparar el resultado obtenido contra el resultado esperado.
4. Imprime en el monitor serial un reporte detallado con iconos (✅ para éxito, ❌ para fallo).

### Metodología de Pruebas
Se aplicó un enfoque de **Black Box Testing** para las funciones de almacenamiento, donde inyectamos datos (como valores de humo/aire) y verificamos que el estado resultante del sistema (el contenido de la flash) sea el correcto.

---
*Desarrollado para el proyecto de Sistemas Embebidos - SAPO.*
