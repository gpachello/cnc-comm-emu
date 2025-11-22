# CNC Communications Emulator (cnc-comm-emu)

A portable **CNC communications emulator** designed for developers building applications that interact with real CNC machines through **FOCAS** and **MTConnect**.  

> [!NOTE]
> This project does **not** execute G/M-code. It focuses exclusively on **communication-layer behavior**, simulating how a CNC responds to protocol requests.

---

## 🔧 Purpose

Modern CNC software development often requires access to real machines to test:

- MTConnect data collection  
- FOCAS-based queries  
- Monitoring and analytics tools  
- Custom dashboards, MES/SCADA integrations  
- Industrial connectivity pipelines  

Real machines are expensive, scarce, and not always available during development.  
**cnc-comm-emu acts as a virtual CNC device**, allowing developers to test their software against realistic protocol responses.

---

## 📡 Position in the MTConnect Chain

This emulator implements the **last link** in the MTConnect communication path, the **CNC device itself**.

```
Subscriber (HTTP client)
⇅ TCP 5000
MTConnect Agent
⇅ TCP 7878
MTConnect Adapter
⇅ TCP 8193
CNC Device (FOCAS) ← (this project)
```

The emulator listens on **TCP 8193** and responds to FOCAS Ethernet requests as a FANUC controller would.

---

## 🏗️ Current Scope

### ✔ Implemented / Initial Version
- FANUC-style CNC device emulator  
- FOCAS over TCP (default port **8193**)  
- Basic communication loop and mock responses  

### ✖ Not included
- No G-code or M-code execution  
- No machine motion simulation  
- No toolpath interpretation  

---

## 🚀 Getting Started

### Build the image

```bash
docker build -t cnc-comm-emu .
```

### Run with Docker Compose

Start it:
```bash
docker compose up -d
```

Your emulator will now accept FOCAS TCP connections on **localhost:8193**

---

### Roadmap (short)

Future expansions may include:

* Additional CNC brands (HAAS, DMG-MORI, Siemens, Mazak, Okuma)
* More detailed FOCAS response models
* MTConnect Adapter and Agent emulation modules
* Randomized or profile-based machine states
* Fault/alarm injection
