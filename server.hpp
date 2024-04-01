
#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <poll.h>
#include <vector>
#include <fcntl.h>
#include <csignal>
#include <cstring>
#include <string>
#include <map>

#include "Client.hpp"
#include "Channel.hpp"

class Client;
class Channel;

class Server{
    private:
		int							serverID;
		int							port;
		int							connectionID;
		static bool					signal;
		std::string					password;
		std::string					nick;
		std::string					user;
		std::string 				command;
		std::string 				args;
		std::vector<struct pollfd>	fds;

        std::vector<Client> clients; //the currently connected clients in the server (the key in map is the client's nickname)
        std::vector<Channel> channels; //Available channels in the server

		// join
		std::map<std::string, std::string> joinChannel;
    public:
        Server();
        ~Server();//close users fds before quitting

        //setters
        void		setPort(int n);
		void		setPassword(char *str);
        
        //getters
        std::string    getPassword();
        
        //building the server
		void		create_socket();
		void		launch_server();
		void		multi_clients();
		void		acceptClient();
		void		recieve_data(int fd);
		static void	sigHandler(int signum);
		void		closeFD();
		void		clearClient(int fd);

		void		sendMsg(int clientFd, std::string msg); //needed to send error messages to a specific client

        //managing users
        bool    isInUseNickname(std::string nickname); //true if a nickname is already choosed by another client
        void    addUser(Client const& client); //add the new client in the clients container (only once)
        void    removeUser(Client const& client); //close fd when removing user

        //managing channels
        bool    isInUseChName(std::string chName); //true if there an other channel with the same name
        void    addChannel(Channel const& channel);
        void    removeChannel(Channel const& channel);

		// other
		void	handleCommands(std::string &cmd, std::string &args, Client &client);


		//
		void	handleCommands1(void);
		int		validArgsJoin(void);
		void	joinCommand(void);

};

#endif