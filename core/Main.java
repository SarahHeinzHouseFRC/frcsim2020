/**
 * Entry point
 */

public class Main
{
    public static void main(String[] args)
    {
        RobotAgent robotAgent = new RobotAgent(8000, 4000);

        System.out.println(args.length);
        int cmd = Integer.parseInt(args[0]);
        while (true)
        {
            // Send robot commands
            robotAgent.commands.elevatorMotorSpeed = cmd;
            robotAgent.txRobotCommands();

            // Receive robot state
            robotAgent.rxRobotState();
            RobotState state = robotAgent.state;
        }
    }
}
