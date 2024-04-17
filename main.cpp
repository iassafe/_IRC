/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khanhayf <khanhayf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 18:17:48 by khanhayf          #+#    #+#             */
/*   Updated: 2024/04/16 16:48:25 by khanhayf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"
#include <cstdlib>

int	main(int ac, char **av){
	Server	serv;
	try{
		int			por;
		std::string	pas;
	
		if (ac != 3){
			std::cerr << "Enter a port and a password" << std::endl;
			return (1);
		}
		serv.setPort(strtol(av[1], NULL, 10));
		serv.setPassword(av[2]);
		por = serv.getPort();
		std::string	s = av[1];
		if (s.find_first_not_of("0123456789") != std::string::npos){
			std::cerr << "Only numbers in port!" << std::endl;
			return (1);
		}

		pas = serv.getPassword();
		if (por < 1024 || por > 49151){
			std::cerr << "Enter a valid port" << std::endl;
			return (1);
		}
		if (pas.size() <= 0){
			std::cout << "Empty Password!" << std::endl;
			return (1);
		}
			// std::cout << "av[2] :" << av[2] << std::endl;
			// std::cout << "out " << strtol(pas.c_str(), NULL, 10) << std::endl;
			
		
			// if (atoi(av[2]) < 32 || atoi(av[2]) > 126){
			// 	std::cout << "Enter a valid password!" << std::endl;
			// 	return (1);
			// }
		
		// if (sizeof(av[2]) > 0){
		// 	for (size_t i = 0; i < sizeof(av[2]); ++i){
		// 		if (!isprint(strtol(av[2], NULL, 10)))
		// 			{
		// 			std::cerr << "Enter a valid password" << std::endl;
		// 			return (1);
		// 		}

		// 	}

		// }
		signal(SIGINT, Server::sigHandler);
		signal(SIGQUIT, Server::sigHandler);
		serv.launch_server();
	}
	catch(const std::exception &e){
		serv.closeFD();
		std::cout << e.what() << std::endl;
	}
}
