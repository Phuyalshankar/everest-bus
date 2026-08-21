'use strict';

/**
 * 🚀 TITAN MICRO-BUS (Universal JS / Node / Dolphin Runtime)
 * ═════════════════════════════════════════════════════════════════════
 * 2-Byte UInt16 (0 - 65,535) Memory-Mapped Register Bus &
 * 24-Byte Binary Packet Highway.
 */

const TITAN_SIGNATURE = 0x5442; // 'TB'
const TITAN_VERSION = 0x02;
const TITAN_HEADER_SIZE = 24;

const CMD = {
  REGISTER:       0x08,
  REGISTER_ACK:   0x09,
  INVITE:         0x10,
  ACCEPT:         0x11,
  REJECT:         0x12,
  HANGUP:         0x13,
  AUDIO_FRAME:    0x14,
  KEYPAD_EVENT:   0x21,
  DISPLAY_WRITE:  0x22,
  RELAY_SET:      0x23,
  SENSOR_STREAM:  0x24,
  HEARTBEAT:      0x30,
  CUSTOM_ACTION:  0x40,
};

class TitanMicroBusEngine {
  constructor() {
    this.registers = new Map();
    this.listeners = new Map();
    this.cmdHandlers = new Map();
    this.seqNo = 1;
    this.myNodeId = 101;
    this.initDefaults();
  }

  initDefaults() {
    this.write(1, 'ONLINE');
    this.write(10, 'Home');
    this.write(1000, '');
  }

  write(reg, value, silent = false) {
    this.registers.set(reg, value);
    if (!silent) {
      const subs = this.listeners.get(reg);
      if (subs) {
        subs.forEach(listener => {
          try { listener(value, reg); } catch (e) { console.error(e); }
        });
      }
    }
  }

  read(reg, defaultValue = null) {
    return this.registers.has(reg) ? this.registers.get(reg) : defaultValue;
  }

  subscribe(reg, listener) {
    if (!this.listeners.has(reg)) {
      this.listeners.set(reg, new Set());
    }
    this.listeners.get(reg).add(listener);

    if (this.registers.has(reg)) {
      listener(this.registers.get(reg), reg);
    }

    return () => {
      this.listeners.get(reg)?.delete(listener);
    };
  }

  emit(targetId, cmd, payload = null, isJson = false) {
    let payloadBuf;
    let flags = 0;

    if (Buffer.isBuffer(payload)) {
      payloadBuf = payload;
    } else if (typeof payload === 'string') {
      payloadBuf = Buffer.from(payload, 'utf8');
    } else if (payload !== null && typeof payload === 'object') {
      payloadBuf = Buffer.from(JSON.stringify(payload), 'utf8');
      flags = 1;
    } else if (typeof payload === 'number') {
      payloadBuf = Buffer.alloc(4);
      payloadBuf.writeInt32BE(payload, 0);
    } else {
      payloadBuf = Buffer.alloc(0);
    }

    const frame = Buffer.alloc(TITAN_HEADER_SIZE + payloadBuf.length);
    frame.writeUInt16BE(TITAN_SIGNATURE, 0);
    frame.writeUInt8(TITAN_VERSION, 2);
    frame.writeUInt8(cmd, 3);
    frame.writeInt32BE(this.myNodeId, 4);
    frame.writeInt32BE(targetId, 8);
    frame.writeInt32BE(payloadBuf.length, 12);
    frame.writeInt32BE(this.seqNo++, 16);
    frame.writeUInt16BE(0, 20);
    frame.writeUInt8(flags, 22);
    frame.writeUInt8(0xAA, 23);

    if (payloadBuf.length > 0) {
      payloadBuf.copy(frame, TITAN_HEADER_SIZE);
    }

    const handlers = this.cmdHandlers.get(cmd);
    if (handlers) {
      handlers.forEach(h => {
        try { h(cmd, this.myNodeId, payload); } catch (e) { console.error(e); }
      });
    }

    return frame;
  }

  onCommand(cmd, handler) {
    if (!this.cmdHandlers.has(cmd)) {
      this.cmdHandlers.set(cmd, new Set());
    }
    this.cmdHandlers.get(cmd).add(handler);
    return () => {
      this.cmdHandlers.get(cmd)?.delete(handler);
    };
  }
}

class TitanDeclarative {
  static extractRegisterId(str) {
    if (!str) return null;
    const match = str.match(/\[?bus:(\d+)\]?/);
    return match ? parseInt(match[1], 10) : null;
  }

  static executeAction(actionStr) {
    if (!actionStr || !actionStr.startsWith('bus:')) return false;

    const parts = actionStr.split(':');
    const verb = parts[1];

    switch (verb) {
      case 'write': {
        const reg = parseInt(parts[2], 10);
        const val = parts.slice(3).join(':');
        TitanMicroBus.write(reg, val);
        return true;
      }

      case 'key': {
        const key = parts[2];
        const current = TitanMicroBus.read(1000, '');
        const updated = current + key;
        TitanMicroBus.write(1000, updated);
        TitanMicroBus.emit(101, CMD.KEYPAD_EVENT, key);
        return true;
      }

      case 'backspace': {
        const current = TitanMicroBus.read(1000, '');
        const updated = current.length > 0 ? current.slice(0, -1) : '';
        TitanMicroBus.write(1000, updated);
        return true;
      }

      case 'dial': {
        const ext = parts[2] || TitanMicroBus.read(1000, '');
        if (ext) {
          TitanMicroBus.emit(101, CMD.INVITE, ext);
          TitanMicroBus.write(10, 'ActiveCall');
        }
        return true;
      }

      case 'relay': {
        const relayId = parseInt(parts[2], 10);
        const state = parts[3] === 'on' || parts[3] === '1' ? 1 : 0;
        const pulse = parts[4] ? parseInt(parts[4], 10) : 0;
        TitanMicroBus.write(20000 + relayId, state);
        TitanMicroBus.emit(300, CMD.RELAY_SET, { relayId, state, pulse });
        return true;
      }

      case 'screen': {
        const screenName = parts[2];
        TitanMicroBus.write(10, screenName);
        return true;
      }

      case 'show': {
        const text = parts.slice(2).join(':');
        TitanMicroBus.write(1010, text);
        return true;
      }

      default:
        return false;
    }
  }
}

const TitanMicroBus = new TitanMicroBusEngine();

module.exports = {
  TitanMicroBus,
  TitanDeclarative,
  CMD,
  TITAN_SIGNATURE,
  TITAN_VERSION,
  TITAN_HEADER_SIZE
};
