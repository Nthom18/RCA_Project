#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    system("touch server_signal.txt");
    system("> server_signal.txt");
    system("echo 0 > server_signal.txt");

    for(int i = 1; i < 20; i++)
    {
        std::string world = "smallworld" + std::to_string(i) + ".world";
        std::string command = "../../Gazebo/gazebo_server.sh ../../Gazebo/smallTestWorlds/" + world + " &";
        system(command.c_str());
        system("../CMake/robot_control &");

        // Time for server and program startup
        sleep(10);
        system("echo 1 > server_signal.txt");

        // Time for program to execute
        sleep(1);
        system("> server_signal.txt");
        system("echo 0 > server_signal.txt");
        
        // Time to finish program before server shutdown
        sleep(1);
        system("pkill gzserver");
        system("> server_signal.txt");
        system("echo 0 > server_signal.txt");
        system("pkill robot_control");
    }

    system("pkill robot_control");

    return 0;
}
