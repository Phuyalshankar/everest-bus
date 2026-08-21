'use strict';

/**
 * 🚀 TITAN MICRO-BUS (TypeScript / JavaScript Core)
 * ══════════════════════════════════════════════════════════════════
 * Ultra-fast 2-Byte (UInt16: 0 - 65535) Memory Register Highway &
 * 24-Byte Binary Packet Router for UI, DB, Audio, and Hardware.
 */

export const TITAN_SIGNATURE = 0x5442; // 'TB'
export const TITAN_VERSION = 0x02;
export const TITAN_HEADER_SIZE = 24;

export enum CMD {
  REGISTER       = 0x08,
  REGISTER_ACK   = 0x09,
  INVITE         = 0x10,
  ACCEPT         = 0x11,
  REJECT         = 0x12,
  HANGUP         = 0x13,
  AUDIO_FRAME    = 0x14,
  VIDEO_FRAME    = 0x15,
  CHAT_MESSAGE   = 0x20,
  KEYPAD_EVENT   = 0x21,
  DISPLAY_WRITE  = 0x22,
  RELAY_SET      = 0x23,
  SENSOR_STREAM  = 0x24,
  HEARTBEAT      = 0x30,
  CUSTOM_ACTION  = 0x40,
}

export type BusListener = (value: any, registerId: number) => void;
export type CmdHandler = (cmd: number, senderId: number, payload: Buffer | any) => void;

class TitanMicroBusEngine {
  private registers: Map<number, any> = new Map();
  private listeners: Map<number, Set<BusListener>> = new Map();
  private cmdHandlers: Map<number, Set<CmdHandler>> = new Map();
  private seqNo: number = 1;
  public myNodeId: number = 101;

  constructor() {
    this.initDefaults();
  }

  private initDefaults(): void {
    this.write(1, 'ONLINE'); // REG_BUS_STATUS
    this.write(10, 'Home');  // REG_ACTIVE_SCREEN
    this.write(1000, '');    // REG_KEYPAD_DIAL_BUFFER
  }

  // 1. 2-Byte Register Read / Write (0 - 65,535)
  public write(reg: number, value: any, silent = false): void {
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

  public read(reg: number, defaultValue: any = null): any {
    return this.registers.has(reg) ? this.registers.get(reg) : defaultValue;
  }

  // 2. Register Change Subscription
  public subscribe(reg: number, listener: BusListener): () => void {
    if (!this.listeners.has(reg)) {
      this.listeners.set(reg, new Set());
    }
    this.listeners.get(reg)!.add(listener);

    // Initial trigger
    if (this.registers.has(reg)) {
      listener(this.registers.get(reg), reg);
    }

    return () => {
      this.listeners.get(reg)?.delete(listener);
    };
  }

  // 3. Command Packing & Dispatch
  public emit(targetId: number, cmd: number, payload: any = null, isJson = false): Buffer {
    let payloadBuf: Buffer;
    let flags = 0;

    if (Buffer.isBuffer(payload)) {
      payloadBuf = payload;
    } else if (typeof payload === 'string') {
      payloadBuf = Buffer.from(payload, 'utf8');
    } else if (payload !== null && typeof payload === 'object') {
      payloadBuf = Buffer.from(JSON.stringify(payload), 'utf8');
      flags = 1; // JSON mode
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
    frame.writeUInt16BE(0, 20); // Session ID
    frame.writeUInt8(flags, 22);
    frame.writeUInt8(0xAA, 23); // Checksum

    if (payloadBuf.length > 0) {
      payloadBuf.copy(frame, TITAN_HEADER_SIZE);
    }

    // Local dispatch
    const handlers = this.cmdHandlers.get(cmd);
    if (handlers) {
      handlers.forEach(h => {
        try { h(cmd, this.myNodeId, payload); } catch (e) { console.error(e); }
      });
    }

    return frame;
  }

  public onCommand(cmd: number, handler: CmdHandler): () => void {
    if (!this.cmdHandlers.has(cmd)) {
      this.cmdHandlers.set(cmd, new Set());
    }
    this.cmdHandlers.get(cmd)!.add(handler);
    return () => {
      this.cmdHandlers.get(cmd)?.delete(handler);
    };
  }

  // 4. Ingest Raw TCP Stream Bytes
  public ingestBytes(chunk: Buffer): void {
    if (chunk.length < TITAN_HEADER_SIZE) return;
    const sig = chunk.readUInt16BE(0);
    if (sig !== TITAN_SIGNATURE) return;

    const cmd = chunk.readUInt8(3);
    const sender = chunk.readInt32BE(4);
    const pLen = chunk.readInt32BE(12);
    const flags = chunk.readUInt8(22);

    let payload: any = null;
    if (pLen > 0 && chunk.length >= TITAN_HEADER_SIZE + pLen) {
      const rawPayload = chunk.subarray(TITAN_HEADER_SIZE, TITAN_HEADER_SIZE + pLen);
      if (flags === 1) {
        try { payload = JSON.parse(rawPayload.toString('utf8')); } catch { payload = rawPayload; }
      } else {
        payload = rawPayload;
      }
    }

    const handlers = this.cmdHandlers.get(cmd);
    if (handlers) {
      handlers.forEach(h => h(cmd, sender, payload));
    }
  }
}

export const TitanMicroBus = new TitanMicroBusEngine();
