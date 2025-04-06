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
    // TODO
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
  }

}
