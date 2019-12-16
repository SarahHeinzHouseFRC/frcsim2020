/**
 * Entry point
 */

public class Main
{
    public static void main(String[] args)
    {
        if (args.length < 1)
        {
            System.out.println("Usage: java Main <speed 0-1023>");
            return;
        }

        RobotAgent robotAgent = new RobotAgent(8000, "localhost", 4000);

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
