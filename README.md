&#x20;Motor Predictive Maintenance System



IoT-based motor health monitoring system using ESP32 that detects developing faults through vibration, current, and temperature sensors with real-time ThingSpeak cloud visualization.





Problem Statement

Industrial motors fail without warning, causing costly downtime. This system continuously monitors motor health and detects faults early by correlating data from multiple sensors.





&#x20;Components Used

| Component | Purpose | Cost |

|---|---|---|

| ESP32 DevKit v1 | Microcontroller + WiFi | ₹500 |

| SW-420 | Vibration detection | ₹100 |

| ACS712 (5A) | Current monitoring | ₹200 |

| DS18B20 | Temperature monitoring | ₹150 |







&#x20;Circuit Connections

| Sensor | Pin | ESP32 |

| SW-420 | DO | GPIO5 |

| SW-420 | VCC | 3.3V |

| SW-420 | GND | GND |

| ACS712 | OUT | GPIO34 |

| ACS712 | VCC | 3.3V |

| ACS712 | GND | GND |

| DS18B20 | DATA | GPIO4 |

| DS18B20 | VCC | 3.3V |

| DS18B20 | GND | GND |





&#x20;Fault Detection Logic

| Condition | Vibration | Current | Temperature |

| Normal | 0 | <0.5A | <45°C |

| Early Fault | Occasional 1 | 0.3-0.5A | 35-42°C |

| Developing Fault | Frequent 1 | 0.5-0.8A | 42-50°C |

| Critical Fault | Constant 1 | >0.8A | >50°C |



&#x20;Alert Conditions

\- Vibration = 1 → Abnormal vibration alert

\- Current > 0.5A → High load alert

\- Temperature > 45°C → Overheating alert

\- Vibration + High Current → Mechanical fault

\- High Current + High Temperature → Motor failure risk





&#x20;ThingSpeak Graphs



&#x20;Vibration (Field 1)

!\[Vibration](docs/thingspeak\_graphs/vibration\_simulation.png)



&#x20;Current (Field 2)

!\[Current](docs/thingspeak\_graphs/current\_simulation.png)



&#x20;Temperature (Field 3)

!\[Temperature](docs/thingspeak\_graphs/temperature\_simulation.png)





How to Run

&#x20;Hardware

1\. Connect sensors as per circuit table above

2\. Open `src/hardware/main.ino`

3\. Add credentials and upload

4\. Place DS18B20 near motor, SW-420 on motor body

5\. Run fan through ACS712





&#x20;Project Structure

motor-predictive-maintenance/

├── src/

│   ├── hardware/         ← Real sensor code

│   └── tests/            ← Individual sensor tests

├── docs/

│   └── thingspeak\_graphs/← Graph screenshots

└── README.md

