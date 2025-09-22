# 🎮 IOTGamified 3.0 – Build Your Own IoT Game Controller!  

Welcome to **IOTGamified 3.0**, hosted by the **IOTHINC Club of VIT Vellore**! 🚀  

In this two-day hands-on workshop, you’ll learn how to design and build your very own **IoT-powered game controller** using **ESP modules** and a **Python-based UDP communication system**.  
Get ready to explore IoT, microcontrollers, sensors, and networking – and walk away with a **fully functional DIY game controller!** 🌟  

---

## 📚 Workshop Roadmap  

Here’s what we’ll cover:  

- **Introduction to IoT** → What IoT is, its real-world applications, and future opportunities 🌐  
- **Microcontrollers Deep Dive** → ESP32 vs Arduino: features, differences, and when to use which  
- **Basic Sensors & Actuators** → Hands-on with temperature sensors, buzzers, and how to integrate them  
- **Virtual Circuits on Wokwi** → Build and test circuits online before hardware deployment 👾  
- **Networking Protocols** → Learn about **UDP** (core focus), with an overview of TCP  
- **Arduino IDE Setup** → Installing and programming ESP modules using Arduino IDE  
- **Multiplexers (MUX)** → Crash course on how MUX works and its use in hardware projects  
- **Networking Basics** → Ports, IP addresses, sockets, and how devices talk to each other 🌍  

---

## 💻 Installation & Setup Guide  

To ensure a smooth workshop experience, please install the following tools before **Day 1**.  

### ✅ 1. Install Visual Studio Code (VS Code)  
Your main editor for ESP and Python scripts.  

🔗 [Download VS Code](https://code.visualstudio.com/download)  

**Steps:**  
1. Choose your OS (Windows, macOS, Linux).  
2. Run the installer and follow on-screen steps.  
3. Open VS Code to confirm installation.  

---

### ✅ 2. Install Python 3  
Required for UDP communication and game controller simulation.  

🔗 [Download Python](https://www.python.org/downloads/)  

**Steps:**  
1. Download the latest stable **Python 3.x** version.  
2. During installation:  
   - ✅ Check **“Add Python to PATH”**  
3. Verify installation:  
   ```bash
   python --version



---

### ✅ 3. Install Arduino IDE  
Used to program the ESP module.  

🔗 [Download Arduino IDE](https://www.arduino.cc/en/software)  

**Steps:**  
1. Download the installer for your OS.  
2. Run setup and complete installation.  
3. Open Arduino IDE to verify it launches.  

---

### ✅ 4. Install Required Python Libraries  

Open **Command Prompt / Terminal** and run:  

For **game controller simulation (vgamepad):**  
   ```bash
   pip install vgamepad
   ```
For **networking (socket – already included in Python, but just in case):**  

```bash
pip install sockets
```

Verify installation:  

```bash
python -m pip show vgamepad
