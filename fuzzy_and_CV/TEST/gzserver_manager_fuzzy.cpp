#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TEST_NR 4

int main()
{
    system("touch server_signal.txt");
    system("> server_signal.txt");
    system("echo 0 > server_signal.txt");

    for(int i = 3; i < TEST_NR; i++)
    {
        std::string world = "bigworldNoMarbles" + std::to_string(i) + ".world";
        std::string command = "../../Gazebo/gazebo_server.sh ../../Gazebo/bigTestWorlds/" + world + " &";
        system(command.c_str());
        
        
        system("echo 1 > server_signal.txt");
        system("../CMake/robot_control &");
        
        // Time for program to execute
        sleep(480);
        system("> server_signal.txt");
        system("echo 0 > server_signal.txt");
        
        // Time to finish program before server shutdown
        sleep(1);
        system("pkill gzserver");
        system("> server_signal.txt");
        system("echo 0 > server_signal.txt");
        system("pkill robot_control");
    }

    return 0;
}
