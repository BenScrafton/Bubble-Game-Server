#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include "Server.h"

int main() 
{
    Server server(53000);
    server.Run();

    return 0;
}

