// ======================================================================
// \title  RobotArm.cpp
// \author ortega
// \brief  cpp file for RobotArm component implementation class
// ======================================================================

#include "Components/RobotArm/RobotArm.hpp"

namespace Components {

  static const U8 CRC8_TABLE[256] = {
    0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
    157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
    35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
    190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
    70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
    219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
    101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
    248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
    140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
    17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
    175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
    50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
    202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
    87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
    233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
    116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
  };

  // ----------------------------------------------------------------------
  // Component construction and destruction
  // ----------------------------------------------------------------------

  RobotArm ::
    RobotArm(const char* const compName) :
      RobotArmComponentBase(compName)
  {

  }

  RobotArm ::
    ~RobotArm()
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for typed input ports
  // ----------------------------------------------------------------------

  void RobotArm ::
    recv_handler(
        FwIndexType portNum,
        Fw::Buffer& recvBuffer,
        const Drv::RecvStatus& recvStatus
    )
  {
    //TODO
    this->deallocate_out(0, recvBuffer);
  }

  void RobotArm ::
    run_handler(
        FwIndexType portNum,
        U32 context
    )
  {
    // TODO
  }

  // ----------------------------------------------------------------------
  // Handler implementations for commands
  // ----------------------------------------------------------------------

  void RobotArm ::
    SetPosition_cmdHandler(
        FwOpcodeType opCode,
        U32 cmdSeq,
        Components::RobotArm_Servo servo,
        U16 position
    )
  {
    static constexpr U16 durationMs = 300;
    this->log_ACTIVITY_HI_SetPosition(servo, position);
    Drv::SendStatus status = this->pwmServoSetPosition(durationMs, servo, position);
    Fw::CmdResponse response = (status == Drv::SendStatus::SEND_OK) ? Fw::CmdResponse::OK : Fw::CmdResponse::EXECUTION_ERROR;

    this->cmdResponse_out(opCode, cmdSeq, response);
  }

  // ----------------------------------------------------------------------
  // Helper functions
  // ----------------------------------------------------------------------

  U8 RobotArm::checksumCrc8(const U8 *const data, const U32 dataSize)
  {
    FW_ASSERT(data != nullptr);
    U8 crc = 0x0;
    for(U32 i = 0; i < dataSize; i++) {
      crc = CRC8_TABLE[crc ^ data[i]];
    }
    return crc;
  }

  Drv::SendStatus RobotArm::pwmServoSetPosition(const U16 durationMs, const RobotArm_Servo servo, const U16 pwm) {
    static constexpr U16 MAX_DATA_SIZE_BYTES = 12;
    static constexpr U8 PWM_SERVO_CMD = 4;
    U8 dataLength = 7;
    U8 buf[MAX_DATA_SIZE_BYTES];
    buf[0] = 0xAA; // Header
    buf[1] = 0x55; // Header
    buf[2] = PWM_SERVO_CMD; // Function
    buf[3] = dataLength; // Data packet length
    buf[4] = 0x01; // First byte of data
    buf[5] = durationMs & 0xFF;
    buf[6] = (durationMs >> 8) & 0xFF;
    buf[7] = 1; // number of positions
    buf[8] = static_cast<U8>(servo.e); // servo ID
    buf[9] = pwm & 0xFF; // little-endian byte of U16 pwm
    buf[10] = (pwm >> 8) & 0xFF;
    // When calculating the checksum, omit the 2 header bytes
    U8 crc = this->checksumCrc8(buf+2, (dataLength+2));
    buf[11] = crc;
    Fw::Buffer buffer(buf, MAX_DATA_SIZE_BYTES);
    Drv::SendStatus status = this->send_out(0, buffer);
    return status;
  }
}
