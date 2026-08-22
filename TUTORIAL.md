# 📘 TITAN MICRO-BUS: MASTER UI & 10,000-PAGE ARCHITECTURE TUTORIAL
═══════════════════════════════════════════════════════════════════════════════
> **How to build ultra-scalable web, mobile, and hardware applications without React Hooks, Redux, or heavy routers.**

---

## 📑 विषयसूची (Table of Contents)
1. [🧠 मूल दर्शन (Core Philosophy)](#1-मूल-दर्शन-core-philosophy)
2. [🗺️ 16-Bit Register Map (0 – 65,535)](#2-16-bit-register-map-0--65535)
3. [🔀 Single-Bus Router (Register 10)](#3-single-bus-router-register-10)
4. [📝 Form Handling & Sub-Opcode (JSON) System](#4-form-handling--sub-opcode-json-system)
5. [🔄 Module-to-Module Communication (Zero Redux)](#5-module-to-module-communication-zero-redux)
6. [🏗️ 10,000 Pages Complete Architecture](#6-10000-pages-complete-architecture)
7. [⚡ 24-Byte Binary Packet & Flags (flags = 0x01)](#7-24-byte-binary-packet--flags-flags--0x01)
8. [📊 Traditional vs Titan Bus Comparison](#8-traditional-vs-titan-bus-comparison)
9. [🎯 Quick Reference Cheatsheet](#9-quick-reference-cheatsheet)

---

## 1. 🧠 मूल दर्शन (Core Philosophy)

परम्परागत Frameworks (React Router, Redux Toolkit, Zustand, Axios) मा १०,००० पेजको प्रोजेक्ट बनाउँदा हजारौं लाइन्सको "Glue-code" लेख्नुपर्छ।

**Titan Micro-Bus ले सबै कुरालाई एउटै १६-बिट मेमोरी बसमा रूपान्तरण गर्छ:**
- **Register Number** = विश्वव्यापी ठेगाना (Global Memory Address)
- **`write(reg, value)`** = स्टेट सेट गर्ने (Set State)
- **`read(reg)`** = स्टेट लिने (Get State)
- **`subscribe(reg, callback)`** = डाटा बदलिँदा UI अपडेट गर्ने (Reactivity)
- **`emit(target, cmd, payload)`** = २४-बाइट बाइनरी प्याकेट पठाउने (Network & Hardware)

---

## 2. 🗺️ 16-Bit Register Map (0 – 65,535)

```
┌──────────────┬──────────────────────────────────────────────────┐
│  0 – 999     │  ⚡ Core System                                   │
│              │  Reg 1  = Bus Status (ONLINE / OFFLINE)           │
│              │  Reg 10 = Active Screen ← ROUTER यही हो!         │
│              │  Reg 11 = Drawer State (open / close)            │
│              │  Reg 12 = Theme (light / dark / oled)            │
├──────────────┼──────────────────────────────────────────────────┤
│  1,000–9,999 │  🎨 UI Components                                │
│              │  Reg 1000 = Keypad Dial Buffer                   │
│              │  Reg 1010 = LCD Line 1                           │
│              │  Reg 1020 = Speedometer                          │
├──────────────┼──────────────────────────────────────────────────┤
│  10,000–     │  🗄️ Database / Records                           │
│  19,999      │  Reg 10001+ = Contacts                           │
│              │  Reg 15000+ = Call History                       │
│              │  Reg 19000  = Current User Profile               │
├──────────────┼──────────────────────────────────────────────────┤
│  20,000–     │  ⚙️ Hardware Relays & GPIO                       │
│  29,999      │  Reg 20001 = Relay 1                             │
│              │  Reg 20010 = Solenoid Lock                       │
├──────────────┼──────────────────────────────────────────────────┤
│  30,000–     │  📡 Sensors & Telemetry                          │
│  39,999      │  Reg 30001 = CPU Load                            │
│              │  Reg 30002 = Battery ADC                         │
│              │  Reg 30003 = Temperature                         │
├──────────────┼──────────────────────────────────────────────────┤
│  40,000–     │  🧩 Custom — तपाईंको Pages, Forms र Modules      │
│  65,535      │  40001 = Login Form                              │
│              │  40002 = Settings Form                           │
│              │  40003+ = तपाईंको जुनसुकै Page                   │
└──────────────┴──────────────────────────────────────────────────┘
```

---

## 3. 🔀 Single-Bus Router (Register 10)

React Router वा भारी नेभिगेसन लाइब्रेरी बिना सिधै **रजिस्टर १०** मार्फत स्क्रिन नियन्त्रण हुन्छ:

```javascript
const { TitanMicroBus } = require('@dolphin/titan-bus');

// १. जुनसुकै पेजमा जान:
TitanMicroBus.write(10, 'Login');        // → Login Page
TitanMicroBus.write(10, 'Dashboard');    // → Dashboard Page
TitanMicroBus.write(10, 'Settings');     // → Settings Page

// २. हालको पेज थाहा पाउन:
console.log(TitanMicroBus.read(10));     // 'Dashboard'

// ३. Router Switcher (App Root):
TitanMicroBus.subscribe(10, (activeScreen) => {
  renderActivePage(activeScreen);
});
```

### React / HTML Root Example:
```jsx
const SCREENS = {
  Login:     <LoginPage />,
  Dashboard: <DashboardPage />,
  Settings:  <SettingsPage />,
  Contacts:  <ContactsPage />,
  // १०,००० पेज भए पनि सिधै यहाँ थप्न सकिन्छ!
};

function App() {
  const currentScreen = TitanMicroBus.read(10, 'Login');
  return SCREENS[currentScreen] || <LoginPage />;
}

// Register 10 बदलिँदा अटोमेटिक UI Re-render
TitanMicroBus.subscribe(10, () => root.render(<App />));
```

---

## 4. 📝 Form Handling & Sub-Opcode (JSON) System

हरेक फारमका इनपुटहरू छुट्टाछुट्टै स्टेटमा राख्नु पर्दैन। **Form Register (जस्तै 40001)** भित्रका JSON Keys नै **Sub-Opcodes** हुन्:

```
Form Address (40001)  ──────┐
                            ├──► { email: "...", password: "..." }
Sub-Opcodes (JSON Keys) ────┘
```

### पूर्ण Form Example (बिना React Hooks):
```jsx
function LoginPage() {
  function handleSubmit(e) {
    e.preventDefault();

    // १. सबै input बाट सिधै Sub-Opcode JSON बनाउने
    const payload = Object.fromEntries(new FormData(e.target));
    // नतिजा: { email: 'shankar@danphe.com', password: '1234', ext: '101' }

    // २. Register 40001 मा Local State सेभ गर्ने
    TitanMicroBus.write(40001, payload);

    // ३. Server मा २४-बाइट बाइनरी प्याकेट पठाउने (JSON Sub-code: flags = 0x01)
    TitanMicroBus.emit(0 /* Server Node */, CMD.REGISTER, payload);

    // ४. Router मार्फत Dashboard मा स्विच गर्ने
    TitanMicroBus.write(10, 'Dashboard');
  }

  return (
    <form onSubmit={handleSubmit} style={{ display: 'flex', flexDirection: 'column', width: '300px', gap: '10px' }}>
      <h2>🔐 Titan Login</h2>
      <input name="email" type="email" placeholder="Email" required />
      <input name="password" type="password" placeholder="Password" required />
      <input name="ext" type="text" placeholder="Extension (e.g. 101)" required />
      <button type="submit">🚀 लगइन गर्नुहोस्</button>
    </form>
  );
}
```

### Form Data Read गर्न (जहाँबाट पनि):
```javascript
const loginData = TitanMicroBus.read(40001);
console.log(loginData.email); // 'shankar@danphe.com'
console.log(loginData.ext);   // '101'
```

---

## 5. 🔄 Module-to-Module Communication (Zero Redux)

परम्परागत Redux मा Module A बाट Module B मा डाटा पठाउन Actions, Reducers, र Selectors चाहिन्छ। 
**Titan Bus मा केवल `write` र `subscribe`:**

```javascript
// ── Auth Module (Login Success) ────────────────────────
TitanMicroBus.write(19000, { name: 'Shankar Phuyal', ext: '101' }); // Profile DB
TitanMicroBus.write(1, 'AUTHENTICATED');                             // Core Status
TitanMicroBus.write(10, 'Dashboard');                                // Router Switch

// ── Dashboard Module (Listen for Auth) ─────────────────
TitanMicroBus.subscribe(1, (status) => {
  if (status === 'AUTHENTICATED') {
    console.log("Welcome! Loading user dashboard...");
  }
});

// ── Top AppBar Module (Show User Profile) ──────────────
TitanMicroBus.subscribe(19000, (profile) => {
  if (profile) {
    updateTopBarUser(profile.name, profile.ext);
  }
});
```

---

## 6. 🏗️ 10,000 Pages Complete Architecture

### Directory Layout:
```
src/
├── constants/
│   └── registers.js       ← सम्पूर्ण एपको Memory Map
├── pages/
│   ├── LoginPage.jsx      (Reg 40001)
│   ├── DashboardPage.jsx  (Reg 40002)
│   ├── SettingsPage.jsx   (Reg 40003)
│   └── ... (१०,००० पेज)
├── components/
│   └── AppBar.jsx         (Reg 19000)
└── index.js               (Register 10 Router Listener)
```

### Central Register Constants File (`constants/registers.js`):
```javascript
export const REGS = {
  // ── Core System (0-999) ──
  STATUS:              1,
  SCREEN:              10,  // Active Page Router
  DRAWER:              11,
  THEME:               12,

  // ── UI Visuals (1000-9999) ──
  DIAL_BUFFER:         1000,
  LCD_LINE1:           1010,
  LCD_LINE2:           1011,

  // ── Database (10000-19999) ──
  CONTACTS_BASE:       10000,
  CALLLOG_BASE:        15000,
  USER_PROFILE:        19000,

  // ── Hardware Relays (20000-29999) ──
  RELAY_1:             20001,
  RELAY_2:             20002,
  SOLENOID_LOCK:       20010,

  // ── Custom Pages / Forms (40000-65535) ──
  PAGE_LOGIN:          40001,
  PAGE_SETTINGS:       40002,
  PAGE_CONTACT_NEW:    40003,
  PAGE_PAYMENT:        40004,
  // ... १०,००० पेजहरूको लागि क्रमशः नम्बर दिँदै जाने
};
```

---

## 7. ⚡ 24-Byte Binary Packet & Flags (`flags = 0x01`)

जब तपाईं `TitanMicroBus.emit()` मा Object पठाउनुहुन्छ, इन्जिनले आफैं २४-बाइट बाइनरी हेडरमा `flags = 0x01` (JSON Sub-code) सेट गर्छ:

```
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   Signature ('TB' = 0x5442)   |  Version(0x02)|   CMD Type    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          Sender Ext                           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          Target Ext                           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          Payload Len                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          Sequence No                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Session ID           | Flags (0x01)  | Checksum(0xAA)|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|              JSON Sub-Opcode Payload (Dynamic)                |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

---

## 8. 📊 Traditional vs Titan Bus Comparison

| कार्य | Traditional Stack | Titan Micro-Bus 🚀 |
| :--- | :--- | :--- |
| **Page Router** | React Router / `<Route>` | `TitanMicroBus.write(10, 'Page')` |
| **State Management** | Redux Toolkit / Zustand | `TitanMicroBus.write(reg, value)` |
| **Form Handling** | Multiple `useState` + Handlers | `write(40001, formDataJson)` |
| **Module Sync** | Actions + Reducers + Dispatch | `TitanMicroBus.subscribe(reg, fn)` |
| **Hardware Control** | छुट्टै WebSocket / MQTT | `TitanMicroBus.emit(300, CMD.RELAY_SET)` |
| **10,000 Pages State** | १०,००० Reducers 💀 | १०,००० Register Numbers (७.४ ms) ⚡ |

---

## 9. 🎯 Quick Reference Cheatsheet

```javascript
import { TitanMicroBus, TitanDeclarative, CMD } from '@dolphin/titan-bus';

// ── State Read & Write ─────────────────────────────────
TitanMicroBus.write(10, 'Dashboard');                  // Router switch
TitanMicroBus.write(40001, { user: 'Admin', pin: 1234 }); // Form write
const currentScreen = TitanMicroBus.read(10);          // 'Dashboard'
const formData = TitanMicroBus.read(40001);            // { user: 'Admin', pin: 1234 }

// ── Reactive Subscription ──────────────────────────────
const unsubscribe = TitanMicroBus.subscribe(10, (screen) => {
  console.log("Current Screen:", screen);
});
unsubscribe(); // Memory cleanup

// ── Binary Packet Dispatch (PBX & MCU) ─────────────────
TitanMicroBus.emit(0, CMD.REGISTER, { user: '101' });   // Server
TitanMicroBus.emit(300, CMD.RELAY_SET, { relayId: 1, state: 1 }); // NodeMCU

// ── Declarative String Actions ─────────────────────────
TitanDeclarative.executeAction('bus:screen:Dashboard'); // Page switch
TitanDeclarative.executeAction('bus:key:9');            // Keypad append
TitanDeclarative.executeAction('bus:relay:1:on');        // Relay toggle
```

---
*Created by Danphe Team — Proudly Engineered in Nepal 🇳🇵*
