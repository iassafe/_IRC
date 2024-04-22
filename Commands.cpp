#include"Server.hpp"

static void	toUpperCase(std::string &command){
	for (size_t i = 0; i < command.size(); ++i){
		command[i] = std::toupper(command[i]);
	}
}

static int validCommand(std::string &cmd){
    if (cmd == "JOIN" || cmd == "PRIVMSG" || cmd == "TOPIC" \
        || cmd == "KICK" || cmd == "MODE" || cmd == "PASS" || \
        cmd == "USER" || cmd == "INVITE" || cmd == "BOT" || cmd == "NICK")
        return(1);
    return(0);
}

void	Server::handleCommands(Client &c){
	this->args = skipSpaces(this->args);
	if(this->args == ""){
		sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), this->command));
		return ;
	}
	if (this->command == "USER" || this->command == "NICK" || this->command == "PASS"){
		if (this->command == "USER")
			userCommand(args, c);
		else if (this->command == "NICK")
			nickCommand(args, c);
		else if (this->command == "PASS")
			passCommand(args, c);
	}
	else{
		if (!c.isRegistered()){
        	sendMsg(c.getClientFD(), ERR_NOTREGISTERED(c.getNickname()));
        	return ;
		}
		if (this->command == "INVITE")
			inviteCommand(args, c);
		else if (this->command == "MODE")
			modeCommand(args, c);
		else if (this->command == "BOT")
			botCommand(c);
		else if (this->command == "JOIN")
			joinCommand(c);
		else if (this->command == "TOPIC")
			topicCommand(c);
		else if (this->command == "KICK")
			kickCommand(c);
	} 
}

void Server::checkCommands(int fd){
	toUpperCase(this->command);
	unsigned int i = 0;
	for (i = 0; i < this->clients.size(); i++){
		if (this->clients[i].getClientFD() == fd){
			break ;
		}
	}
	if (i == this->clients.size()) //this is not part of the implementation just in case this happens
		std::cout << "Client no found in container\n";
	if (validCommand(this->command))
		handleCommands(this->clients[i]);
	else if (this->command != "" && this->clients[i].isRegistered())
		sendMsg(fd, ERR_UNKNOWNCOMMAND(this->clients[i].getNickname(), this->command));
}

int countComma(std::string str){
	int count = 0;
	for(size_t i=0; i < str.length(); i++){
		if(str[i] == ',')
			count++;
	}
	return (count);
}


void Server::execKickCommand(void){
	if (send(this->connectionID, "Valid Command\n", 14, 0) == -1)
		throw (std::runtime_error("failed to send to client"));
}

std::string	skipSpaces(std::string str){
	size_t i;
	for (i = 0; str[i] == ' '; ++i){
	}
	return (&str[i]);
}

void Server::makeClientKick(std::string clKick, int exist2Points){
	std::vector<std::string> vec;
	clKick = skipSpaces(clKick);
	size_t countClient = 0;
	if (!exist2Points)
		countClient = countComma(clKick);
	if (!countClient)
		vec.push_back(clKick);
	else{
		size_t found_comma = clKick.find_first_of(",");
		vec.push_back(clKick.substr(0, found_comma));
		clKick = clKick.substr(found_comma + 1, clKick.length());
		for(size_t i = 0; i < countClient; ++i){
			found_comma = clKick.find_first_of(",");
			if (found_comma != std::string::npos){
				vec.push_back(clKick.substr(0, found_comma));
				clKick = clKick.substr(found_comma + 1, clKick.length());
			}
			else
				vec.push_back(clKick.substr(0, clKick.length()));
		}
	}
	for(size_t i = 0; i < vec.size(); ++i){
		if (vec[i] != "")
			this->ClientsKick.push_back(vec[i]);
	}
	vec.clear();
}

int Server::validArgsKick(void){
	this->args = skipSpaces(this->args);
	int count_args = 1;
	for(size_t i =0; i < this->args.length(); ++i){
		if (this->args[i] == ' '){
			for(; this->args[i] == ' '; ++i){}
			if (this->args[i])
				count_args++;
		}
	}
	if (count_args < 2)
		return(0);
	size_t found = this->args.find_first_of(" \t\r");
	std::string temp_args = this->args;
	this->Channelkick = temp_args.substr(0, found);
	temp_args = temp_args.substr(found, temp_args.length());
	int exist2Points = 0;
	temp_args = skipSpaces(temp_args);
	if (temp_args[0] == ':'){
		temp_args = temp_args.substr(1, temp_args.length());
		exist2Points = 1;
	}
	size_t found_sp = temp_args.find_first_of(" \t\r");
	if (found_sp != std::string::npos){
		temp_args = skipSpaces(temp_args);
		if(!exist2Points)
			temp_args = temp_args.substr(0, found_sp);
		else
			temp_args = temp_args.substr(0, temp_args.length());
	}
	makeClientKick(temp_args, exist2Points);
	return (1);
}

void Server::kickCommand(Client &c){
	this->args = skipSpaces(this->args);
	if (this->args == "")
		sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), "KICK"));
	else{
		if(validArgsKick())
				execKickCommand();
		else
			sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), "KICK"));
	}
}

void Server::joinCommand(Client &c){
	this->existPassword = 0;
	this->args = skipSpaces(this->args);
	if (this->args == "")
		sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), "JOIN"));
	else{
		int check = argsJoin();
		if (!check)
			whithoutPassword();
		else if (check){
			this->existPassword = 1;
			whithPassword();
		}
		execJoinCommand(c);
	}
}
