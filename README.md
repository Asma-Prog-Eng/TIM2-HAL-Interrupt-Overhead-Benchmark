# STM32 TIM2 Dual-Pin Control & Context Benchmarking

A precise periodic interrupt application utilizing a hardware timer on an **STM32F103** microcontroller. This project implements a dual-channel real-time performance instrumentation strategy to isolate, measure, and analyze high-level application execution times versus low-level firmware framework overhead without triggering interrupt flag lockups.

## 🚀 Project Features
* **Precise Hardware Timing:** Configured TIM2 running on a **36 MHz System Clock (SYSCLK)** to execute an interrupt sequence precisely every 100 ms.
* **Dual-Channel Profiling Architecture:**
  * **Channel 0 (PA9 - LED Application Output):** Controlled via `HAL_TIM_PeriodElapsedCallback` to generate a stable macro timebase.
  * **Channel 1 (PA10 - ISR Instrumentation Monitor):** Toggled instantly at the entry and exit points of `TIM2_IRQHandler` to measure raw CPU processing windows.
* **Robust Flag Management:** Employs explicit status register clearing (`TIM2->SR = 0;`) right after the HAL handler to mitigate infinite loop ISR traps from unhandled peripheral flags.

## 🛠️ Hardware & Tools
* **Microcontroller:** STM32F103 (ARM Cortex-M3)
* **System Clock:** 36 MHz (1 clock cycle = ~27.7 nanoseconds)
* **IDE:** STM32CubeIDE (GCC Compiler)
* **Measurement Tool:** USB Logic Analyzer paired with **Saleae Logic 2** software

---

## 📊 Logic Analyzer Empirical Benchmarks

The interactions between the hardware timer and software execution layers were verified simultaneously using a digital logic analyzer at a high sampling rate. 

### 1. Macro Application Layer (Channel 0 - PA9)
* **Observed Waveform:** Perfectly balanced macro square wave.
* **Measured Metrics:** **50% Duty Cycle**, **4.995 Hz Frequency**, and a half-cycle width of **100.103 ms** (Total Period = **200.206333 ms**).
* **Analysis:** Validates that the hardware prescaler and counter configurations generate a highly accurate 100 ms timebase under a 36 MHz CPU clock.

### 2. Micro Interrupt Residency Layer (Channel 1 - PA10)
* **Observed Waveform:** Ultra-narrow, isolated microsecond pulses aligning perfectly with every state transition (both rising and falling edges) of Channel 0.
* **Measured ISR Window ($\Delta T$):** **9.417 µs**
* **Analysis:** At 36 MHz, an execution window of 9.417 µs equates to exactly **340 CPU clock cycles** spent inside the interrupt context. 
---
## 📈 Waveform Capture Reference

Below is the verified hardware timing profile captured from the Saleae Logic 2 interface, proving the clean alignment of the 9.417 µs ISR execution spike against the 100 ms application toggle:
<img width="1868" height="630" alt="image" src="https://github.com/user-attachments/assets/95a0f8a0-a9ef-4565-8ed0-f2d6a68d3610" />

---

## 🔧 How to Run
1. Clone this repository into your STM32CubeIDE workspace.
2. Hook up Channel 0 of your logic analyzer to pin **PA9** and Channel 1 to pin **PA10**.
3. Flash the target microcontroller.
4. Open your logic analyzer software, ensure your sampling rate is set comfortably high (e.g., 20 MHz or above), and capture the live real-time interaction between hardware intervals and software execution.
