# Smartwatch Simulator (FreeRTOS + ESP-IDF)

![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.5+-blue.svg)
![FreeRTOS](https://img.shields.io/badge/FreeRTOS-v10.4-green.svg)
![QEMU](https://img.shields.io/badge/QEMU-ESP32_Emulator-orange.svg)

## 🔍 Project Highlights
**Core Innovation:** Developed a realistic heart-rate simulation demonstrating FreeRTOS capabilities using ESP-IDF's QEMU emulator.

## Key Features
| Feature | Description | FreeRTOS Element Used |
|---------|-------------|-----------------------|
| ❤️ Heart-rate Sensor | Simulated values (60-140 BPM) with workout mode | Tasks + Event Groups |
| 👟 Step Counter | Auto-increments with CLI reset capability | Software Timers |
| 🕒 Time/Date | Simulated real-time clock | NVS Storage |
| 🔘 CLI Interface | Virtual buttons via terminal input | Queue |

## Getting Started

### Prerequisites
- ESP-IDF v5.5+
- QEMU (included with ESP-IDF)
- Python 3.8+

## Project Structure
```bash
smartwatch-simulator/
├── main/              # Core application
├── components/
│   ├── heart_rate/    # BPM simulation
│   ├── step_counter/  # Pedometer logic
│   └── cli/           # Command interface
│   └── datetime/      # datetime interface
└── partitions.csv     # Memory allocation
```

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.