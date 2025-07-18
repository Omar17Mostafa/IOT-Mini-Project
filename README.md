# 🚀 SectorB5 IoT Project

**Mentored by:** Omar Barakat

## 📖 Project Overview

This mini IoT project demonstrates **two-way real-time communication** between hardware and a mobile app using the **MQTT protocol** and the **PubSubClient** library on an **ESP32 microcontroller**.

The system allows:
- A **virtual switch** in the mobile app (publisher) to control a **hardware LED** (subscriber).
- A **physical switch** (publisher) to control a **virtual LED** in the mobile app (subscriber), and simultaneously light up a second hardware LED on as visual feedback.

This project reflects a typical **Internet of Things (IoT)** use case with remote device control, state synchronization, and cloud-based messaging using **MQTT** — a lightweight and efficient protocol ideal for real-time systems.

---

## 🔌 Components

- ESP32 board
- 2 LEDs
- One controlled by mobile app (GPIO14)
- One as button feedback (GPIO23)
- Push button switch with pull-up resistor (GPIO0)
- Resistors: 220Ω (for LEDs)
- Breadboard, jumper wires
- Wi-Fi access point
- Smartphone with **IoT MQTT Panel**
- USB cable for ESP32 programming

---

## 🛠️ Wiring

| Component               | ESP32 Pin |
|-------------------------|-----------|
| App-Controlled LED      | GPIO14    |
| Button Feedback LED     | GPIO23    |
| Button (with pull-up)   | GPIO0     |
| GND (LED cathodes)      | GND       |
| VCC (3.3V for pull-up)  | 3.3V      |

- The button is connected with a **pull-up resistor** to 3.3V.
- When the button is **pressed**, it pulls GPIO0 **LOW** (active-low logic).
- The feedback LED on GPIO23 gives **local confirmation** of the button press.

---

## 🛰️ MQTT Topics

| Topic                | Direction       | Description                               |
|----------------------|------------------|-------------------------------------------|
| `esp32/ledControl`   | App → ESP32      | Turns ON/OFF the LED on GPIO14            |
| `esp32/buttonStatus` | ESP32 → App      | Updates the app LED based on button state |

### 🔄 Message Flow

- **Publisher (App)** *(QoS 0)* sends `1` or `0` to `esp32/ledControl`
- **Subscriber (ESP32)** *(QoS 0)* listens and controls LED
- **Publisher (ESP32)** *(QoS 0)* sends `1` or `0` to `esp32/buttonStatus`
- **Subscriber (App)** *(QoS 0)* listens and updates virtual LED
- **Feedback LED** on GPIO23 is also toggled as local confirmation between the physical push button and the app LED

---

## 🔧 How MQTT Works Internally

MQTT (IOT Message Queuing Telemetry Transport) is a lightweight **publish/subscribe** protocol where devices communicate **through a central MQTT broker**. Here's how your project uses it:

1. **App as Publisher:**
   - The switch widget in the **IoT MQTT Panel** sends a message (`"1"` or `"0"`) to topic `esp32/ledControl`.
   - This message goes through the **MQTT broker** (like `broker.emqx.io`).

2. **ESP32 as Subscriber:**
   - ESP32 is subscribed to `esp32/ledControl`.
   - Once the broker receives the message, it **immediately pushes** it to all subscribers.
   - ESP32 receives the message, and sets GPIO14 HIGH or LOW to turn the LED ON/OFF.

3. **ESP32 as Publisher:**
   - When the physical button is pressed (GPIO0 pulled LOW), ESP32 publishes `"1"` to `esp32/buttonStatus`.
   - This message is sent to the broker.

4. **App as Subscriber:**
   - The LED widget in the app is subscribed to `esp32/buttonStatus`.
   - As soon as the broker receives the update, it forwards it to the app.
   - The app LED changes accordingly.

> 🧠 The broker ensures **decoupled communication**: the publisher and subscriber don't need to know about each other — only the topic matters.

---

## ⚙️ How It Works

1. **Mobile → ESP32 (LED Control):**
   - App sends `1` or `0` to `esp32/ledControl`.
   - ESP32 reads it and controls GPIO14 (main LED).

2. **ESP32 → Mobile (Button Feedback):**
   - When the button on GPIO0 is pressed (goes LOW), ESP32:
     - Sends `1` to `esp32/buttonStatus`
     - Turns ON the feedback LED on GPIO23
   - When released, sends `0` and turns feedback LED OFF.
   - App LED updates based on this.

---

## 📱 Mobile App Setup (IoT MQTT Panel)

1. **Install App:**  
   - Android: [IoT MQTT Panel](https://play.google.com/store/apps/details?id=snr.lab.iotmqttpanel)

2. **Create MQTT Broker Profile:**
   - Broker: `broker.emqx.io`
   - Port: `1883`
   - Client ID: anything unique (`omarmmomarom`)

3. **Add Widgets:**

| Widget Type | Topic                | Payload ON | Payload OFF |
|-------------|----------------------|------------|-------------|
| Switch      | `/OPERA/omar/led`   | `on`        | `off`         |
| LED         | `/sectorb5/omar/led` | `on`        | `off`         |

---

## 💡 Real-World Applications

This model is the foundation of many smart systems:

- **Smart Homes**  
  Remote light/appliance control + physical switch feedback

- **Industrial Automation**  
  Operator actions on site reflected in control centers, and vice versa

- **Security Systems**  
  Alarms or door sensors publish events; security apps subscribe

- **Healthcare**  
  Emergency buttons for patients with immediate app alert for staff

- **Remote Monitoring**  
  Any kind of switch or sensor input published to cloud dashboards

---

## 🎥 Schematic & Overview Figure images about the Project

![Image Alt](https://github.com/Omar17Mostafa/IOT-Mini-Project/blob/9cbb44372349c33de0a4783d5b8e4ff0d78749a2/Simulation%20Circuit%20.png)
![Image Alt]()

- ✅ App switch turns ON LED on GPIO14
- ✅ Physical button press:
  - Updates app’s LED widget
  - Turns ON feedback LED on GPIO23

---

## 🧠 Learnings

- Learned to implement **full-duplex MQTT messaging** with PubSubClient on ESP32.
- Understood the role of the **broker** in IoT communication and how topics isolate data streams.
- Used **pull-up logic** for GPIO0 and handled edge detection in code.
- Built a reliable IoT system that could scale into **real-world smart devices**.
- Gained experience with **mobile-to-hardware integration** using only free tools.
- Realized the importance of **visual confirmation mechanisms** like the feedback LED for debugging and user feedback.

---

