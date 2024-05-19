
#include "Server.hpp"

bool	Server::signal = false;

std::string    Server::tolowercase(std::string str){
    if (!str.empty()){
        for (unsigned int i = 0; i < str.size(); ++i)
            str[i] = std::tolower(str[i]);
    }
    return (str);
}

bool    Server::isInUseChName(std::string chName){
   chName = tolowercase(chName);
    for (unsigned int i = 0; i < channels.size(); ++i){
        if (tolowercase(channels[i].getName()) == chName)
            return true;
    }
    return false;
}


void	Server::sigHandler(int signum){
	(void)signum;
	std::cout << "signal found!" << std::endl;
	signal = true;//to stop the server
}
Server::Server(){
	serverFD = -1; 
	password = "\0";
	fillSayingsBox("sayings.txt");
	// nick = "tikchbila";
	// user = "tiwliwla";
	// fds(0), sayingsBox(0), clients(0), channels(0),
	// joinChannel(0), joinPassword(0), vec_cl(0), vec_ch(0), channelPass(0),
	// ClientsKick(0)
	// this->serverFD = -1; 
	// this->password = "";
	// fillSayingsBox("sayings.txt");
	// this->reason = "";
	// this->port = 0;
	// this->signal = false;
	// this->connectionID = 0;
	// this->command = "";
	// this->args = "";
	// this->target = "";
	// this->message = "";
	// this->ChannelTopic = "";
	// this->existPassword = 0;
	// this->topic = "";
	// this->Channelkick = "";
}

Server::~Server(){
	this->clients.clear();
	this->channels.clear();
	this->sayingsBox.clear();
	this->fds.clear();
}

void	Server::setPort(int n){
	this->port = n;
}

void	Server::setPassword(char *str){
	this->password = str;
}
int	Server::getPort(){
	return(this->port);
}
std::string	Server::getPassword(){
	return (this->password);
}
int	Server::getServerFD(){
	return this->serverFD;
}

std::string Server::getCommand(){
	return command;
}

void    Server::clearClient(int fd){
    for (size_t i = 0; i < fds.size(); ++i){
        if(fds[i].fd == fd){
            fds.erase(fds.begin() + i);
            break ;
        }
    }
    for (size_t i = 0; i < clients.size(); ++i){
        if(clients[i].getClientFD() == fd){
            for (unsigned int j = 0; j < channels.size(); j++){
                if (channels[j].isMember(clients[i])){
                    channels[j].removeOperator(clients[i]);
                    channels[j].removeRegularUser(clients[i]);
                }
            }
            clients.erase(clients.begin() + i);
            return ;
        }
    }

}
void	Server::closeFD(){
	for (size_t i = 0; i < clients.size(); ++i){
		std::cout << "client disconnected" << std::endl;
		close(clients[i].getClientFD());
		}
	//if (serverFD == -1){//close server socket//KHH
		std::cout << "server disconnected" << std::endl;
		close(serverFD);//}
	//KHH
}

void		Server::create_socket(){
	struct sockaddr_in serveraddress;
	struct pollfd		pollf;
	memset(&serveraddress, 0, sizeof(serveraddress));
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(this->port);
	serveraddress.sin_addr.s_addr = INADDR_ANY;//any local machine address

	serverFD = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFD == -1)
		throw (std::runtime_error("Server failed to get created!")); //socket failed to get created
	// std::cout << "Socket created successfully" << std::endl;
	int	val = 1;
	if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)//SET OPTIONS OF SOCKET: SOL_SOCKET:the option is defined at socket level/ SO_REUSADDR : the option that allows to reuse local addresses which can be useful in scenarios where you want to quickly restart a server on the same address and port after it has been stopped.
		throw (std::runtime_error("the reuse of the address has failed!"));
	// std::cout << "Setsocketopt successfully" << std::endl;
	if (fcntl(serverFD, F_SETFL, O_NONBLOCK) == -1)//PERFORM OPERATIONS ON FD : F_SETFL: THE OPERATION IS TO SET FILE STATUS FLAGS/ O_NONBLOCK : SOCKET NONBLOCKING
		throw ("Failed to set nonblocking flag!");
	std::cout << "fcntl successfully" << std::endl;
	if (bind(serverFD, (const sockaddr *)&serveraddress, sizeof(serveraddress)) == -1)//bind the server to a port and IP //KHH return 0 on success
		throw(std::runtime_error("Binding to IP and port failed!"));
	// std::cout << "Socket binded" << std::endl;
	if (listen(serverFD, SOMAXCONN) == -1)//socket is passive and listening to coming connections
		throw (std::runtime_error("server isn't listening!"));
	std::cout << "Socket listening ..." << std::endl;
	pollf.fd = serverFD;//initialize the fds with server
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
	socklen_t			clientaddrlen = sizeof(clientaddress);

	memset(&clientaddress, 0, sizeof(clientaddress));
	this->connectionID = accept(serverFD, (struct sockaddr *)&clientaddress, &clientaddrlen);//new socket to 	assure safe communication with multiple clients 
	if (connectionID == -1){
		std::cerr << "Failed to connect!" << std::endl;
		return ;
	}
	if (clientaddrlen != sizeof(clientaddress)) {
		std::cerr << "Failed to get client address!" << std::endl;
		return ;
	}
	if (send(connectionID, "enter: password, nickname, and username\n", 41, 0) == -1)
		throw (std::runtime_error("failed to send to client"));
	if (fcntl(connectionID, F_SETFL, O_NONBLOCK) == -1){//server configuration can impact client's so we set new socket as server 
		std::cerr << "failed to set nonblocking option!" << std::endl;
		return ;
	}
	newpool.fd = connectionID;
	newpool.events = POLLIN;

	client.setClientFD(connectionID);
	client.setClientIP(inet_ntoa(clientaddress.sin_addr));
	clients.push_back(client);
	fds.push_back(newpool);
	std::cout << "accepted!" << std::endl;
}

void	Server::recieve_data(int fd){
	char		buffer[1024];
	std::string	str;
	size_t		i;

	memset(buffer, 0, sizeof(buffer));
	size_t	total = recv(fd, buffer, sizeof(buffer) - 1, 0);
	std::cout << "buffer==" << buffer << "==\n";
	if (total <= 0){
		if (errno == EAGAIN || errno == EWOULDBLOCK)
        	std::cout << "11111\n";
    	else if (errno == EINTR) 
        	std::cout << "22222\n";
    	else if (errno == EBADF) //The argument fd is an invalid descriptor.
        	std::cout << "33333\n";
    	else if (errno == ECONNRESET) 
        	std::cout << "44444\n";
    	else if (errno == EFAULT) 
        	std::cout << "55555\n";
    	else if (errno == EINVAL) 
        	std::cout << "66666\n";
    	else if (errno == ENOBUFS) 
        	std::cout << "77777\n";
    	else if (errno == ENOTCONN) 
        	std::cout << "88888\n";
    	else if (errno == ENOTSOCK) 
        	std::cout << "99999\n";
    	else if (errno == EOPNOTSUPP) 
        	std::cout << "*****\n";
    	else if (errno == ETIMEDOUT) 
        	std::cout << "-----\n";
    	else if (errno == EMSGSIZE) 
        	std::cout << "+++++\n";
    	else if (errno == ENOMEM) 
        	std::cout << "/////\n";
		else
        	std::cout << "33333\n";
		std::cout << "client disconnected" << std::endl;
		clearClient(fd);
		close(fd);
	}
	std::string strBuffer = buffer;
	for (i = 0; i < clients.size(); i++){
		if (clients[i].getClientFD() == fd)//IF ITS NOT FOUND
			break ;
	}
	if (strBuffer.find_first_of("\n") == std::string::npos)
		clients[i].setBuffer(buffer);
	else{
		if (clients[i].getBuffer() != ""){
			clients[i].setBuffer(strBuffer);
			strBuffer = clients[i].getBuffer();
		}
		if (strBuffer.size() > 512){
			if (!isRegistered(clients[i].getNickname()))
				str = "*";
			else
				str = clients[i].getNickname();
			sendMsg(fd, ERR_INPUTTOOLONG(str));
		}
		else{
			std::string	buf = strBuffer;
			size_t fond;
			std::string	new_buf = skipSpaces(buf);
			for(size_t i = 0; i <= new_buf.size(); i++){
				fond = new_buf.find_first_of("\n");
				if (fond == std::string::npos)
					return;
				std::string	commond = new_buf.substr(0, fond);
				size_t	sp = commond.find_first_of("\t\r ");
				if (sp != std::string::npos){
					size_t	ind = sp;
					while (commond[ind] == '\t' || commond[ind] == '\r' || commond[ind] == ' ')
						ind++;
					if (commond[ind] == '\n')
						this->args = "";
					else
						this->args = commond.substr(ind, fond);
					this->command = commond.substr(0, sp);
				}
				else{
					this->command = commond.substr(0, fond); 
					this->args = '\0';
				}
				new_buf = new_buf.substr(fond+1, new_buf.size());
				std::cout << "command=" << command << "=\n";
				checkCommands(fd);
				command.clear();
				args.clear();
			}
		}
		strBuffer.clear();
		this->clients[i].clearBuffer();
	}
}

void	Server::multi_clients(){
	while (Server::signal == false){
		if (poll(&fds[0], fds.size(), -1) == -1 && Server::signal == false)//poll blocked indefinitely till an event occurs or ctrl c
			throw (std::runtime_error("poll() failed!"));
		for (size_t i = 0; i < fds.size(); ++i){
			if (fds[i].revents & POLLIN)//returned event: data to read
			{
				if (fds[i].fd == serverFD)
					acceptClient();
				else
					recieve_data(fds[i].fd);
			}
		}
	}
	closeFD();
}

void	Server::sendMsg(int fd, std::string msg){
	if (send(fd, msg.c_str(), msg.size(), 0) == -1)
		throw (std::runtime_error("failed to send to client"));
}

bool    Server::isInUseNickname(std::string nickname){
	nickname = tolowercase(nickname);
    for (unsigned int i = 0; i < clients.size(); i++){
		std::string cltnick;
		cltnick = tolowercase(clients[i].getNickname());
        if (cltnick == nickname && clients[i].isRegistered())
            return true;
    }
    return false;
}

void    Server::addChannel(Channel const& channel){
    channels.push_back(channel);
}

bool	Server::isRegistered(std::string nickname){
	for (unsigned int i = 0; i < clients.size(); i++){
        if (clients[i].getNickname() == nickname && clients[i].isRegistered())
            return true;
    }
	return false;
}

Client		&Server::findClient(std::string nn){
	unsigned int i;
	nn = tolowercase(nn);
	for (i = 0; i < clients.size(); i++){
		if (tolowercase(clients[i].getNickname()) == nn)
			return (clients[i]);
	}
	return clients[i];
}

Channel		&Server::findChannel(std::string chname){
	unsigned int i;
	chname = tolowercase(chname);
	for (i = 0; i < channels.size(); i++){
		if (tolowercase(channels[i].getName()) == chname)
			return (channels[i]);
	}
	return channels[i];
}

void	Server::fillSayingsBox(std::string fileName){
	std::fstream base(fileName);
    if (!base.is_open())
        throw std::runtime_error("Can not open the sayings data base\n");
    std::string line;
    while (std::getline(base, line))
        sayingsBox.push_back(line);
    base.close();
}

void Server::removeChannel(std::string chName){
	for (unsigned int i = 0; i < this->channels.size(); i++){
        if (tolowercase(this->channels[i].getName()) == tolowercase(chName)){
            this->channels.erase(this->channels.begin() + i);
            break ;
        }
    }
}

bool	Server::msgAlreadyRecieved(std::string nick){
	for (unsigned int i = 0; i < nickMsgRecievers.size(); i++){
		if (nickMsgRecievers[i] == nick)
			return true;
	}
	return false;
}