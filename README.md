# PnRAX - Decompiler and PE viewer

I built this tool because i wanted a lightweight, modular, and easy-to-use GUI for exploring x86-64 Windows PE files and decompiling code without the bulk of larger applications.
 
<img width="1662" height="900" alt="PnRAX_V0 1Scr" src="https://github.com/user-attachments/assets/285518e5-a044-4ca3-a787-6cbc84fe9542" />

## Third-party
* Capstone
* ImGui
* fmt


## Notes
* This project is only for Windows, because it uses the windows loader structures to load and map the PE file
* Created a custom wrapper for imgui called ImEasy for easier use of imgui

## Build instructions
Before you start make sure you have the vcpkg installed.
* Clone the repository then run cmake to build the project

## Contribution
If you have any idea that makes the application better or any useful module idea please create a issue and explain it to me.

## Contact
You can reach me in telegram via @KVMSwitch
