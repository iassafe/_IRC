#include "Server.hpp"

Server::Server(){
	this->serverFD = -1;
	this->password = "\0";
	fillSayingsBox("bonus/sayings.txt");
}

Server::Server(Server const& obj){
	*this = obj;
}

Server& Server::operator=(Server const& obj){
	if (this != &obj){
		this->serverFD = obj.serverFD;
		this->port = obj.port;
		this->signal = obj.signal;
		this->password = obj.password;
		this->connectionID = obj.connectionID; 
		this->command = obj.command;
		this->args = obj.args;
		this->target = obj.target;
		this->message = obj.message;
		this->existPassword = obj.existPassword;
		this->ChannelTopic = obj.ChannelTopic; 
		this->topic = obj.topic;
		this->reason = obj.reason;
		this->ipAddress = obj.ipAddress;
		this->Channelkick = obj.Channelkick;
		this->joinChannel = obj.joinChannel;
		this->joinPassword = obj.joinPassword; 
		this->channelPass = obj.channelPass;
		this->ClientsKick = obj.ClientsKick;	
		this->vec_cl = obj.vec_cl;
		this->vec_ch = obj.vec_ch;
		this->fds = obj.fds;
		this->clients = obj.clients;
		this->channels = obj.channels;
		this->sayingsBox = obj.sayingsBox;
	}
	return(*this);
}

Server::~Server(){
	this->clients.clear();
	this->channels.clear();
	this->sayingsBox.clear();
	this->fds.clear();
}

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
	signal = true;
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

std::string Server::getTopic() const{
	return (this->topic);
}
std::string Server::getChannelTopic() const{
	return (this->ChannelTopic);
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
	std::cout << "server disconnected" << std::endl;
	close(serverFD);
}

void		Server::create_socket(){
	struct sockaddr_in serveraddress;
	struct pollfd		pollf;
	memset(&serveraddress, 0, sizeof(serveraddress));
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(this->port);
	serveraddress.sin_addr.s_addr = INADDR_ANY;

	serverFD = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFD == -1)
		throw (std::runtime_error("Server failed to get created!"));
	int	val = 1;
	if (setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) == -1)
		throw (std::runtime_error("the reuse of the address has failed!"));
	if (fcntl(serverFD, F_SETFL, O_NONBLOCK) == -1)
		throw ("Failed to set nonblocking flag!");
	std::cout << "fcntl successfully" << std::endl;
	if (bind(serverFD, (const sockaddr *)&serveraddress, sizeof(serveraddress)) == -1)
		throw(std::runtime_error("Binding to IP and port failed!"));
	if (listen(serverFD, SOMAXCONN) == -1)
		throw (std::runtime_error("server isn't listening!"));
	std::cout << "Socket listening ..." << std::endl;
	pollf.fd = serverFD;
	pollf.events = POLLIN;
	fds.push_back(pollf);
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
	this->connectionID = accept(serverFD, (struct sockaddr *)&clientaddress, &clientaddrlen);
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
	if (fcntl(connectionID, F_SETFL, O_NONBLOCK) == -1){
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
	if (total <= 0){
		std::cout << "client disconnected" << std::endl;
		clearClient(fd);
		close(fd);
		return ;
	}
	std::string strBuffer = buffer;
	for (i = 0; i < clients.size(); i++){
		if (clients[i].getClientFD() == fd)
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
		if (poll(&fds[0], fds.size(), -1) == -1 && Server::signal == false)
			throw (std::runtime_error("poll() failed!"));
		for (size_t i = 0; i < fds.size(); ++i){
			if (fds[i].revents & POLLIN)
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