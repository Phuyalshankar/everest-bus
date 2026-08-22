# 📖 EVEREST BUS (`everest-bus`) MASTER TUTORIAL
═════════════════════════════════════════════════════════════════════════════════════════════════
> **The Definitive Beginner-to-Advanced Developer Guide for Building 10,000+ Page Web Applications, 4K H.264/H.265 & RTSP Video Streaming, Realtime Audio DSP, In-Memory Databases, and IoT/PLC Networks on a Single Unified 24-Byte Binary Highway.**  
> *Author: Danphe Team (Nepal) 🇳🇵 | Version: 2.0.0*

---

# 📑 विस्तृत विषयसूची (Table of Contents)

1. [🌟 भाग १: परिचय र मूल दर्शन (Architecture & Philosophy)](#1-भाग-१-परिचय-र-मूल-दर्शन)
   - १.१ Everest Bus के हो र किन परम्परागत Stacks (Redux/Router) भन्दा फरक छ?
   - १.२ Physical-Virtual Parity को सिद्धान्त
   - १.३ ३-लाइनको सुनौलो नियम (`write`, `read`, `subscribe`, `emit`)
2. [🗺️ भाग २: विश्वव्यापी १६-बिट रजिस्टर म्याप (The 7 Global Register Ranges)](#2-भाग-२-विश्वव्यापी-१६-बिट-रजिस्टर-म्याप)
   - २.१ `0 - 999`: ⚡ Hardware / IoT Range (`1` = Relay 1, `30` = Temp ADC)
   - २.२ `1,000 - 1,999`: 🎨 UI / Graphics Range (`1000` = Route, `1010` = Dial Input)
   - २.३ `2,000 - 2,999`: 🗄️ Database Range (`2000` = Search, `2001` = Cart Total, `2010` = User Profile)
   - २.४ `3,000 - 3,999`: 🎥 Video / NVR Range (`3000` = Cam ID, `3020` = Motion Alert)
   - २.५ `4,000 - 4,999`: 🎙️ Audio / PBX Range (`4000` = Call State, `4001` = Partner Ext)
   - २.६ `5,000 - 5,999`: 🧠 AI / ML Edge Range (`5000` = Model ID, `5010` = Face Match)
   - २.७ `6,000 - 65,535`: 🧩 Custom Plugins, Forms & 10,000+ Pages (`6001` = Login, `6002` = Settings)
3. [🔀 भाग ३: सिंगल-बस राउटर इन्जिन (Register 1000 = Route)](#3-भाग-३-सिंगल-बस-राउटर-इन्जिन)
   - ३.१ React Router बिना केवल Register `1000` बाट नेभिगेसन
   - ३.२ Multi-page Switching र Nested Navigation
   - ३.३ Declarative Navigation (`action="bus:screen:Dashboard"`)
4. [⚡ भाग ४: विश्वव्यापी स्टेट व्यवस्थापन (Universal State & Zero Boilerplate)](#4-भाग-४-विश्वव्यापी-स्टेट-व्यवस्थापन)
   - ४.१ No Prop Drilling, No Context Nesting, No Redux Store
   - ४.२ `write()`, `read()`, र `subscribe()` मार्फत सम्पूर्ण एप व्यवस्थापन
   - ४.३ मेमोरी लिक हुन नदिने तरिका (Subscription Cleanup)
5. [📝 भाग ५: फर्म ह्यान्डलिंग र सब-अपकोड इन्जिन (Form Handling & Sub-Opcodes)](#5-भाग-५-फर्म-ह्यान्डलिंग-र-सब-अपकोड-इन्जिन)
   - ५.१ Sub-Opcode (JSON) भनेको के हो?
   - ५.२ Dynamic JSON Serialization र `flags = 0x01`
   - ५.३ Zero-Hook Declarative Form Handling
6. [🎥 भाग ६: भिडियो, अडियो र NVR स्ट्रिमिङ (4K RTSP / H.264 & Audio DSP)](#6-भाग-६-भिडियो-अडियो-र-nvr-स्ट्रिमिङ)
   - ६.१ Register Range `3000 - 3999` मा NVR / RTSP क्यामेरा व्यवस्थापन
   - ६.२ Register Range `4000 - 4999` मा VoIP / Opus अडियो कल व्यवस्थापन
7. [⚡ भाग ७: हार्डवेयर र माइक्रोकन्ट्रोलर इन्टिग्रेशन (IoT / ESP32 / NodeMCU)](#7-भाग-७-हार्डवेयर-र-माइक्रोकन्ट्रोलर-इन्टिग्रेशन)
   - ७.१ Web Button बाट NodeMCU Relay (Register `1..256`) बाल्ने/निभाउने
   - ७.२ Physical Wall Switch थिच्दा Web UI Live Sync हुने तरिका
   - ७.३ रियल-टाइम सेन्सर टेलिमेट्री (Register `30` = Temp ADC)
8. [🏭 भाग ८: औद्योगिक PLC र SCADA अटोमेसन](#8-भाग-८-औद्योगिक-plc-र-scada-अटोमेसन)
   - ८.१ Modbus RTU / TCP सँग 1:1 म्यापिङ
   - ८.२ Heavy Motor, VFD Speed र Web HMI/SCADA निर्माण
9. [🧱 भाग ९: २४-बाइट बाइनरी प्रोटोकल संरचना](#9-भाग-९-२४-बाइट-बाइनरी-प्रोटोकल-संरचना)
   - ९.१ Big-Endian हेडरको बाइट-बाइट विश्लेषण
   - ९.२ कमाण्ड अपकोडहरू (CMD Table)
   - ९.३ Checksum, Sequence Numbering र Zero-Copy Architecture
10. [🏗️ भाग १०: १०,००० पेजको मास्टर प्रोजेक्ट संरचना (`constants/registers.js`)](#10-भाग-१०-१०-०००-पेजको-मास्टर-प्रोजेक्ट-संरचना)
11. [🧪 भाग ११: पर्फर्मेन्स बेन्चमार्क (९४M ops/sec & ४००M pkts/sec)](#11-भाग-११-पर्फर्मेन्स-बेन्चमार्क)
12. [🎯 भाग १२: क्विक रिफरेन्स र चीटशीट](#12-भाग-१२-क्विक-रिफरेन्स-र-चीटशीट)

---

# 1. 🌟 भाग १: परिचय र मूल दर्शन

## १.१ Everest Bus के हो?
**Everest Bus** विश्वकै पहिलो **Unified Cyber-Physical Fieldbus Engine** हो। यसले एउटै २४-बाइट बाइनरी राजमार्गमा:
- 🎨 **Declarative UI / 10,000+ Pages** (React / Dolphin JSX)
- 🎥 **4K H.264/H.265 & RTSP Video Streaming** (NVR क्यामेरा र डोअरबेल)
- 🎙️ **Realtime Audio DSP** (VoIP, Intercom, Opus/G.711)
- 🗄️ **In-Memory Database & Search**
- ⚡ **Microcontrollers (ESP32, NodeMCU, Arduino, PLC)**
लाई बिना कुनै Redux, React Router, GraphQL वा MQTT Broker सिधै जोड्छ।

## १.२ ३-लाइनको सुनौलो नियम (The 3-Line Golden Rule)
```javascript
const { EverestBus, CMD } = require('everest-bus');

// १. State/Data लेख्ने (Set State)
EverestBus.write(1000, 'Dashboard');

// २. State/Data पढ्ने (Get State)
const current = EverestBus.read(1000);

// ३. State बदलिँदा सुन्ने (Reactivity)
EverestBus.subscribe(1000, (val) => {
  console.log("State Changed:", val);
});
```

---

# 2. 🗺️ भाग २: विश्वव्यापी १६-बिट रजिस्टर म्याप

README मा परिभाषित मापदण्ड अनुसार Everest Bus का **६५,५३६ वटा ठेगानाहरूलाई ७ वटा दायरामा** विभाजन गरिएको छ:

```
┌────────────────────┬───────────┬─────────────────────────┬────────────────────────────────────────────────────────┐
│ Decimal Range      │ Hex Range │ Domain                  │ Sub-Engine Description & Practical Examples            │
├────────────────────┼───────────┼─────────────────────────┼────────────────────────────────────────────────────────┤
│ 0 - 999            │ 0x0000..  │ ⚡ Hardware / IoT        │ Relays, Sensors, Solenoids (`1`=Relay1, `30`=Temp ADC) │
│ 1,000 - 1,999      │ 0x03E8..  │ 🎨 UI / Graphics        │ JSX Elements, Screens, Dials (`1000`=Route, `1010`=Dial)│
│ 2,000 - 2,999      │ 0x07D0..  │ 🗄️ Database             │ In-Memory Sync, Records (`2000`=Search, `2001`=Cart)   │
│ 3,000 - 3,999      │ 0x0BB8..  │ 🎥 Video / NVR          │ 4K H.264/H.265, RTSP (`3000`=Cam ID, `3020`=Motion)    │
│ 4,000 - 4,999      │ 0x0FA0..  │ 🎙️ Audio / PBX          │ VoIP Calls, Opus (`4000`=Call State, `4001`=Peer Ext)  │
│ 5,000 - 5,999      │ 0x1388..  │ 🧠 AI / ML Edge         │ Face Recognition (`5000`=Model ID, `5010`=Face Match)  │
│ 6,000 - 65,535     │ 0x1770..  │ 🧩 Custom & 10k Pages   │ User Forms, 10,000+ Pages (`6001`=Login, `6002`=Config)│
└────────────────────┴───────────┴─────────────────────────┴────────────────────────────────────────────────────────┘
```

---

# 3. 🔀 भाग ३: सिंगल-बस राउटर इन्जिन (Register 1000 = Route)

## ३.१ Register 1000 बाट नेभिगेसन
README मा तोकिए अनुसार **Register `1000`** ले Active Screen / Route नियन्त्रण गर्छ:

```javascript
const { EverestBus, EverestDeclarative } = require('everest-bus');

// १. पेज स्विच गर्न:
EverestBus.write(1000, 'Dashboard');

// २. वा Declarative Action मार्फत १-लाइनमै:
EverestDeclarative.executeAction('bus:screen:Dashboard');
```

## ३.२ पूर्ण Router कार्यान्वयन कोड (Zero React Router):

```jsx
import React from 'react';
import ReactDOM from 'react-dom/client';
import { EverestBus, EverestDeclarative } from 'everest-bus';

// १. विभिन्न पेजहरू
function HomePage() {
  return <div><h1>🏠 Home Page</h1><button onClick={() => EverestBus.write(1000, 'About')}>Go About</button></div>;
}

function AboutPage() {
  return <div><h1>ℹ️ About Page</h1><button onClick={() => EverestBus.write(1000, 'Home')}>Go Home</button></div>;
}

function ContactPage() {
  return <div><h1>📞 Contact Page</h1></div>;
}

const SCREENS = {
  Home:    <HomePage />,
  About:   <AboutPage />,
  Contact: <ContactPage />,
};

// २. Root App Router (Register 1000 Listen गर्छ)
function App() {
  const currentRoute = EverestBus.read(1000, 'Home'); // Reg 1000 = Route
  return (
    <div>
      <nav style={{ display: 'flex', gap: '10px', background: '#0f172a', padding: '10px' }}>
        <button onClick={() => EverestBus.write(1000, 'Home')}>Home</button>
        <button onClick={() => EverestBus.write(1000, 'About')}>About</button>
        <button onClick={() => EverestBus.write(1000, 'Contact')}>Contact</button>
      </nav>
      <main style={{ padding: '20px' }}>
        {SCREENS[currentRoute] || <HomePage />}
      </main>
    </div>
  );
}

const root = ReactDOM.createRoot(document.getElementById('root'));
EverestBus.subscribe(1000, () => root.render(<App />));
root.render(<App />);
```

---

# 4. ⚡ भाग ४: विश्वव्यापी स्टेट व्यवस्थापन

## ४.१ No Prop Drilling सिद्धान्त
कुनै पनि कम्पोनेन्टबाट अर्को कम्पोनेन्टमा डाटा पुर्‍याउन `props` वा `Context.Provider` चाहिन्छ भन्नु गलत हो। सिधै Register मा लेख्ने र पढ्ने:

```javascript
// Search Bar ले Register 2000 (Database Search Range) मा लेख्छ:
EverestBus.write(2000, 'Kathmandu');

// Product List ले सिधै पढ्छ:
const searchQuery = EverestBus.read(2000);
```

---

# 5. 📝 भाग ५: फर्म ह्यान्डलिंग र सब-अपकोड इन्जिन

## ५.१ Custom Register Range (`6001+`) मा Form Handling

```jsx
import React from 'react';
import { EverestBus, CMD } from 'everest-bus';

function MasterLoginForm() {
  function handleSubmit(e) {
    e.preventDefault();

    // १. FormData → Sub-Opcode JSON Object
    const payload = Object.fromEntries(new FormData(e.target));
    // { email: 'shankar@danphe.com', password: '1234', ext: '101' }

    // २. Register 6001 (Custom Form Space) मा Local State सेभ
    EverestBus.write(6001, payload);

    // ३. Server (Node 0) मा २४-बाइट बाइनरी प्याकेट पठाउने (flags = 0x01 Auto JSON)
    EverestBus.emit(0, CMD.REGISTER, payload);

    // ४. Router (Reg 1000) मार्फत Dashboard मा स्विच
    EverestBus.write(1000, 'Dashboard');
  }

  return (
    <form onSubmit={handleSubmit} style={{ display: 'flex', flexDirection: 'column', gap: '8px', width: '280px' }}>
      <h3>🔐 Everest Login</h3>
      <input name="email" placeholder="Email" required />
      <input name="password" type="password" placeholder="Password" required />
      <input name="ext" placeholder="Extension (e.g. 101)" required />
      <button type="submit" style={{ padding: '10px', background: '#0284c7', color: 'white' }}>
        Login 🚀
      </button>
    </form>
  );
}
```

---

# 6. 🎥 भाग ६: भिडियो, अडियो र NVR स्ट्रिमिङ

Everest Bus को Register Range `3000 - 3999` (Video) र `4000 - 4999` (Audio) उच्च-गतिको रियल-टाइम मिडियाको लागि समर्पित छ:

```javascript
// १. NVR क्यामेरा ३ को 4K RTSP स्ट्रिम खोल्ने (Register 3000 = Cam ID)
EverestBus.write(3000, { camId: 3, streamUrl: 'rtsp://192.168.1.50/stream1' });

// २. VoIP अडियो कल सुरु गर्ने (Register 4000 = Call State, 4001 = Partner Ext)
EverestBus.write(4001, '102'); // Call extension 102
EverestBus.emit(102, CMD.INVITE, '102');
```

---

# 7. ⚡ भाग ७: हार्डवेयर र माइक्रोकन्ट्रोलर इन्टिग्रेशन

## ७.१ Web Button बाट NodeMCU Relay (Register `1`) बाल्ने

```javascript
// Hardware Range: 0 - 999
// Register 1 = Relay 1
EverestBus.write(1, 1); // Turn ON Relay 1
EverestBus.emit(300 /* NodeMCU ID */, CMD.RELAY_SET, { relayId: 1, state: 1 });
```

## ७.२ NodeMCU / ESP32 Arduino Sketch:

```cpp
#include <ESP8266WiFi.h>
#include "EverestBus_Arduino.h"

WiFiClient client;
EverestArduinoBus bus;

void onRelaySet(uint8_t cmd, uint32_t senderId, const uint8_t* payload, uint16_t len) {
    if (len >= 3) {
        uint16_t relayId = (payload[0] << 8) | payload[1];
        uint8_t state = payload[2];
        if (relayId == 1) {
            digitalWrite(D1, state ? LOW : HIGH);
        }
    }
}

void setup() {
    EverestArduino_Init(&bus, 300 /* NodeMCU ID */);
    EverestArduino_On(&bus, 0x23 /* CMD_RELAY_SET */, onRelaySet);
    client.connect("192.168.1.3", 9092);
}

void loop() {
    while (client.available()) {
        uint8_t b = client.read();
        EverestArduino_Feed(&bus, &b, 1);
    }
}
```

---

# 8. 🧱 भाग ९: २४-बाइट बाइनरी प्रोटोकल संरचना

```
┌──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┐
│ Offset   │ 0..1     │ 2        │ 3        │ 4..7     │ 8..11    │ 12..15   │ 16..19   │ 20..21   │
├──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤
│ Field    │ Signature│ Version  │ CmdType  │ SenderID │ TargetID │ PayloadL │ SeqNo    │ Session  │
│ Size     │ 2 Bytes  │ 1 Byte   │ 1 Byte   │ 4 Bytes  │ 4 Bytes  │ 4 Bytes  │ 4 Bytes  │ 2 Bytes  │
│ Value    │ 0x5442   │ 0x02     │ Opcode   │ Int32BE  │ Int32BE  │ Int32BE  │ Int32BE  │ UInt16BE │
└──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘
Offset 22: Flags (0x00=Binary, 0x01=JSON) | Offset 23: CRC8 / Checksum | Offset 24..N: Payload
```

---

# 9. 🏗️ भाग १०: १०,००० पेजको मास्टर प्रोजेक्ट संरचना

### मास्टर कन्स्ट्यान्ट्स फाइल (`constants/registers.js`):

```javascript
export const REGS = {
  // ── 0-999: Hardware / IoT ──
  RELAY_1:             1,
  RELAY_2:             2,
  TEMP_SENSOR_ADC:     30,

  // ── 1000-1999: UI / Graphics ──
  ROUTE:               1000, // Master Router
  DIAL_INPUT:          1010, // Dial Buffer
  LCD_LINE_1:          1020,

  // ── 2000-2999: Database ──
  SEARCH_QUERY:        2000,
  CART_TOTAL:          2001,
  USER_PROFILE:        2010,

  // ── 3000-3999: Video / NVR ──
  ACTIVE_CAM_ID:       3000,
  MOTION_ALERT_STATE:  3020,

  // ── 4000-4999: Audio / PBX ──
  CALL_STATE:          4000,
  PEER_EXTENSION:      4001,

  // ── 5000-5999: AI / ML Edge ──
  AI_MODEL_ID:         5000,
  FACE_MATCH_RESULT:   5010,

  // ── 6000-65535: Custom Pages & Forms ──
  PAGE_LOGIN_FORM:     6001,
  PAGE_SETTINGS_FORM:  6002,
  PAGE_BILLING_FORM:   6003,
  PAGE_CRM_CUSTOMER_1: 6010,
  // ... १०,००० पेजहरू क्रमशः थप्दै जाने
};
```

---

# 10. 🧪 भाग ११: पर्फर्मेन्स बेन्चमार्क

| बेन्चमार्क परीक्षण | सङ्ख्या | समय (Elapsed) | प्रमाणित गति (Throughput) |
| :--- | :--- | :--- | :--- |
| **Register Read/Write** | १,०००,००० अपरेसन | **२१.२८ ms** | ⚡ **९३,९९८,६५६ ops/sec** |
| **C++ Frame Dispatch** | ५००,००० फ्रेमहरू | **१.२५ ms** | ⚡ **४००,०००,००० pkts/sec** |
| **Multi-Node Burst** | ५० नोड कन्करेन्ट | **३३.०८ ms** | ⚡ **१,५११,२३१ msgs/sec** |
| **RAM Footprint** | Static Ring Buffer | **< ५१२ Bytes** | 💾 **Zero Heap Fragmentation** |
| **Network Resilience** | Byte fragmentation | **Zero Crashes** | 🛡️ **Instant Auto-Resync** |

---

# 11. 🎯 भाग १२: क्विक रिफरेन्स र चीटशीट

```javascript
const { EverestBus, EverestDeclarative, CMD } = require('everest-bus');

// ── Read & Write ───────────────────────────────────────
EverestBus.write(1000, 'Dashboard');                 // Router switch (Reg 1000)
EverestBus.write(6001, { user: 'Admin', pin: 1234 }); // Form write (Reg 6001)
const currentScreen = EverestBus.read(1000);         // 'Dashboard'

// ── Subscription ───────────────────────────────────────
const unsub = EverestBus.subscribe(1000, (page) => console.log("Page:", page));
unsub(); // Cleanup

// ── Hardware & Binary Dispatch ─────────────────────────
EverestBus.write(1, 1);                              // Relay 1 = ON
EverestBus.emit(300, CMD.RELAY_SET, { relayId: 1, state: 1 }); // ESP32

// ── Declarative JSX Actions ────────────────────────────
// <span className="text-3xl">[bus:1000]</span>
// <button action="bus:key:5">5</button>
// <button action="bus:dial:102">Call 102</button>
// <button action="bus:relay:1:on">Unlock Door</button>
```

---
*MIT License — Proudly Engineered by **Danphe Team (Nepal)** 🇳🇵💎*
