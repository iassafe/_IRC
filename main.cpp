

#include "Server.hpp"
#include <cstdlib>

//When you initialize the static references Client::server and Channel::server with serve, you're referring to the global serve object. However, when you later assign serv to these references inside main, you're referring to the local serv object.
//In this context, serve and serv are not the same object; they are distinct objects with different lifetimes and scopes.

Server	serve; //This declares a Server object named serv in the global scope. This object exists throughout the lifetime of the program.
Server &Client::server = serve; //This initializes a static reference server in the Client class with the serv object. This means that the Client::server reference will always refer to the serv object.
Server &Channel::server = serve; //This initializes a static reference server in the Channel class with the serv object. Similar to Client::server, the Channel::server reference will always refer to the serv object.

int	main(int ac, char **av){
	Server	serv;
	Client::server = serv;//This assigns the serv object to the static reference server in the Client class. This effectively changes what Client::server refers to, making it point to the newly created serv object inside main.
	Channel::server = serv;//Similarly, this assigns the serv object to the static reference server in the Channel class, changing what Channel::server refers to.
	try{
		if (ac != 3){
			std::cerr << "Enter a port and a password" << std::endl;
			return (1);
		}
		// if (strtol(av[1], NULL, 10) < 1024 || strtol(av[1], NULL, 10) > 49151){
		// 	std::cerr << "Enter a valid port" << std::endl;
		// 	return (1);
		// }
		// if (!isprint(strtol(av[2], NULL, 10)) || sizeof(av[2]) <= 0)
		// 	{
		// 	std::cerr << "Enter a valid password" << std::endl;
		// 	return (1);  
		// }
		serv.setPort(strtol(av[1], NULL, 10));
		serv.setPassword(av[2]);
		signal(SIGINT, Server::sigHandler);
		signal(SIGQUIT, Server::sigHandler);
		serv.launch_server();
	}
	catch(const std::exception &e){
		serv.closeFD();
		std::cout << e.what() << std::endl;
	}
}