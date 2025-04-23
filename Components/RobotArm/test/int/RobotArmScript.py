import time

from fprime_gds.common.testing_fw import predicates

def test_cmd_no_op(fprime_test_api):
    """Test command CMD_NO_OP

    Test that CMD_NO_OP can be sent and return without and errors
    """
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.cmdDisp.CMD_NO_OP")
    # reset positions
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["CLAW", 1700])
    time.sleep(1)
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["BASE", 1500])
    time.sleep(1)
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["SHOULDER", 1500])
    time.sleep(1)
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["ELBOW", 1500])
    time.sleep(1)
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["WRIST", 1500])
    time.sleep(1)

    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["CLAW", 2500])
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["SHOULDER", 2000])
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["ELBOW", 2500])
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["WRIST", 1500])

    time.sleep(1)
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["SHOULDER", 2100])
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["CLAW", 1400])

    time.sleep(1)

    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["SHOULDER", 1500])
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["BASE", 500])
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["ELBOW", 1700])
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["WRIST", 500])
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["SHOULDER", 2400])
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["CLAW", 2300])
    time.sleep(1)

    # reset positions
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["CLAW", 1700])
    time.sleep(1)
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["SHOULDER", 1500])
    time.sleep(1)
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["BASE", 1500])
    time.sleep(1)
    
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["ELBOW", 1500])
    time.sleep(1)
    fprime_test_api.send_and_assert_command("ArmPiMiniRobotArm.robotArm.SetPosition", ["WRIST", 1500])
    

#elbow at 1700
#wrist 500
#shoulder at 2400

