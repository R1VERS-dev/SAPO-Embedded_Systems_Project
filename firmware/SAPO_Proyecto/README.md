# Proyecto SAPO (Sistema de Alerta de Partículas y Oxígeno)

Este proyecto es un sistema embebido basado en ESP32 configurado como Servidor Web Local y Punto de Acceso (Access Point). Está diseñado para detectar humo y medir la calidad del aire, alertar ante condiciones de peligro y almacenar un historial de métricas directamente en la memoria del microcontrolador.

## 📁 Arquitectura del Código (Módulos)

El código ha sido altamente modularizado siguiendo el estándar MISRA, lo cual significa que está dividido en pequeños archivos enfocados en una sola responsabilidad, sin usar retrasos (`delay()`) bloqueantes:

- **`SAPO_Proyecto.ino`**: Es el archivo principal (Main Loop). Aquí se orquestan y juntan todos los módulos. Lee los sensores periódicamente usando `millis()`, verifica estados de alarma y guarda datos en el historial.
- **`configuracion_pines.h`**: El diccionario del proyecto. Aquí se definen en qué pin físico de la tarjeta va conectado cada cable (Sensores, Buzzer, LED) y los umbrales de disparo.
- **`sensores_gas.h` / `.cpp`**: Administra los lectores analógicos. Configura la resolución de la ESP32 (a 12 bits) e implementa un "filtro de paso bajo" (haciendo promediado) para devolver lecturas limpiecitas sin ruido electromagnético.
- **`actuadores_alerta.h` / `.cpp`**: Contiene la lógica sencilla para encender o apagar el zumbador (Buzzer) y las luces de emergencia (LED).
- **`boton_interrupcion.h` / `.cpp`**: Escucha el Botón (Pushbutton) de apagado. Lo hace utilizando **Interrupciones de Hardware** (ISR) y un control de rebote (Debounce) en el código, lo que significa que la ESP32 detectará el botonazo de inmediato sin importar lo que esté procesando.
- **`almacenamiento_local.h` / `.cpp`**: Utiliza **LittleFS** para administrar archivos de texto adentro de la ESP32 (`.csv`). Registra los datos de los 7 días; si se llena la cuota de líneas predefinidas, borra la cola original empujando los datos en formato de anillo FIFO (First In, First Out).
- **`servidor_web.h` / `.cpp`**: Levanta la red Wi-Fi ("Red SAPO") y atiende peticiones. Otorga los datos a la página del celular a través de respuestas en formato JSON (`/api/datos`), por lo que la interfaz gráfica nunca necesita recargar la página.
- **`interfaz_usuario.h`**: Almacena de lado del servidor (en la memoria PROGMEM para no saturar RAM) todo el código gráfico `HTML`, `CSS` (Glassmorphism moderno) y `JavaScript` que dibuja la página y las gráficas en los navegadores web.

---

## 💨 Sensores Utilizados: Mediciones y Rangos

El microcontrolador ESP32 posee un ADC (Convertidor Analógico a Digital) de **12 Bits**.
Esto significa que todas las lecturas físicas de los componentes se mapearán en un rango matemático que va desde **`0` hasta `4095`**. 

> **Nota:** Un valor de `0` significa cero gas detectado (aire totalmente puro al teórico o sensor apagado), y `4095` significa saturación total de gas (peligro inminente o fallo de conexión).

### 1. Sensor MQ-2 (Humo y Gases Combustibles) 🚬
El MQ-2 es sensible a las partículas físicas en el ambiente y a gases inflamables volátiles.
*   **¿Qué mide principalmente?**: Humo, Gas LP (Metano, Propano, Butano), Hidrógeno y Alcohol.
*   **Comportamiento del Rango**:
    *   **200 - 800 (ADC)**: Aire típicamente normal de un hogar/oficina con un nivel ínfimo de ruido. Depende de la temperatura ambiente y de si el sensor ya pre-calentó internamente.
    *   **1500 - 2000 (ADC)**: Posible presencia de humo esporádico (ej: Se quemó pan en la tostadora o alguien fuma cerca).
    *   **2000 - 4000+ (ADC)**: Humo muy denso o acumulación peligrosa de gas. (El **umbral de alerta** del proyecto está colocado en `2000`).

### 2. Sensor MQ-135 (Calidad de Aire) 🏭
Especializado en detectar gases nocivos invisibles comunes que afectan a la salud.
*   **¿Qué mide principalmente?**: Gases de la contaminación general como Amoníaco (NH3), Óxidos de nitrógeno (NOx), Benceno, Monóxido de Carbono (CO) y Dióxido de Carbono (CO2).
*   **Comportamiento del Rango**:
    *   **100 - 600 (ADC)**: Calidad de aire excelente / buena. Habitación ventilada.
    *   **800 - 1500 (ADC)**: Aire enrarecido / viciado (Mucho CO2 acumulado por respiración humana en cuartos cerrados y sin ventilación).
    *   **2000 - 4000+ (ADC)**: Aire altamente contaminado, nocivo para la salud o presencia de vapores químicos fuertes. (El **umbral de alerta** del proyecto está en `2000`).

*(Es importante recordar que los sensores MQ no arrojan valores directos en "PPM - Partes Por Millón" de fábrica, sino valores de caída de voltaje/ADC, por lo que el algoritmo usa esos valores relativos para predecir concentraciones sospechosas).*
