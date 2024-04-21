#include"Server.hpp"

static void	toUpperCase(std::string &command){
	for (size_t i = 0; i < command.size(); ++i){
		command[i] = std::toupper(command[i]);
	}
}

static int validCommand(std::string &cmd){
    if (cmd == "JOIN" || cmd == "PRIVMSG" || cmd == "TOPIC" \
        || cmd == "kick" || cmd == "MODE" || cmd == "PASS" || \
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

void Server::execTopicCommand(void){
	std::cout << "----------[" << this->command << "][" << this->args << "]\n";
	if (send(this->connectionID, "Topic Valid Command\n", 20, 0) == -1)
		throw (std::runtime_error("failed to send to client"));
	size_t found = this->args.find_first_of(" \t\r");
    std::string channelName = this->args.substr(0, found);

    // Find the channel
    std::vector<Channel>::iterator it = channels.begin();
    for (; it != channels.end(); ++it) {
        if (it->getName() == channelName){
            break;
        }
    }
    // Check if the channel was found
    if (it != channels.end()) {
		it->setTopic(this->args.substr(found + 1, this->args.length()));
		std::cout << "New topic is:------->" << it->getTopic() << std::endl;
    } else {
        // Channel not found
    }
}

std::string	skipSpaces(std::string str){
	size_t i;
	for (i = 0; str[i] == ' '; ++i){
	}
	return (&str[i]);
}

int Server::validArgsTopic(void){
	this->args = skipSpaces(this->args);
	int count_args = 1;
	for(size_t i =0; i < this->args.length(); ++i){
		if (this->args[i] == ' '){
			count_args++;
			for(; this->args[i] == ' '; ++i){}
		}
	}
	if (count_args < 2)
		return(0);
	size_t found = this->args.find_first_of(" \t\r\n");
	// if (found == std::string::npos || this->args[found] == '\n')
	// 	return (0);
	std::string temp_args = this->args; 
	while(temp_args[found] == ' '){
		found++;
	};
	// if (temp_args[found] == '\0')
	// 	return(0);
	temp_args = temp_args.substr(found, temp_args.length());
	size_t found_sp = temp_args.find_first_of(" ");
	if (found_sp != std::string::npos){
		temp_args = temp_args.substr(found_sp + 1, temp_args.length());
		size_t i;
		for(i=0; i < temp_args.length() && temp_args[i] == ' '; ++i){
		};
		// if (temp_args[i] != '\n')
		// 	return (0);
	}
	return (1);
}

int Server::validArgsKick(void){
	this->args = skipSpaces(this->args);
	int count_args = 1;
	for(size_t i =0; i < this->args.length(); ++i){
		if (this->args[i] == ' '){
			count_args++;
			for(; this->args[i] == ' '; ++i){}
		}
	}
	std::cout << "couuuuuuunt_args:--->"<< count_args << std::endl;
	if (count_args < 2)
		return(0);
	size_t found = this->args.find_first_of(" \t\r\n");
	// if (found == std::string::npos || this->args[found] == '\n')
	// 	return (0);
	std::string temp_args = this->args;
	while(found < temp_args.length() && temp_args[found] == ' '){
		found++;
	};
	temp_args = temp_args.substr(found, temp_args.length());
	size_t found_sp = temp_args.find_first_of(" ");
	if (found_sp != std::string::npos){
		temp_args = temp_args.substr(found_sp + 1, temp_args.length());
		size_t i;
		for(i=0; i < temp_args.length() && temp_args[i] == ' '; ++i){
		};
	}
	return (1);
}

void Server::topicCommand(Client &c){
	this->args = skipSpaces(this->args);
	if (this->args == "")
		sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), "TOPIC"));
	else{
		if(validArgsTopic())
				execTopicCommand();
		else
			sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), "TOPIC"));
	}
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
