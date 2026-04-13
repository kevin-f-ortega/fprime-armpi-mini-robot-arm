# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an **F´ (F Prime) v4.2.0** project for controlling an ArmPi mini robot arm. F´ is NASA/JPL's component-driven embedded systems framework designed for spaceflight applications.

The project contains a single custom component (`RobotArm`) that communicates with the ArmPi hardware via serial protocol (PWM servo commands). The component provides commands to set servo positions and emits telemetry for each servo's current position.

## Build Commands

All build commands use `fprime-util`, F´'s command-line utility. The Python virtual environment (`fprime-venv`) must be activated or will be automatically used via CMake presets.

### Generate Build System
```bash
cd ArmPiMiniRobotArm/Deployment
fprime-util generate
```

### Build Deployment
```bash
cd ArmPiMiniRobotArm/Deployment
fprime-util build
```

### Build Individual Component
```bash
cd ArmPiMiniRobotArm/Components/RobotArm
fprime-util build
```

### Run Application with Ground System
```bash
cd ArmPiMiniRobotArm/Deployment
fprime-gds
```

This starts both the flight software binary and the F´ Ground Data System (GDS) web interface.

### Run GDS Without Starting Application
```bash
cd ArmPiMiniRobotArm/Deployment
fprime-gds --no-app
```

Then manually run the binary:
```bash
cd ArmPiMiniRobotArm/Deployment/build-artifacts/<platform>/bin/
./Deployment -a 127.0.0.1 -p 50000
```

### CMake Presets
Available presets in `CMakePresets.json`:
- `fprime` - Release build
- `fprime-debug` - Debug build
- `fprime-ut` - Debug build with unit tests enabled
- Ninja variants: append `-ninja` to any preset name

Note: Unit tests are currently disabled in `settings.ini` via `FPRIME_ENABLE_FRAMEWORK_UTS=OFF` and `FPRIME_ENABLE_AUTOCODER_UTS=OFF`.

## Architecture

### Deployment Structure
The deployment (`ArmPiMiniRobotArm/Deployment`) uses F´'s **core subtopologies** for modular architecture:

- **CdhCore**: Command & Data Handling (command dispatching, event logging, telemetry, health monitoring)
- **ComCcsds**: CCSDS Communication Subsystem (CCSDS protocol, uplink/downlink, frame processing)
- **FileHandling**: File transfer, parameter database, command sequencing
- **DataProducts**: Data product cataloging and storage

These subtopologies are imported in `topology.fpp` and provide standard flight software capabilities without custom implementation.

### FPP Files (F´ Prime)
Component interfaces are defined in `.fpp` files using F´'s modeling language:

- `topology.fpp` - Defines component instances, port connections, and rate group assignments
- `instances.fpp` - Defines component instances with base IDs, queue sizes, stack sizes, priorities
- `RobotArm.fpp` - Component interface definition (commands, events, telemetry, ports)

**Base ID Convention**: 8-digit hex format `0xDSSCCxxx`
- D = Deployment digit (1 for this deployment)
- SS = Subtopology (00 for main, 01-05 for subtopologies)
- CC = Component number
- xxx = Reserved for internal component items

### RobotArm Component
Located at `ArmPiMiniRobotArm/Components/RobotArm/`

**Servo Enumeration** (defined in `RobotArm.fpp`):
- CLAW = 1
- WRIST = 3
- ELBOW = 4
- SHOULDER = 5
- BASE = 6

**Key Ports**:
- `$recv` - Receives serial data from arm (async input, ByteStreamRecv)
- `$send` - Sends commands to arm (output, ByteStreamSend)
- `run` - Periodic scheduling port for telemetry requests
- Standard AC ports (commands, events, telemetry, time)

**Commands**:
- `SetPosition(servo, position)` - Sets a servo to a specific PWM position (300ms duration)

**Telemetry Channels**:
- `clawPosition`, `wristPosition`, `elbowPosition`, `shoulderPosition`, `basePosition` - Each reports ServoStats struct

**Protocol Implementation**:
The component implements a custom serial protocol with CRC-8 checksums. When servo position feedback is received (command byte `PWM_SERVO_CMD`), the component parses the 8-byte message and emits telemetry for the corresponding servo.

## Rate Groups
Three rate groups drive periodic execution:
- **rateGroup1** (priority 43): High-rate telemetry, file downlink, system resources, communication queue
- **rateGroup2** (priority 42): Command sequencer
- **rateGroup3** (priority 41): Health checks, buffer management, data products

## Key Files
- `project.cmake` - Registers project-wide components
- `settings.ini` - F´ project configuration (framework path, default CMake options)
- `CMakeLists.txt` - Top-level CMake configuration that includes F´ framework
- `lib/fprime/` - F´ framework submodule (v4.2.0)

## Development Workflow

1. **Activate virtual environment** (if not using CMake presets):
   ```bash
   source fprime-venv/bin/activate
   ```

2. **Modify component interface**: Edit `.fpp` files, then regenerate/rebuild

3. **Modify component implementation**: Edit `.cpp` files, then rebuild

4. **Autocoding**: F´ automatically generates C++ from `.fpp` files during build. Generated files appear in `build-artifacts/` and should not be edited directly.

5. **Adding new components**: Create directory under `ArmPiMiniRobotArm/Components/`, add `CMakeLists.txt`, `.fpp`, and `.cpp` files. Register in parent `CMakeLists.txt`.

## Important Notes

- **Serial Communication Timing**: The RobotArm requires a 1-second sleep between commands. Rapid commands cause servo misbehavior requiring manual recovery.

- **Deployment.old/**: Contains previous deployment structure before v4.2.0 upgrade. Used as reference during migration to subtopology architecture.

- **Testing**: Unit testing is currently disabled project-wide. To enable, modify `settings.ini` and uncomment `UT_SOURCE_FILES` section in component `CMakeLists.txt` files.

- **Cross-compilation**: F´ supports cross-compilation via CMake toolchain files in `lib/fprime/cmake/toolchain/` (e.g., `raspberrypi.cmake`, `arm-hf-linux.cmake`).
