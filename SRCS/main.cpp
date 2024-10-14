#include <iostream>
#include <string>
#include "header.hpp"

int main(int argc, char **argv)
{

    if (argc != 3)
    {
        std::cerr << "Error : Wrong number of arguments\n";
        std::cerr << "Usage : ./ircser <port> <password>" << std::endl;
        return (1); 
    }
    if (!checkPort(argv[1]))
        return (1);
    signal(SIGINT, signalHandler);
    initErrReplies();
    Server  Ircserv(argv[1], argv[2]);
    try
    {
        Ircserv.initSever();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (errno);
    }
}