#!/usr/bin/env python3
"""
Simple demo script to control ArmPi mini robot arm servos via serial protocol.
Matches the protocol used in the F Prime RobotArm component.
"""
import serial
import time

# CRC8 lookup table (same as in RobotArm.cpp)
CRC8_TABLE = [
    0,   94,  188, 226, 97,  63,  221, 131, 194, 156, 126, 32,  163, 253, 31,  65,
    157, 195, 33,  127, 252, 162, 64,  30,  95,  1,   227, 189, 62,  96,  130, 220,
    35,  125, 159, 193, 66,  28,  254, 160, 225, 191, 93,  3,   128, 222, 60,  98,
    190, 224, 2,   92,  223, 129, 99,  61,  124, 34,  192, 158, 29,  67,  161, 255,
    70,  24,  250, 164, 39,  121, 155, 197, 132, 218, 56,  102, 229, 187, 89,  7,
    219, 133, 103, 57,  186, 228, 6,   88,  25,  71,  165, 251, 120, 38,  196, 154,
    101, 59,  217, 135, 4,   90,  184, 230, 167, 249, 27,  69,  198, 152, 122, 36,
    248, 166, 68,  26,  153, 199, 37,  123, 58,  100, 134, 216, 91,  5,   231, 185,
    140, 210, 48,  110, 237, 179, 81,  15,  78,  16,  242, 172, 47,  113, 147, 205,
    17,  79,  173, 243, 112, 46,  204, 146, 211, 141, 111, 49,  178, 236, 14,  80,
    175, 241, 19,  77,  206, 144, 114, 44,  109, 51,  209, 143, 12,  82,  176, 238,
    50,  108, 142, 208, 83,  13,  239, 177, 240, 174, 76,  18,  145, 207, 45,  115,
    202, 148, 118, 40,  171, 245, 23,  73,  8,   86,  180, 234, 105, 55,  213, 139,
    87,  9,   235, 181, 54,  104, 138, 212, 149, 203, 41,  119, 244, 170, 72,  22,
    233, 183, 85,  11,  136, 214, 52,  106, 43,  117, 151, 201, 74,  20,  246, 168,
    116, 42,  200, 150, 21,  75,  169, 247, 182, 232, 10,  84,  215, 137, 107, 53
]

# Protocol constants
PWM_SERVO_CMD = 0x04
PWM_SET_POSITION_CMD = 0x01

# Servo IDs (matching RobotArm.fpp enum)
SERVO_CLAW = 1
SERVO_WRIST = 3
SERVO_ELBOW = 4
SERVO_SHOULDER = 5
SERVO_BASE = 6


def calculate_crc8(data):
    """Calculate CRC8 checksum for data."""
    crc = 0x00
    for byte in data:
        crc = CRC8_TABLE[crc ^ byte]
    return crc


def set_servo_position(ser, servo_id, pwm_value, duration_ms=300):
    """
    Send command to set servo position.

    Args:
        ser: Serial port object
        servo_id: Servo ID (1, 3, 4, 5, or 6)
        pwm_value: PWM pulse width in microseconds (500-2500)
        duration_ms: Movement duration in milliseconds (default: 300)
    """
    # Clamp values to safe ranges
    pwm_value = max(500, min(2500, pwm_value))
    duration_ms = max(0, min(30000, duration_ms))

    data_length = 7

    # Build the command packet
    buf = bytearray([
        0xAA,                           # Header byte 1
        0x55,                           # Header byte 2
        PWM_SERVO_CMD,                  # Function byte
        data_length,                    # Data packet length
        PWM_SET_POSITION_CMD,           # Command: set position
        duration_ms & 0xFF,             # Duration low byte
        (duration_ms >> 8) & 0xFF,      # Duration high byte
        1,                              # Number of servos (always 1)
        servo_id,                       # Servo ID
        pwm_value & 0xFF,               # PWM low byte (little endian)
        (pwm_value >> 8) & 0xFF,        # PWM high byte
    ])

    # Calculate checksum (skip the 2 header bytes)
    crc = calculate_crc8(buf[2:])
    buf.append(crc)

    # Send the command
    ser.write(buf)
    print(f"Servo {servo_id}: PWM={pwm_value}, Duration={duration_ms}ms")


def main():
    """Demo: Move each servo through different positions."""

    # Connect to serial port
    print("Connecting to /dev/ttyAMC0...")
    try:
        ser = serial.Serial(
            port='/dev/ttyAMC0',
            baudrate=9600,
            timeout=1
        )
        print(f"Connected! Baudrate: {ser.baudrate}")
    except Exception as e:
        print(f"Error opening serial port: {e}")
        print("Make sure the device is connected and you have permissions.")
        return

    try:
        print("\n=== ArmPi Mini Servo Demo ===\n")

        # Demo sequence: Move each servo
        servos = [
            (SERVO_CLAW, "Claw"),
            (SERVO_WRIST, "Wrist"),
            (SERVO_ELBOW, "Elbow"),
            (SERVO_SHOULDER, "Shoulder"),
            (SERVO_BASE, "Base")
        ]

        # Test each servo with 3 positions
        positions = [1500, 2000, 1000, 1500]  # Center, right, left, center

        for servo_id, servo_name in servos:
            print(f"\n--- Testing {servo_name} (Servo {servo_id}) ---")
            for pwm in positions:
                set_servo_position(ser, servo_id, pwm, duration_ms=500)
                time.sleep(1.0)  # IMPORTANT: 1 second delay between commands

        print("\n=== Demo complete! ===")
        print("Returning all servos to center position...\n")

        # Return all servos to center
        for servo_id, servo_name in servos:
            set_servo_position(ser, servo_id, 1500, duration_ms=500)
            time.sleep(1.0)

        print("Done!")

    except KeyboardInterrupt:
        print("\n\nDemo interrupted by user.")
    except Exception as e:
        print(f"\nError during demo: {e}")
    finally:
        ser.close()
        print("Serial port closed.")


if __name__ == "__main__":
    main()
