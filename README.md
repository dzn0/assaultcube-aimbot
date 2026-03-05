# AssaultCube External Memory Controller
> **A technical Study on Memory Manipulation, 3D Vector Math, and WinAPI Interfacing.**

[![Platform](https://img.shields.io/badge/Platform-Windows-blue.svg)](https://en.wikipedia.org/wiki/Microsoft_Windows)
[![Language](https://img.shields.io/badge/Language-C%2B%2B17-orange.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Arch](https://img.shields.io/badge/Arch-x64-red.svg)](https://en.wikipedia.org/wiki/X86)

This project is a lightweight, external tool developed for **AssaultCube v1.2.0.2**. It serves as a practical application of Reverse Engineering concepts, focusing on how a separate process can synchronize with a game's memory space to perform real-time coordinate transformations.

---

## 🚀 Key Technical Features

* **External Execution**: Operates entirely in User Mode (Ring 3) using `ReadProcessMemory` and `WriteProcessMemory`, ensuring no code injection is performed.
* **FOV-Based Logic**: Implements angular distance calculations to prioritize targets closest to the player's crosshair rather than absolute 3D distance.
* **Linear Interpolation (Smoothing)**: Features an adjustable smoothing factor to transition view angles gradually, mimicking human input.
* **Real-time Telemetry**: Active console feedback showing Target Entity ID and FOV delta values.

<img width="1783" height="1017" alt="image" src="https://github.com/user-attachments/assets/18cc1250-4bdd-4d4e-841b-2de8f4641bac" />


## ⌨️ Controls & Interface

| Action | Input |
| :--- | :--- |
| **Activate Assist** | `Right Mouse Button (Hold)` |
| **Panic/Safe Exit** | `END Key` |

---

## 🛠️ Requirements & Environment

To compile and run this Proof of Concept, ensure your environment meets these specifications:

* **Target Game**: AssaultCube v1.2.0.2
* **Architecture**: Must be compiled as **x64** (64-bit).
* **Compiler**: Visual Studio 2019/2022 (C++17 Standard).
* **Character Set**: Must be set to `Use Multi-Byte Character Set` in Project Properties.



---

## 📝 Setup Instructions

1.  Launch **AssaultCube**.
2.  Join a match and ensure your character has **spawned (alive)**.
3.  Run the compiled `.exe` (found in `x64 > Release`) as **Administrator**.
4.  Hold the **Right Mouse Button** to engage the assist logic.

---

## 🚧 Technical Constraints (Bug Report)

* **Dynamic Pointer Resolution**: Currently, the script resolves base addresses and offsets at startup. Due to this, it must be launched while the player is alive in a match. Initialization at the main menu will result in null pointer references.

---

## 🎯 Development Roadmap (To-do)

- [ ] **Team ID Filter**: Implement a team check to differentiate between allies and enemies.
- [ ] **Persistence Fix**: Refactor initialization logic to allow software launch at any game state.
- [ ] **Recoil Compensation**: Implement a module to counteract weapon kickback offsets.

---

## ⚠️ Educational Disclaimer

This project is for **educational purposes only**. It was created to explore the internal workings of Windows processes and game engines. Using such tools in multiplayer environments is a violation of most Terms of Service and can result in permanent bans. **The author does not condone cheating in online gaming.**

---
**Developed by:** [@dzn](https://github.com/seu-usuario)  
**Contact:** Discord `d.zn.`
