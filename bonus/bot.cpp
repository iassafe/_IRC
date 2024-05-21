
#include "Server.hpp"

void    Server::botCommand(Client &c){
    if (!sayingsBox.empty()){
        std::srand(time(0));
        int ranIdx = std::rand() % sayingsBox.size();
        sendMsg(c.getClientFD(), "\n");
        sendMsg(c.getClientFD(), "\n");
        sendMsg(c.getClientFD(), "Welcome to the Wisdom Bot! Prepare for a moment of inspiration.\n");
        sendMsg(c.getClientFD(), "Get ready to receive a random saying or quote that may just light up your day.\n");
        sendMsg(c.getClientFD(), "\n");
        sendMsg(c.getClientFD(), "\n");
        sendMsg(c.getClientFD(), "\n");
        sendMsg(c.getClientFD(), "\n");
        sendMsg(c.getClientFD(), " __          ___         _                   ____        _   \n");
        sendMsg(c.getClientFD(), " \\ \\        / (_)       | |                 |  _ \\      | |  \n");
        sendMsg(c.getClientFD(), "  \\ \\  /\\  / / _ ___  __| | ___  _ __ ___   | |_) | ___ | |_ \n");
        sendMsg(c.getClientFD(), "   \\ \\/  \\/ / | / __|/ _` |/ _ \\| '_ ` _ \\  |  _ < / _ \\| __|\n");
        sendMsg(c.getClientFD(), "    \\  /\\  /  | \\__ \\ (_| | (_) | | | | | | | |_) | (_) | |_ \n");
        sendMsg(c.getClientFD(), "     \\/  \\/   |_|___/\\__,_|\\___/|_| |_| |_| |____/ \\___/ \\__|\n");
        sendMsg(c.getClientFD(), "\n");
        sendMsg(c.getClientFD(), "         *                                            *\n");
        sendMsg(c.getClientFD(), "                                   *                                                    *\n");
        sendMsg(c.getClientFD(), "\n");
        sendMsg(c.getClientFD(), "                      *                                               *\n");
        sendMsg(c.getClientFD(), "                                                  *\n");
        sendMsg(c.getClientFD(), "*\n");
        sendMsg(c.getClientFD(), "           *                            *\n");
        sendMsg(c.getClientFD(), "                               *                                           *\n");
        sendMsg(c.getClientFD(), "\n");
        sendMsg(c.getClientFD(), sayingsBox[ranIdx]);
        sendMsg(c.getClientFD(), "\n");
    }
    else
        throw (std::runtime_error("sayingsBox failed to get created!"));
}
       