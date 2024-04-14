
#include "Server.hpp"

void    botCommand(Client &c, Server &s){
    if (!s.sayingsBox.empty()){
        std::srand(time(0));
        int ranIdx = std::rand() % s.sayingsBox.size();
        s.sendMsg(c.getClientFD(), "\n");
        s.sendMsg(c.getClientFD(), "\n");
        s.sendMsg(c.getClientFD(), "Welcome to the Wisdom Bot! Prepare for a moment of inspiration.\n");
        s.sendMsg(c.getClientFD(), "Get ready to receive a random saying or quote that may just light up your day.\n");
        s.sendMsg(c.getClientFD(), "\n");
        s.sendMsg(c.getClientFD(), "\n");
        s.sendMsg(c.getClientFD(), "\n");
        s.sendMsg(c.getClientFD(), "\n");
        s.sendMsg(c.getClientFD(), " __          ___         _                   ____        _   \n");
        s.sendMsg(c.getClientFD(), " \\ \\        / (_)       | |                 |  _ \\      | |  \n");
        s.sendMsg(c.getClientFD(), "  \\ \\  /\\  / / _ ___  __| | ___  _ __ ___   | |_) | ___ | |_ \n");
        s.sendMsg(c.getClientFD(), "   \\ \\/  \\/ / | / __|/ _` |/ _ \\| '_ ` _ \\  |  _ < / _ \\| __|\n");
        s.sendMsg(c.getClientFD(), "    \\  /\\  /  | \\__ \\ (_| | (_) | | | | | | | |_) | (_) | |_ \n");
        s.sendMsg(c.getClientFD(), "     \\/  \\/   |_|___/\\__,_|\\___/|_| |_| |_| |____/ \\___/ \\__|\n");
        s.sendMsg(c.getClientFD(), "\n");
        s.sendMsg(c.getClientFD(), "         *                                            *\n");
        s.sendMsg(c.getClientFD(), "                                   *                                                    *\n");
        s.sendMsg(c.getClientFD(), "\n");
        s.sendMsg(c.getClientFD(), "                      *                                               *\n");
        s.sendMsg(c.getClientFD(), "                                                  *\n");
        s.sendMsg(c.getClientFD(), "*\n");
        s.sendMsg(c.getClientFD(), "           *                            *\n");
        s.sendMsg(c.getClientFD(), "                               *                                           *\n");
        s.sendMsg(c.getClientFD(), "\n");
        s.sendMsg(c.getClientFD(), s.sayingsBox[ranIdx]);
        s.sendMsg(c.getClientFD(), "\n");
    }
    else
        throw (std::runtime_error("sayingsBox failed to get created!"));
}
             