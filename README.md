# SDR_ESP32

Ein **experimenteller** Softwarebaukasten für Software Defined Radios (oder andere DSP-Spielereien) auf Basis eines ESP32 (Audio Kits) und mit Hilfe des ESP-IDF/ADF und ESP-DSP Frameworks realisiert. 

![1](https://github.com/BM45/SDR_ESP32/blob/main/pics4www/1.jpg)

Wie auch schon bei unserem ESP32-basierten iRadioMini (https://github.com/BM45/iRadioMini/) und ganz nach dem Vorbild des ESP32-ADF, erfolgt die Signalverarbeitung blockweise (AudioElemente, siehe https://espressif-docs.readthedocs-hosted.com/projects/esp-adf/en/latest/api-reference/framework/audio_element.html), die zu einer Audiopipeline zusammengeschaltet wurden. Die Stereo-Daten (hier I/Q-Signale) werden dabei von Block zu Block durchgereicht und in jedem Block spezifisch behandelt oder bearbeitet. Eigene Blöcke können durch die Interfaces (Schablonen) des ESP32-ADF leicht selbst hergestellt werden und zur Audiopipeline hinzugefügt werden. Über weitere FreeRTOS-Prozesse kann Einfluss auf die Bearbeitung in der Pipeline genommen werden (gpiod, displayd, ... analog unseres iRadio und iRadioMini-Projektes)

So können zum Beispiel leicht eigene ZF/NF- Filter im Zeitbereich, oder mittels iFFT auch im Frequenzbereich hinzugefügt werden. Adaptive Filter für Noise Reduction, zusätzliche Demodulatoren oder gar Dekoder für bestimmte Betriebsarten (FT8, CW, RTTY, ...) sind denkbar. Eine Freeware zum Filterentwurf liegt bei (WinFilter unter SDR_ESP32/main/modules/WinFilter/ ) und kann auch unter unixoiden Betriebssystemen zum Beispiel mit wine (https://www.winehq.org/) genutzt werden.

![2](https://github.com/BM45/SDR_ESP32/blob/main/pics4www/2.jpg)

## RF-Frontends für das SDR_ESP32.

Das SDR_ESP32 hat natürlich durch die Nutzung der I2S-Audiocodecs nur eine relativ geringe Bandbeite (<< 100 kHz) und somit die Funktion eines Basisbandprozessors. Es kann ohne weitere Hilfe nicht direkt die ZF-Frequenzen (455 kHz, 10.7 MHz) gängiger Radios abtasten oder als Direktempfänger gar den Kurzwellenbereich empfangen. Auch die I/Q-Signalgenerierung (Q = 90° Phasenversatz zu I) muss beim ESP32_SDR noch extern erfolgen. Das ist aber relativ einfach und mit den längst bekannten und gängigen I/Q-Mischerschaltungen, die mit gerigem Bauteil- und IC-Einsatz auskommen und günstig herzustellen sind, realisierbar. Ein Großteil kommerzieller SDR-Empfänger (Radios, Fernseher, Funktelefone) nutzen solche Mischer, um auf eine LOW-IF oder gar ZERO-IF zu kommen, also eine Umsetzung von HF direkt auf 0 kHz (DC), um dort eine Basisbandverabeitung zu machen. 

Beispiel eines der bekanntesten I/Q-Mischer im Hobbybereich, der DR2-xx-Mischer:
![5](https://github.com/BM45/SDR_ESP32/blob/main/pics4www/5.jpg)

Viele Bauvorschläge, PCBs und Schaltpläne liegen in der Dokumentation unter https://github.com/BM45/SDR_ESP32/blob/main/datasheets/QRP%20YU1LM%20SDR-RX%20TX.pdf

Auch Fertigprojekte wie das Elektor-SDR-Shield lassen sich leicht für dieses SDR hier nutzen. So ist mit wenig Aufwand ein PC-unabhängiger SDR-Empfänger möglich!

![3](https://github.com/BM45/SDR_ESP32/blob/main/pics4www/3.jpg)

Wer erstmal keine Lust aufs Bauen hat, kann auch mit den vielen weltweit vertretenen Kiwi-SDRs ein I/Q-Signal zu Testzwecken erzeugen.
Eine Karte mit öffentlichen Empfängern gibt es unter http://rx.linkfanel.net/ .

In der Weboberfläche wählt man unter den Demodulationsarten "IQ" aus, damit umgeht man den Demodulator im Kiwi und schickt ein I/Q-Rohsignal über die Soundkarte des PCs zum ESP32. Für Test- und Entwicklungszwecke sehr gut nutzbar, ohne den Lötkolben auch nur einmal angeheizt zu haben.

![4](https://github.com/BM45/SDR_ESP32/blob/main/pics4www/4.jpg)
