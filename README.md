# 🚗 Seat Heater Control System
### Real-Time Embedded System using FreeRTOS

This project implements a dual-seat heater control system for a vehicle using **FreeRTOS** on a **Tiva C** microcontroller. Each seat (driver & passenger) has independent temperature monitoring and heater control with real-time task scheduling, diagnostics, and runtime measurements.

The system simulates real automotive seat heating logic using sensors, LEDs, buttons, and UART output.

---

## 🧠 System Overview
Each seat includes:
*  **Temperature Sensor:** (LM35 or simulated via potentiometer + ADC)
*  **Heater Element:** (Simulated using LEDs for intensity levels)
*  **User Control Button:** (Cycles through heating levels)
*  **Error LED:** (Sensor failure indication)

**Shared Resources:**
* A shared **UART display** outputs system status for both seats.
* The driver seat features an additional **steering-wheel control button**.

---

## 🎚 Heating Levels
| Level | Target Temperature | Description |
| :--- | :--- | :--- |
| **OFF** | — | Heater disabled |
| **LOW** | 25 °C | Gentle heating |
| **MEDIUM** | 30 °C | Standard comfort |
| **HIGH** | 35 °C | Rapid warming |

> **Note:** The controller maintains temperature within **±2 °C** of the selected level.

---

##  Heater Intensity Logic
Heating intensity is determined by the gap between the current temperature and the target:

| Temperature Difference (Target − Current) | Heater State | LED Color |
| :--- | :--- | :--- |
| $\geq 10$ °C below target | High intensity | **Cyan** |
| $5–10$ °C below target | Medium intensity | **Blue** |
| $2–5$ °C below target | Low intensity | **Green** |
| Above target | Heater OFF | — |

*If temperature drops 2 °C below target, heating automatically resumes.*

---

## ⚠ Sensor Diagnostics
**Valid temperature range:** 5 °C – 40 °C

If the sensor reading goes outside this range:
1.  Heater is **disabled** immediately.
2.   **Red LED** turns ON.
3.  Failure is logged with a **timestamp**.
4.  Control resumes automatically when readings return to normal.

*All diagnostic events are stored in RAM (and optionally in non-volatile memory).*

---

## 🧵 FreeRTOS Task Architecture
The system is built using multiple RTOS tasks with proper synchronization and event handling.

### 🧩 Implemented Tasks
* **`vCheckSeatTemp`**: Reads ADC temperature, validates sensor range.
* **`vHeaterSetLevel`**: Handles user input and cycles heating levels.
* **`vControlHeater`**: Applies heater intensity based on temperature difference.
* **`vDisplayStatus`**: Sends temperature, level, and heater state via UART.
* **`vRunTimeMeasurementsTask`**: Measures execution time, CPU load, and resource lock time.

> *Note: Tasks are instantiated per seat where required (Driver & Passenger).*

### 🔔 Interrupt Handlers
| Handler | Function |
| :--- | :--- |
| **`ExternalButton_Handler`** | Handles steering-wheel heater button (driver seat) |
| **`BuiltInButtons_Handler`** | Handles console buttons for both seats |

*Buttons use edge-triggered interrupts to ensure fast response with minimal CPU load.*

---

## 🔒 RTOS Synchronization
*  **Mutexes** protect shared UART and log memory.
*  **Semaphores/Event Flags** trigger heater control and display updates.
*  **Safe data sharing** between tasks ensures no data loss.
*  Designed for high responsiveness and low CPU usage.

---

## FreeRTOS Task Details

### vRunTimeMeasurementsTask

- **Type:** Periodic  
- **Periodicity:** 1000 ms  
- **Description:**  
  Collects and displays runtime statistics for all tasks and overall CPU load.  
- **Events / Semaphores:**  
  Does not set or wait for any.

---

### vCheckSeatTemp  
*(Created twice: one for Passenger Seat and one for Driver Seat. Each task receives a pointer to its respective seat structure as a parameter.)*

- **Type:** Periodic  
- **Periodicity:** 500 ms  
- **Description:**  
  Reads the seat temperature from the ADC, computes the heater state requirements, and updates shared seat data.  
- **Waits For:**  
  - `xADC0Mutex` — Mutex protecting ADC access  
- **Sets:**  
  - Corresponding seat’s **Heater Semaphore** (to trigger heater control)  
  - Corresponding seat’s **Display Semaphore** (to update the display)

---

### vDisplayStatus  
*(Created twice: one for Passenger Seat and one for Driver Seat. Each task receives a pointer to its respective seat structure as a parameter.)*

- **Type:** Event-Based  
- **Trigger:**  
  Unblocked by the corresponding seat’s **Display Semaphore** (given by `vCheckSeatTemp`)  
- **Description:**  
  Displays seat status over UART including:
  - Current temperature  
  - Desired temperature  
  - Heater state  
- **Waits For:**  
  - Display Semaphore  
  - `xUART0Mutex` — Mutex protecting UART access

---

### vHeaterSetLevel  
*(Created twice: one for Passenger Seat and one for Driver Seat. Each task receives a pointer to its respective seat structure as a parameter.)*

- **Type:** Event-Driven  
- **Trigger:**  
  Waits for button events via `xButtonEventGroup`  
  - **Passenger Seat:** `mainSW1_INTERRUPT_BIT`  
  - **Driver Seat:**  
    - `mainSW2_INTERRUPT_BIT`  
    - `mainExternal_Button_INTERRUPT_BIT`  

- **Description:**  
  Handles user button presses and cycles the desired heating temperature level.

---

### vControlHeater  
*(Created twice: one for Passenger Seat and one for Driver Seat. Each task receives a pointer to its respective seat structure as a parameter.)*

- **Type:** Event-Based  
- **Trigger:**  
  Unblocked by the corresponding seat’s **Heater Semaphore** (given by `vCheckSeatTemp`)  
- **Description:**  
  Controls heater output and LEDs for each seat based on the updated heater state.

Temp: 34.7 C
Level: HIGH
Heater: OFF
