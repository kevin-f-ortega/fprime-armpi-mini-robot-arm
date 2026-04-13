// ======================================================================
// \title  RobotArm.hpp
// \author ortega
// \brief  hpp file for RobotArm component implementation class
// ======================================================================

#ifndef Components_RobotArm_HPP
#define Components_RobotArm_HPP

#include "ArmPiMiniRobotArm/Components/RobotArm/RobotArmComponentAc.hpp"

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

    private :

    static constexpr U8 PWM_SERVO_CMD = 0x04;
    static constexpr U8 PWM_SET_POSITION_CMD = 0x01;
    static constexpr U8 PWM_READ_POSITION_CMD = 0x05;
    static constexpr U8 PWM_READ_POSITION_DATA_LEN = 2;

    // ----------------------------------------------------------------------
    // Handler implementations for typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for recv
    //!
    //! Receive telemetry
    void recv_handler(FwIndexType portNum,  //!< The port number
                      Fw::Buffer& recvBuffer,
                      const Drv::ByteStreamStatus& recvStatus) override;

    //! Handler implementation for run
    //!
    //! Periodically request servo telemetry
    void run_handler(FwIndexType portNum,  //!< The port number
                     U32 context           //!< The call order
                     ) override;

    private :

        // ----------------------------------------------------------------------
        // Handler implementations for commands
        // ----------------------------------------------------------------------

        //! Handler implementation for command SetPosition
        //!
        //! Set servo position by angle
        void
        SetPosition_cmdHandler(FwOpcodeType opCode,  //!< The opcode
                               U32 cmdSeq,           //!< The command sequence number
                               Components::RobotArm_Servo servo,
                               F32 angle) override;

    private :

        // ----------------------------------------------------------------------
        // Helper functions
        // ----------------------------------------------------------------------
        U8
        checksumCrc8(const U8* const data, const U32 dataSize);

    //! Convert angle (0-180 degrees) to PWM pulse width (500-2500 microseconds)
    U16 angleToPwm(const F32 angle);

    //! Convert PWM pulse width (500-2500 microseconds) to angle (0-180 degrees)
    F32 pwmToAngle(const U16 pwm);

    Drv::ByteStreamStatus pwmServoSetPosition(const U16 durationMs, const RobotArm_Servo servo, const U16 pwm);

    Drv::ByteStreamStatus readServoPosition(const RobotArm_Servo servo);
};

}  // namespace Components

#endif
