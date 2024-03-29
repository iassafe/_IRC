#include "Server.hpp"
#include <cstdlib>
int	main(int ac, char **av){
	Server	serv;
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
