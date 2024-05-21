#include"Server.hpp"

static void	toUpperCase(std::string &command){
	for (size_t i = 0; i < command.size(); ++i){
		command[i] = std::toupper(command[i]);
	}
}

static int validCommand(std::string &cmd){
    if (cmd == "JOIN" || cmd == "PRIVMSG" || cmd == "TOPIC" \
        || cmd == "KICK" || cmd == "MODE" || cmd == "PASS" || \
        cmd == "USER" || cmd == "INVITE" || cmd == "NICK")
        return(1);
    return(0);
}

void Server::handleError(Client &c){                      
    if (this->command == "USER")
		sendMsg(c.getClientFD(), ERR_USAGE(c.getNickname(), this->command, "<username> <unused> <unused> :<realname>"));
	else if (this->command == "NICK")
		sendMsg(c.getClientFD(), ERR_USAGE(c.getNickname(), this->command, "<newnick>"));
	else if (this->command == "PASS")
		sendMsg(c.getClientFD(), ERR_USAGE(c.getNickname(), this->command, "<password>"));
	else if (this->command == "MODE")
		sendMsg(c.getClientFD(), ERR_USAGE(c.getNickname(), this->command, "<target> [[(+|-)]<modes> [<mode-parameters>]]"));
	else if (this->command == "JOIN")
		sendMsg(c.getClientFD(), ERR_USAGE(c.getNickname(), this->command, "<channel>[,<channel>]+ [<key>[,<key>]+]"));
	else if (this->command == "TOPIC")
		sendMsg(c.getClientFD(), ERR_USAGE(c.getNickname(), this->command, "<channel> [:<topic>]"));
	else if (this->command == "KICK")
		sendMsg(c.getClientFD(), ERR_USAGE(c.getNickname(), this->command, "<channel> <nick>[,<nick>]+ [:<reason>]"));
    else if(this->command == "PRIVMSG")
		sendMsg(c.getClientFD(), ERR_USAGE(c.getNickname(), this->command, "<target>[,<target>]+ :<message>")); 
}

void	Server::handleCommands(Client &c){
	this->args = skipSpaces(this->args);
	if(this->args.empty() && this->command != "INVITE"){
		sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), this->command));
		if (c.isRegistered())
			handleError(c);
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
        	sendMsg(c.getClientFD(), ERR_NOTREGISTERED(this->command));
        	return ;
		}
		if (this->command == "INVITE")
			inviteCommand(args, c);
		else if (this->command == "MODE")
			modeCommand(args, c);
		else if (this->command == "JOIN")
			joinCommand(c);
		else if (this->command == "TOPIC")
			topicCommand(c);
		else if (this->command == "KICK")
			kickCommand(c);
		else if (this->command == "PRIVMSG")
			privmsgCommand(args, c);
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
	if (i == clients.size())
		throw(std::runtime_error("Client no found in the server container\n")); 
	if (validCommand(this->command))
		handleCommands(this->clients[i]);
	else if (this->command != "" && this->clients[i].isRegistered())
		sendMsg(fd, ERR_UNKNOWNCOMMAND(this->clients[i].getNickname(), this->command));
}

int countComma(std::string str){
	int count = 0;
	for(size_t i=0; i < str.length(); i++){
		if(str[i] == ','){
			while(str[i] == ','){
				i++;
			}
			if (str[i] && str[i] != ' ' && str[i] != '\r' && str[i] != '\t')
				count++;
		}
	}
	return (count);
}

std::string	skipSpaces(std::string str){
	size_t i;
	for (i = 0; str[i] == ' '; ++i){
	}
	return (&str[i]);
}

