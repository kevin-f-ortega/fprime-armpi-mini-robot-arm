// ======================================================================
// \title  RobotArm.cpp
// \author ortega
// \brief  cpp file for RobotArm component implementation class
// ======================================================================

#include "Components/RobotArm/RobotArm.hpp"

namespace Components {

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
    // TODO
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
	  // TODO: Add evr here
    static constexpr U16 durationMs = 300;
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
