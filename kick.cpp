#include"Server.hpp"

void Server::execKickCommand(Client& c){
	for (size_t i = 0; i < this->ClientsKick.size(); ++i){
			if (!isInUseChName(this->Channelkick))
				sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(this->Channelkick, c.getNickname()));
			else{
				if (!isInUseNickname(this->ClientsKick[i]))
					sendMsg(c.getClientFD(), ERR_NOSUCHNICK(c.getNickname(), this->ClientsKick[i]));
				else{
					Client& findingClient = findClient(this->ClientsKick[i]);
					Channel &findingChannel = findChannel(this->Channelkick);
					if (!findingChannel.isMember(findingClient))
						sendMsg(c.getClientFD(), ERR_USERNOTINCHANNEL(c.getNickname(), findingClient.getNickname(), this->Channelkick));
					else{
						if (!findingChannel.isOperator(c))
							sendMsg(c.getClientFD(), ERR_CANNOTKICK(c.getNickname(), this->Channelkick));
						else{
							findingChannel.sendMsgKick2Members(*this, c, findingClient.getNickname(), this->reason);
							if(findingChannel.isOperator(findingClient))
								findingChannel.removeOperator(findingClient);
							else if (findingChannel.isRegularuser(findingClient))
								findingChannel.removeRegularUser(findingClient);
							if (!findingChannel.getSizeMembers())
								removeChannel(findingChannel.getName());
						}
					}

				}
			}
	}
	this->reason.clear();
	this->ClientsKick.clear();
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
	this->reason = "";
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
	if (count_args > 2 && !exist2Points){
		std::string str = this->args.substr(found_sp + 1, this->args.length());
		str = skipSpaces(str);
		size_t space = str.find_first_of(" \r\t");
		str = str.substr(space, str.length());
		str = skipSpaces(str);
		this->reason = str;
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
				execKickCommand(c);
		else
			sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), "KICK"));
	}
}