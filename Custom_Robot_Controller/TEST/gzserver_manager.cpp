#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    system("touch server_signal.txt");
    system("> server_signal.txt");

    for(int i = 1; i < 4; i++)
    {
        std::string world = "smallworld" + std::to_string(1) + ".world";
        std::string command = "../../Gazebo/gazebo_server.sh ../../Gazebo/smallTestWorlds/" + world + " &";
        system(command.c_str());

        sleep(6);
        system("echo 1 > server_signal.txt");

        sleep(3);
        system("> server_signal.txt");
        system("echo 0 > server_signal.txt");
        
        sleep(1);
        system("pkill gzserver");
        system("> server_signal.txt");
        system("echo 0 > server_signal.txt");
    }

    system("pkill robot_control");

    return 0;
}
