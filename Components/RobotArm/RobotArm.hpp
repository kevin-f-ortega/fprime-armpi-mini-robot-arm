// ======================================================================
// \title  RobotArm.hpp
// \author ortega
// \brief  hpp file for RobotArm component implementation class
// ======================================================================

#ifndef Components_RobotArm_HPP
#define Components_RobotArm_HPP

#include "Components/RobotArm/RobotArmComponentAc.hpp"

namespace Components {

class RobotArm : public RobotArmComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct RobotArm object
    RobotArm(const char* const compName  //!< The component name
    );

    //! Destroy RobotArm object
    ~RobotArm();

    PRIVATE :

        static constexpr U8 PWM_SERVO_CMD = 0x04;
    static constexpr U8 PWM_SET_POSITION_CMD = 0x01;
    static constexpr U8 PWM_READ_POSITION_CMD = 0x05;

    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for recv
    //!
    //! Receive telemetry
    void recv_handler(FwIndexType portNum,  //!< The port number
                      Fw::Buffer& recvBuffer,
                      const Drv::RecvStatus& recvStatus) override;

    //! Handler implementation for run
    //!
    //! Periodically request servo telemetry
    void run_handler(FwIndexType portNum,  //!< The port number
                     U32 context           //!< The call order
                     ) override;

    PRIVATE :

        // ----------------------------------------------------------------------
        // Handler implementations for commands
        // ----------------------------------------------------------------------

        //! Handler implementation for command SetPosition
        //!
        //! Set servo position
        void
        SetPosition_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                               U32 cmdSeq,           //!< The command sequence number
                               Components::RobotArm_Servo servo,
                               U16 position) override;

    PRIVATE :

        // ----------------------------------------------------------------------
        // Helper functions
        // ----------------------------------------------------------------------
        U8
        checksumCrc8(const U8* const data, const U32 dataSize);

    Drv::SendStatus pwmServoSetPosition(const U16 durationMs, const RobotArm_Servo servo, const U16 pwm);

    Drv::SendStatus readServoPosition(const RobotArm_Servo servo);
};

}  // namespace Components

#endif
