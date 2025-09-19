# ESP32 MQTT Demo

This project demonstrates an MQTT (Message Queuing Telemetry Transport) server and client implementation on ESP32.

## Getting Started

### Prerequisites
1. **Install Visual Studio Code**: Download from [https://code.visualstudio.com/](https://code.visualstudio.com/)

2. **Install PlatformIO Extension**: 
   - Open VS Code
   - Go to Extensions
   - Search for "PlatformIO IDE" and install it

3. **Install Wokwi Extension**:
   - In VS Code Extensions, search for "Wokwi Simulator"
   - Install the official Wokwi extension

4. **Create Wokwi Account**:
   - Visit [https://wokwi.com](https://wokwi.com)
   - Register for a free account
   - In Visual Studio Code, when the Wokwi extension asks for a license, follow the setup steps
   - The license is free - just follow the steps!

### Running the Project
1. **Open Project**: Open this folder in VS Code.
    - Wait for the Platform IO extension: it will automatically install the required tools and dependencies
2. **Build**: Use PlatformIO to build the project (Ctrl+Alt+B)
3. **Simulate**: Open `diagram.json`, the ESP32 and its attached led should appear. Press the play button to start the simulation
    - You can also run the simulation by opening the VSCode command palette (F1 usually), and use `Wokwi: Start Simulator`
    - If it doesn't work, open `wokwi.toml` and adjust the path slashes: `\` for Windows, `/` for other operating systems
    - Wokwi should automatically open the serial monitor

## File Structure

### Main Files
- **`main.cpp`**: Main application entry point, handles WiFi setup and starts the server and client components
  - For Wokwi simulation: WiFi connection is automatically simulated
- **`mqtt_server.h/cpp`**: MQTT server implementation with message handlers
- **`mqtt_client.h/cpp`**: MQTT client implementation for publishing messages

### Architecture Overview

#### MQTT Server (`MqttServer` class)
- **Purpose**: Handles incoming MQTT messages
- **Topics**:
  - `esp32/hello`: Responds with "world" to any message
  - `esp32/led`: Controls ESP32 built-in LED
- **Features**:
  - `esp32/led` with "get": Returns LED state ("on"/"off") via `esp32/led/response`
  - `esp32/led` with "on"/"off": Sets LED state (accepts "on"/"off", "1"/"0", "true"/"false")

#### MQTT Client (`MqttClient` class)
- **Purpose**: Makes MQTT requests to test server functionality
- **Behavior**:
  - Sends a one-time message "hello" to `esp32/hello` after 5 seconds
  - Periodically toggles LED by publishing "on"/"off" to `esp32/led` every 2 seconds
  - Subscribes to response topics and displays responses from server

## Usage

The application will:
1. Connect to WiFi (simulated using Wokwi)
2. Connect both server and client to MQTT broker (mqtt-dashboard.com)
3. Server subscribes to `esp32/hello` and `esp32/led` topics
4. Client subscribes to response topics and automatically tests server functionality

## Dependencies
- ESP32 Arduino Core
- WiFi library
- PubSubClient library (knolleary)
