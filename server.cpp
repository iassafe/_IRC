
#include "Server.hpp"

bool	Server::signal = false;

Server::Server(){
	serverID = -1; 
	password = "\0";
	nick = "tikchbila";
	user = "tiwliwla";
}

Server::~Server(){
    for (unsigned int i = 0; i < clients.size(); i++)
        close(clients[i].getSocketDescriptor());//close users fd before quitting 
}

//setters

void	Server::setPort(int n){
	port = n;
}

void	Server::setPassword(char *str){
	password = str;
}

//getters

std::string    Server::getPassword(){
    return password;
}


//building the server
void	Server::sigHandler(int signum){
	(void)signum;
	std::cout << "signal found!" << std::endl;
	signal = true;//to stop the server
}

void	Server::clearClient(int fd){
	for (size_t i = 0; i < fds.size(); ++i){//remove client from fds vector
		if(fds[i].fd == fd){
			fds.erase(fds.begin() + i);
			break ;
		}
	}
	for (size_t i = 0; i < clients.size(); ++i){//remove client from Clients vector
		if(clients[i].getClientID() == fd){
			clients.erase(clients.begin() + i);
			break ;
		}
	}
}

void	Server::closeFD(){
	for (size_t i = 0; i < clients.size(); ++i){//close clients fd
		std::cout << "client disconnected" << std::endl;
		close(clients[i].getClientID());}
	if (serverID == -1){//close server socket
		std::cout << "server disconnected" << std::endl;
		close(serverID);}
}

void		Server::create_socket(){
	struct sockaddr_in serveraddress;
	struct pollfd		pollf;		
	memset(&serveraddress, 0, sizeof(serveraddress));
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(this->port);
	serveraddress.sin_addr.s_addr = INADDR_ANY;//any local machine address

	serverID = socket(AF_INET, SOCK_STREAM, 0);
	if (serverID == -1)
		throw (std::runtime_error("Server failed to get created!"));
	// std::cout << "Socket created successfully" << std::endl;
	int	val = 1;
	if (setsockopt(serverID, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)//SET OPTIONS OF SOCKET: SOL_SOCKET:the option is defined at socket level/ SO_REUSADDR : the option that allows to reuse local addresses which can be useful in scenarios where you want to quickly restart a server on the same address and port after it has been stopped.
		throw (std::runtime_error("the reuse of the address has failed!"));
	// std::cout << "Setsocketopt successfully" << std::endl;
	if (fcntl(serverID, F_SETFL, O_NONBLOCK) == -1)//PERFORM OPERATIONS ON FD : F_SETFL: THE OPERATION IS TO SET FILE STATUS FLAGS/ O_NONBLOCK : SOCKET NONBLOCKING
		throw ("Failed to set nonblocking flag!");
	std::cout << "fcntl successfully" << std::endl;
	if (bind(serverID, (const sockaddr *)&serveraddress, sizeof(serveraddress)) == -1)//bind the server to a port and IP
		throw(std::runtime_error("Binding to IP and port failed!"));
	// std::cout << "Socket binded" << std::endl;
	if (listen(serverID, SOMAXCONN) == -1)//socket is passive and listening to coming connections
		throw (std::runtime_error("server isn't listening!"));
	std::cout << "Socket listening ..." << std::endl;
	pollf.fd = serverID;//initialize the fds with server
	pollf.events = POLLIN;//flag to indicate theres data to read
	fds.push_back(pollf);//initialize fds vector
	std::cout << "server is listening from port : " << this->port << std::endl;
}

void	Server::launch_server(){
	create_socket();
	multi_clients();
}

void	Server::acceptClient(){
	Client				client;
	struct sockaddr_in	clientaddress;
	struct pollfd		newpool;
	socklen_t			clientaddrlen = 0;

	memset(&clientaddress, 0, sizeof(clientaddress));
	this->connectionID = accept(serverID, (struct sockaddr *)&clientaddress, &clientaddrlen);//new socket to 	assure safe communication with multiple clients 
	if (connectionID == -1){
		std::cerr << "Failed to connect!" << std::endl;
		return ;
	}
	if (send(connectionID, "enter: password, nickname, and username\n", 40, 0) == -1)
		throw (std::runtime_error("failed to send to client"));
	if (fcntl(connectionID, F_SETFL, O_NONBLOCK) == -1){//server configuration can impact client's so we set new socket as server 
		std::cerr << "failed to set nonblocking option!" << std::endl;
		return ;
	}
	newpool.fd = connectionID;
	newpool.events = POLLIN;

	client.setClientID(connectionID);
	client.setIP(inet_ntoa(clientaddress.sin_addr));
	clients.push_back(client);
	fds.push_back(newpool);
	std::cout << "accepted!" << std::endl;
}

void	to_lower(std::string &command){
	for (size_t i = 0; i < command.size(); ++i){
		command[i] = std::tolower(command[i]);
	}
}

std::string	skip_spaces(std::string str){
	for (size_t i = 0; i < str.size(); ++i){
		if (str[i] != ' ')
			return (&str[i]);
	}
	std::cout << "lets see" << str << std::endl;
	return (str);
}

static int validCommand(std::string &cmd){
    if (cmd == "join" || cmd == "privmsg" || cmd == "topic" \
        || cmd == "kick" || cmd == "mode" || cmd == "pass" || \
        cmd == "user" || cmd == "invite")
        return(1);
    return(0);
}

int countComma(std::string str){
	int count = 0;
	for(size_t i=0; i < str.length(); i++){
		if(str[i] == ',')
			count++;
	}
	return (count);
}

void Server::joinCommand(void){
	if (send(this->connectionID, "Valid Command\n", 14, 0) == -1)
		throw (std::runtime_error("failed to send to client"));
}

void Server::topicCommand(void){
	std::cout << "----------[" << this->command << "][" << this->args << "]\n";
	if (send(this->connectionID, "Topic Valid Command\n", 20, 0) == -1)
		throw (std::runtime_error("failed to send to client"));
}

int Server::validArgsJoin(void){
	if (!this->args[0] || this->args[0]== '\n' || this->args[0] != '#'
		|| (this->args[0] == '#' && !isalpha(this->args[1]))){
		return (0);
	}
	size_t fond = this->args.find_first_of(" \t\r\n");
	if (fond == std::string::npos || this->args[fond] == '\n')
		return (0);
	size_t fondComma = this->args.find_first_of(",");
	if (fondComma == std::string::npos){
		std::string temp_args = this->args;
		this->joinChannel[temp_args.substr(0, fond)] = temp_args.substr(fond + 1, temp_args.length());
		while(fond < temp_args.length() && temp_args[fond] == ' '){
			fond++;
		};
		temp_args = temp_args.substr(fond, temp_args.length());
		size_t fond_sp = temp_args.find_first_of(" ");
		if (fond_sp != std::string::npos){
			temp_args = temp_args.substr(fond_sp + 1, temp_args.length());
			size_t i;
			for(i=0; i < temp_args.length() && temp_args[i] == ' '; ++i){
			};
			if (temp_args[i] != '\n')
				return (0);
		}
	}
	else{
		std::string temp_args = this->args;
		std::string channels = temp_args.substr(0, fond);
		std::string password = temp_args.substr(fond + 1, temp_args.length());
		int count_ch = countComma(channels);
		int count_ps = countComma(password);
		if (count_ch != count_ps)
			return (0);
		password = skip_spaces(password);
		size_t fond_commach = channels.find_first_of(",");
		size_t fond_commaps = password.find_first_of(",");
		this->joinChannel[channels.substr(0, fond_commach)] = password.substr(0, fond_commaps);
		std::cout << "[" << channels.substr(0, fond_commach) <<"]["<< password.substr(0, fond_commaps) <<"]\n";
		channels = channels.substr(fond_commach + 1, channels.length());
		password = password.substr(fond_commaps + 1, password.length());
		for(int i = 0; i < count_ch; ++i){
			fond_commach = channels.find_first_of(",");
			fond_commaps = password.find_first_of(",");
			this->joinChannel[channels.substr(0, fond_commach)] = password.substr(0, fond_commaps);
			std::cout << "[" << channels.substr(0, fond_commach) <<"]["<< password.substr(0, fond_commaps) <<"]\n";
			channels = channels.substr(fond_commach + 1, channels.length());
			password = password.substr(fond_commaps + 1, password.length());
		}
	}
	return (1);
}

int Server::validArgsTopic(void){
	if (!this->args[0] || this->args[0]== '\n' || this->args[0] != '#'
		|| (this->args[0] == '#' && !isalpha(this->args[1]))){
		return (0);
	}
	size_t fond = this->args.find_first_of(" \t\r\n");
	if (fond == std::string::npos || this->args[fond] == '\n')
		return (0);
	std::string temp_args = this->args;
	this->joinChannel[temp_args.substr(0, fond)] = temp_args.substr(fond + 1, temp_args.length());
	while(fond < temp_args.length() && temp_args[fond] == ' '){
		fond++;
	};
	temp_args = temp_args.substr(fond, temp_args.length());
	size_t fond_sp = temp_args.find_first_of(" ");
	if (fond_sp != std::string::npos){
		temp_args = temp_args.substr(fond_sp + 1, temp_args.length());
		size_t i;
		for(i=0; i < temp_args.length() && temp_args[i] == ' '; ++i){
		};
		if (temp_args[i] != '\n')
			return (0);
	}
	return (1);
}


void Server::handleCommands1(void){
	if (this->command == "join"){
			if(validArgsJoin())
				joinCommand();
			else{
				if (send(this->connectionID, "Invalid args join\n", 18, 0) == -1)
					throw (std::runtime_error("failed to send to client"));
			}
	}
	else if (this->command == "topic"){
		if(validArgsTopic())
				topicCommand();
			else{
				if (send(this->connectionID, "Invalid args topic\n", 19, 0) == -1)
					throw (std::runtime_error("failed to send to client"));
			}
	}
}

void	Server::recieve_data(int fd){
	char	buffer[1024];
	Client	c;

	memset(buffer, 0, sizeof(buffer));
	size_t	total = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (total <= 0){
		std::cout << "client gone" << std::endl;
		clearClient(fd);
		close(fd);
	}
	else{
		std::string	buf = buffer;
		 size_t fond;
		std::string	new_buf = skip_spaces(buf);
		for(size_t i = 0; i <= new_buf.size(); i++){
			fond = new_buf.find_first_of("\n");
			if (fond == std::string::npos)
				return;
			std::cout << "content of fond++" << new_buf[fond] << "++" << std::endl;	
			std::string	commond = new_buf.substr(0, fond);
			std::cout << "command:" << commond << "--" << std::endl;
			size_t	sp = commond.find_first_of(" \t\r");
			this->command = commond.substr(0, sp);
			std::cout << "com:[" << this->command << "]--" << std::endl;
			new_buf = new_buf.substr(fond+1, new_buf.size());
			this->args = skip_spaces(commond.substr(sp + 1, commond.length()));
			std::cout << "argu:[" << this->args << "]--" << std::endl;
			std::cout << "new_buff :" << &new_buf[i] << std::endl;
			to_lower(this->command);
			if (validCommand(this->command)){
				handleCommands1();
        	}
			else if(send(this->connectionID, "Invalid command\n", 16, 0) == -1)
				throw (std::runtime_error("failed to send to client"));
		}
		// this->password += "\n";
		// if (strcmp(buffer, this->password.c_str())){
		// 	if (send(this->connectionID, "password :", 10, 0) == -1)
		// 		throw (std::runtime_error("failed to send to client"));
		// 	size_t	total = recv(fd, buffer, sizeof(buffer) - 1, 0);
		// 	if (total <= 0){
		// 		std::cout << "client gone" << std::endl;
		// 		clearClient(fd);
		// 		close(fd);
		// 	}
		// }
		// if (send(this->connectionID, "nickname :", 10, 0) == -1)
		// 		throw (std::runtime_error("failed to send to client"));
		// this->nick = buffer;
		// if (send(this->connectionID, "username :", 10, 0) == -1)
		// 		throw (std::runtime_error("failed to send to client"));
		// this->user = buffer;
		// std::cout << "user = " << this->nick << std::endl;
		// std::string	buf = buffer;
		// size_t found = buf.find(' ');
		// this->command = buf.substr(0, found);
		// this->args = buf.substr(found + 1, buf.length());
		// to_lower(this->command);
		// if (this->command == "join\n")
		// 	std::cout <<"join channel" << std::endl;
		// else if (this->command == "invite\n")
		// 	std::cout <<"invite user" << std::endl;
		// else if (this->command == "kick\n")
		// 	std::cout <<"kick user" << std::endl;
		// std::cout << "command : " << this->command << " args :" << this->args << std::endl;

	}
		// this->Clients[fd].setBuffer(buffer);
}

void	Server::multi_clients(){
	while (Server::signal == false){
		if (poll(&fds[0], fds.size(), -1) == -1 && Server::signal == false)//poll blocked indefinitely till an event occurs or ctrl c
			throw (std::runtime_error("poll() failed!"));
		for (size_t i = 0; i < fds.size(); ++i){
			if (fds[i].revents & POLLIN)//returned event: data to read
			{
				if (fds[i].fd == serverID)
					acceptClient();
				else
					recieve_data(fds[i].fd);
			}
		}
	}
	closeFD();
}

//managing users
bool    Server::isInUseNickname(std::string nickname){
    for (unsigned int i = 0; i < clients.size(); i++){
        if (clients[i].getNickname() == nickname)
            return true;
    }
    return false;
}

void    Server::addUser(Client const& client){
    clients.push_back(client);
}

void    Server::removeUser(Client const& client){
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); it++){
        if (it->getNickname() == client.getNickname()){
			close(client.getSocketDescriptor()); //close client fd
            clients.erase(it);
            return ;
        }
    }
}

bool    Server::isInUseChName(std::string chName){
    for (unsigned int i = 0; i < channels.size(); i++){
        if (channels[i].getName() == chName)
            return true;
    }
    return false;
}

void    Server::addChannel(Channel const& channel){
    channels.push_back(channel);
}

void    Server::removeChannel(Channel const& channel){
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); it++){
        if (it->getName() == channel.getName()){
            channels.erase(it);
            return ;
        }
    }
}

//other

void	Server::sendMsg(int clientFd, std::string msg){
	std::cerr << clientFd << ">> Error: " << msg <<"\n";
}

void	Server::handleCommands(std::string &cmd, std::string &args, Client &client){
	tolowercase(cmd);
	if (cmd == "user")
		userCommand(args, client);
	else if (cmd == "nick")
		nickCommand(args, client);
	else if (cmd == "pass")
		passCommand(args, client);
}

