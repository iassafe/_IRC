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
#include <cctype>
#include "responses.hpp"

class Client;
class Channel;

class	Server{
	private:
		int													serverFD;
		int													port;
		static bool											signal;
		std::string											password;
		int													connectionID;
		std::string 										command;
		std::string 										args;
		std::vector<struct pollfd>							fds;
		std::vector<Client>									clients;
		std::vector<Channel>								channels;
		std::string											target;
		std::string											message;
		std::vector<std::string>							vec_cl;
		std::vector<std::string>							vec_ch;
		int													existPassword;
		std::string											ChannelTopic;
		std::string											topic;
		std::string											reason;
		std::string											ipAddress;
		std::string											Channelkick;
		std::vector<std::string>							joinChannel;
		std::vector<std::string>							joinPassword;
		std::vector<std::pair<std::string, std::string> > 	channelPass;
		std::vector<std::string>							ClientsKick;	

	public:
		std::vector<std::string> nickMsgRecievers;
		Server();
		Server(Server const& obj);
		Server& operator=(Server const& obj);
		~Server();

		int			getPort();
		int			getServerFD();
		std::string	getPassword();
		std::string getCommand();
		std::string getTopic() const;
		std::string getChannelTopic() const;

		void		setPort(int n);
		void		setPassword(char *str);

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

        void		nickCommand(std::string &args, Client &c);
        void		userCommand(std::string &args, Client &c);
        void		passCommand(std::string &args, Client &c);
        void		inviteCommand(std::string &args, Client &c);
        void		modeCommand(std::string &args, Client &c);


		void		checkCommands(int fd);
		int			argsJoin(void);
		void		joinCommand(Client &c);
		void		execJoinCommand(Client &c);
		int			validArgsTopic(void);
		int			validArgsKick(void);
		void		topicCommand(Client &c);
		void		execTopicCommand(Client &c);
		void		kickCommand(Client &c);
		void		execKickCommand(Client &c);
		void 		joinSingleChannel(void);
		void		joinMultiChannels(void);

		void 		whithoutPassword(void);
		void 		whithPassword(void);
		void		createChannel(Client &c, int i);
		void		addChannel(Client &c, int i);
		void 		makeClientKick(std::string clKick, int exist2Points);
		std::string tolowercase(std::string str);
		bool		isValidNickName(std::string nickname);
		void		privmsgCommand(std::string &args, Client &cli);
		int			validArgsPriv(std::string &args, Client &cli);
		void 		handleError(Client &c);
		void 		removeChannel(std::string chName);
		bool		msgAlreadyRecieved(std::string nick);
};

std::string	skipSpaces(std::string str);
int countComma(std::string str);
std::string skipCommas(std::string s);

#endif