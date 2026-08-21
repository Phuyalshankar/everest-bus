'use strict';

import { TitanMicroBus, CMD } from './TitanMicroBus.js';

/**
 * 🌟 TitanDeclarative Engine
 * Resolves declarative string bindings:
 * - Text: "[bus:1000]" -> Replaced with live register value
 * - Actions: "bus:write:1000:5", "bus:dial:102", "bus:relay:1:on"
 */
export class TitanDeclarative {

  /**
   * Extract register ID from string like "[bus:1000]" or "bus:1000"
   */
  static extractRegisterId(str: string): number | null {
    if (!str) return null;
    const match = str.match(/\[?bus:(\d+)\]?/);
    return match ? parseInt(match[1], 10) : null;
  }

  /**
   * Execute declarative action string
   */
  static executeAction(actionStr: string): boolean {
    if (!actionStr || !actionStr.startsWith('bus:')) return false;

    const parts = actionStr.split(':');
    const verb = parts[1]; // write, dial, relay, key, screen, show

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
        TitanMicroBus.write(1000, updated); // Update dial buffer
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
          TitanMicroBus.write(10, 'ActiveCall'); // Change screen to ActiveCall
        }
        return true;
      }

      case 'relay': {
        const relayId = parseInt(parts[2], 10);
        const state = parts[3] === 'on' || parts[3] === '1' ? 1 : 0;
        const pulse = parts[4] ? parseInt(parts[4], 10) : 0;
        TitanMicroBus.write(20000 + relayId, state);
        TitanMicroBus.emit(300 /* MCU */, CMD.RELAY_SET, { relayId, state, pulse });
        return true;
      }

      case 'screen': {
        const screenName = parts[2];
        TitanMicroBus.write(10, screenName);
        return true;
      }

      case 'show': {
        const text = parts.slice(2).join(':');
        TitanMicroBus.write(1010, text); // 16x2 LCD
        return true;
      }

      default:
        return false;
    }
  }
}
