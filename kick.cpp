#include"Server.hpp"


void Server::execKickCommand(void){
	if (send(this->connectionID, "Valid Command\n", 14, 0) == -1)
		throw (std::runtime_error("failed to send to client"));
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