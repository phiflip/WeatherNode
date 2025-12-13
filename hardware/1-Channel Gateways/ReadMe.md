# CubeCell LoRaWAN with 1-Channel Gateways

## Scope
This document describes the required **CubeCell Arduino code adjustments** when operating a node with a **1-channel LoRaWAN gateway**.

1-channel gateways are typically based on a *Single Channel Packet Forwarder*. They are **not LoRaWAN-compliant** and should only be used for testing, teaching, or controlled lab setups.

---

## Background

A 1-channel gateway can only receive uplinks on **one fixed frequency channel**.

By default, CubeCell LoRaWAN nodes use multiple channels (EU868: channels 0–7). If this default behavior is not restricted, most uplinks will be sent on channels the gateway cannot hear.

The solution is:
- Restrict the node to **exactly one channel** using a channel mask
- Disable **ADR (Adaptive Data Rate)** to prevent the network from changing data rate or spreading factor

---

## 1) Channel Mask Configuration

Enable **only Channel 0** in the LoRaWAN channel mask.

Add or modify the following line in your CubeCell Arduino sketch:

```cpp
/* LoRaWAN Channel Mask (default channels 0–7) */
uint16_t userChannelsMask[6] = {
  0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
```

### Explanation

- `0x0001` activates **only channel 0**
- All other channels are disabled

Important:
- Your **gateway must listen on channel 0** (EU868 default: 868.1 MHz)
- If your gateway uses a different channel, the bitmask must be adapted accordingly

---

## 2) Disable ADR (Adaptive Data Rate)

ADR must be disabled so the node does not change its data rate or spreading factor.

### Arduino IDE

```
Tools → ADR → false
```

### In Code

```cpp
bool loraWanAdr = false;  // ADR is disabled
```

With ADR disabled, the node will transmit with a fixed data rate. In most standard EU868 CubeCell configurations, this results in **SF7** operation.

---

## 3) Required Conditions

Before deploying:

- Gateway frequency matches the active channel in `userChannelsMask`
- ADR is disabled in **Tools** and **code**
- Node is reflashed after any configuration change

---

## Limitations

- 1-channel gateways do **not** support proper downlinks
- Join reliability may be reduced
- Not suitable for production deployments

For real-world applications, always use a **full multi-channel LoRaWAN gateway**.

---

## Minimal Configuration Summary

```cpp
uint16_t userChannelsMask[6] = { 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 };
bool loraWanAdr = false;
```

---

## Tested Use Case

- CubeCell HTCC-AB02 / AB01
- EU868 region
- Single-channel gateway (channel 0)
- TTN / private LoRaWAN backend

---

## Disclaimer

This setup is intended for **education, testing, and demos only**.


