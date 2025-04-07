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
          position: U16
        }

        @ Set servo position
        async command SetPosition(
          servo: Servo
          position: U16
        )

        @ Servos position
        telemetry serverPosition: ServoStats

        @ Event indicating commanded servor and position
        event SetPosition(
          servo: Servo
          position: U16
        ) \
        severity activity high \
        format "Setting servo {} to position {}"

        @ Periodically request servo telemetry
        async input port run: Svc.Sched

        @ Receive telemetry
        async input port $recv: Drv.ByteStreamRecv

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
