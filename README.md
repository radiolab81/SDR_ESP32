# SDR_ESP32

Ein experimenteller Softwarebaukasten für Software Defined Radios (oder andere DSP-Spielereien) auf Basis eines ESP32 (Audio Kits) mit Hilfe des ESP-IDF/ADF und ESP-DSP Frameworks. 

![1](https://github.com/BM45/SDR_ESP32/blob/main/pics4www/1.jpg)

Wie auch schon bei unserem ESP32-basierten iRadioMini (https://github.com/BM45/iRadioMini/) und ganz nach dem Vorbild des ESP32-ADF, erfolgt die Signalverarbeitung blockweise (AudioElemente), die zu einer Audiopipeline zusammengeschaltet wurden. Die Stereo-Daten (hier I/Q-Signale) werden dabei von Block zu Block durchgereicht und in jedem Block spezifisch behandelt oder bearbeitet. Eigene Blöcke können durch die Interfaces (Schabloben) des ESP32-ADF leicht selbst hergestellt werden und zu Audiopipeline hinzugefügt werden. Über weitere FreeRTOS-Prozesse kann Einfluss auf die Bearbeitung in der Pipeline genommen werden (gpiod, displayd, ... analog unseres iRadio und iRadioMini-Projektes)

![2](https://github.com/BM45/SDR_ESP32/blob/main/pics4www/2.jpg)

![3](https://github.com/BM45/SDR_ESP32/blob/main/pics4www/3.jpg)

![4](https://github.com/BM45/SDR_ESP32/blob/main/pics4www/4.jpg)
