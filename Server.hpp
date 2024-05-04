#ifndef SERVER_HPP
#define SERVER_HPP
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "Client.hpp"
#include "Channel.hpp"
#include <poll.h>
#include <vector>
#include <fcntl.h>
#include <csignal>
#include <cstring>
#include <string>
#include <cstdlib>
#include <sstream>
#include <map>
#include <fstream>
#include <cctype> // toupper
#include "responses.hpp"
#include <ctime>

class Client;
class Channel;
//TODO : need canonical form for classes
class	Server{
	private:
		int									serverFD; // (name changed from serverID to severFD)
		int									port;
		static bool							signal;
		std::string							password;
		int									connectionID;
		std::string 						command;
		std::string 						args;
		std::vector<struct pollfd>			fds;
		// std::map<std::string, std::string>	map;
		std::vector<std::string>			sayingsBox;
		std::vector<Client>					clients;
		std::vector<Channel>				channels;
		//AZMARA
		std::string							target;//NEW
		std::string							message;//NEW
		std::vector<std::string>			vec_cl;//NEW
		std::vector<std::string>			vec_ch;//NEW

		// join
		int existPassword;
		std::vector<std::string> joinChannel;
		std::vector<std::string> joinPassword;
		std::vector<std::pair<std::string, std::string> > channelPass;
		std::string ChannelTopic;
		std::string topic;
		std::string Channelkick;
		std::vector<std::string> ClientsKick;	

		///////&&&&&&&&&&&&&&
		std::string reason;

	public:
		Server();
		~Server();//close users fds before quitting
		//--Setters--//
		void		setPort(int n);
		void		setPassword(char *str);
		//--Getters--//
		int			getPort();
		std::string	getPassword();
		int			getServerFD();
		std::string getCommand();
		//----//
		void		create_socket();
		void		launch_server();
		void		multi_clients();
		void		acceptClient();
		void		recieve_data(int fd);
		static void	sigHandler(int signum);
		void		closeFD();
		void		clearClient(int fd);

		void		addChannel(Channel const& channel);
		bool    	isInUseNickname(std::string nickname);
		bool    	isInUseChName(std::string chName);
		void		sendMsg(int clientFd, std::string msg);
		void		handleCommands(Client &c);
		bool		isRegistered(std::string nickname);
		Client		&findClient(std::string nn);
		Channel		&findChannel(std::string chname);

		void		clearClientslist();
		void		clearChannelslist();

		void	fillSayingsBox(std::string fileName);
        void    nickCommand(std::string &args, Client &c);
        void    userCommand(std::string &args, Client &c);
        void    passCommand(std::string &args, Client &c);
        void	inviteCommand(std::string &args, Client &c);
        void    modeCommand(std::string &args, Client &c);
        void    botCommand(Client &c);

		// ikrame
		void	checkCommands(int fd);
		int		argsJoin(void);
		void	joinCommand(Client &c);
		void	execJoinCommand(Client &c);
		int		validArgsTopic(void);
		int		validArgsKick(void);
		void	topicCommand(Client &c);
		void	execTopicCommand(Client &c);
		void	kickCommand(Client &c);
		void	execKickCommand(Client &c);
		int 	joinSingleChannel(void);
		void	joinMultiChannels(void);

		void 	whithoutPassword(void);
		void 	whithPassword(void);
		void	createChannel(Client &c, int i);
		void	addChannel(Client &c, int i);
		void 	makeClientKick(std::string clKick, int exist2Points);

		std::string    tolowercase(std::string str);
		bool	isValidNickName(std::string nickname);
		//AZMARA
		//AZMARA
		void	privmsgCommand(std::string &args, Client &cli);//NEW
		int		validArgsPriv(std::string &args, Client &cli);//NEW
		void	store_clients_channels(std::string &args, size_t count, size_t ind, size_t start);
		void	sendToClients(size_t msg_begin, Client &cli, bool isMessage);
		void	sendToChannels(size_t msg_begin, Client &cli, bool isMessage);


		////////UPPPPPPPP
		void handleError(Client &c);
		void sendNickMsg2Mem(std::string msg, Client c);
		void removeChannel(std::string chName);


		
};



///////
std::string	skipSpaces(std::string str);
int countComma(std::string str);

#endif