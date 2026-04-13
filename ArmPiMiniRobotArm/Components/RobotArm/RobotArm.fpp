module Components {
    @ Component to control the ArmPi mini Robot Arm
    active component RobotArm {

        enum Servo {
          CLAW = 1
          WRIST = 3
          ELBOW = 4
          SHOULDER = 5
          BASE = 6
        }

        struct ServoStats {
          servo: Servo
          angle: F32 @< Servo angle in degrees (0.0-180.0)
        }

        @ Set servo position by angle
        async command SetPosition(
          servo: Servo
          angle: F32 @< Servo angle in degrees (0.0-180.0)
        )

        @ Servo angles in degrees
        telemetry clawPosition: ServoStats
        telemetry wristPosition: ServoStats
        telemetry elbowPosition: ServoStats
        telemetry shoulderPosition: ServoStats
        telemetry basePosition: ServoStats

        @ Event indicating commanded servo and angle
        event SetPosition(
          servo: Servo
          angle: F32
        ) \
        severity activity high \
        format "Setting servo {} to angle {} degrees"

        @ Event indicating we received an unknown servo
        event UnknownServo(
          servo: Servo
        ) \
        severity warning low \
        format "Received unknown servo {}"

        @ Periodically request servo telemetry
        async input port run: Svc.Sched

        @ Receive telemetry
        async input port $recv: Drv.ByteStreamData

        @ Deallocate received buffer
        output port deallocate: Fw.BufferSend

        @ Send out arm data
        output port $send: Drv.ByteStreamSend

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

    }
}
