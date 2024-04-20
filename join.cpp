#include"Server.hpp"

void Server::createChannel(Client &c, int i){
	Channel newChannel(c, this->channelPass[i].first, *this);
	newChannel.addOperator(c);
	sendMsg(c.getClientFD(), RPL_JOIN(c.getNickname(), c.getUsername(), newChannel.getName(), c.getClientIP()));
	sendMsg(c.getClientFD(), RPL_NAMREPLY(c.getNickname(), newChannel.getName(),c.getNickname()));
	sendMsg(c.getClientFD(), RPL_ENDOFNAMES(c.getHostname(), c.getNickname(), newChannel.getName()));
}

void Server::addChannel(Client &c, int i){
	Channel &findingChannel = findChannel(this->channelPass[i].first);
	if(!findingChannel.hasAKey()){
		if (!findingChannel.isMember(c) && findingChannel.getMode() != "invite-only"){
			findingChannel.addRegularUser(c);
			sendMsg(c.getClientFD(), RPL_JOIN(c.getNickname(), c.getUsername(), findingChannel.getName(), c.getClientIP()));
			sendMsg(c.getClientFD(), RPL_NAMREPLY(c.getNickname(), findingChannel.getName(), c.getNickname()));
			sendMsg(c.getClientFD(), RPL_ENDOFNAMES(c.getHostname(), c.getNickname(), findingChannel.getName()));
			findingChannel.sendMsg2Members(*this, c);
		}
	}
	else{
		if (findingChannel.getKey() == this->channelPass[i].second){
			if (!findingChannel.isMember(c) && findingChannel.getMode() != "invite-only"){
				findingChannel.addRegularUser(c);
				sendMsg(c.getClientFD(), RPL_JOIN(c.getNickname(), c.getUsername(), findingChannel.getName(), c.getClientIP()));
				sendMsg(c.getClientFD(), RPL_NAMREPLY(c.getNickname(), findingChannel.getName(), c.getNickname()));
				sendMsg(c.getClientFD(), RPL_ENDOFNAMES(c.getHostname(), c.getNickname(), findingChannel.getName()));
				findingChannel.sendMsg2Members(*this, c);
			}
		}
		else
			sendMsg(c.getClientFD(), ERR_BADCHANNELKEY(c.getNickname(), findingChannel.getName()));
	}
}

void Server::execJoinCommand(Client &c){
	this->channelPass.resize(this->joinChannel.size());
	for(size_t i = 0; i < this->joinChannel.size(); ++i){
		this->channelPass[i].first = this->joinChannel[i];
		if (i < this->joinPassword.size()){
			size_t found = this->joinPassword[i].find_first_of(" ");
			if (i == (this->joinPassword.size() - 1) && found != std::string::npos){
				this->channelPass[i].second = this->joinPassword[i].substr(0, found);
			}
			else
				this->channelPass[i].second = this->joinPassword[i];
    	}
		else
			this->channelPass[i].second = "";
	}
	for(size_t i=0; i < this->channelPass.size(); ++i){
		if (this->channelPass[i].first[0] != '#' || (this->channelPass[i].first[0] == '#' && \
			!isdigit(this->channelPass[i].first[1]) && !isalpha(this->channelPass[i].first[1])))
			sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(this->channelPass[i].first, c.getNickname()));
		else{
			if (!this->isInUseChName(this->channelPass[i].first))
				createChannel(c, i);
			else
				addChannel(c, i);
		}
	}

	for (size_t i = 0; i < this->channelPass.size(); ++i){
    	this->channelPass[i].first.clear();
    	this->channelPass[i].second.clear();
	}
	this->joinChannel.clear();
	this->joinPassword.clear();
}

int Server::joinSingleChannel(void){
	size_t found = this->args.find_first_of(" \r\t");
	std::string temp_args = this->args;
	this->joinChannel.push_back(this->args.substr(0, found));
	if (this->existPassword){
		while(temp_args[found] == ' ' || temp_args[found] == '\r' || temp_args[found] == '\t'){
			found++;
		};
		temp_args = temp_args.substr(found, temp_args.length());
		size_t found_sp = temp_args.find_first_of(" \r\t");
		if (found_sp != std::string::npos)
			this->joinPassword.push_back(this->args.substr(found, found_sp));
		else
			this->joinPassword.push_back(this->args.substr(found, this->args.length()));
	}
	return(1);
}

void Server::joinMultiChannels(void){
	std::string temp_args = this->args;
	size_t found = this->args.find_first_of(" \r\t");
		std::string channels = temp_args.substr(0, found);
		int count_ch = countComma(channels);
		size_t found_commach = channels.find_first_of(",");
		this->joinChannel.push_back(channels.substr(0, found_commach));
		channels = channels.substr(found_commach + 1, channels.length());
		for(int i = 0; i < count_ch; ++i){
			found_commach = channels.find_first_of(",");
			this->joinChannel.push_back(channels.substr(0, found_commach));
			std::cout << "[" << channels.substr(0, found_commach) <<"]\n";
			channels = channels.substr(found_commach + 1, channels.length());
		}
		if (this->existPassword){
			std::string passWord = temp_args.substr(found + 1, temp_args.length());
			int count_ps = countComma(passWord);
			passWord = skipSpaces(passWord);
			size_t found_commaps = passWord.find_first_of(",");
			this->joinPassword.push_back(passWord.substr(0, found_commaps));
			passWord = passWord.substr(found_commaps + 1, passWord.length());
			for(int i = 0; i < count_ps; ++i){
				found_commaps = passWord.find_first_of(",");
				this->joinPassword.push_back(passWord.substr(0, found_commaps));
				passWord = passWord.substr(found_commaps + 1, passWord.length());
			}
		}
}

int Server::argsJoin(void){
	this->args = skipSpaces(this->args);
	size_t found = this->args.find_first_of(" ");
	if (found == std::string::npos) 
		return (0);
	else{
		while (this->args[found] == ' ' || this->args[found] == '\r' || this->args[found] == '\t'){
			found++;
		}
		if (this->args[found])
		{
			std::string passWord = &this->args[found];
			size_t foundSpace = passWord.find_first_of(" ");
			if (passWord[foundSpace] == ' ' || passWord[foundSpace] == '\r' || passWord[foundSpace] == '\t'){
				while (this->args[foundSpace] == ' ' || this->args[foundSpace] == '\r' || this->args[foundSpace] == '\t'){
					foundSpace++;
				}
			}
			return (1);
		}
		else
			return (0);
	}
}


void Server::whithoutPassword(void){
	size_t foundComma = this->args.find_first_of(",");
	if (foundComma != std::string::npos)
		joinMultiChannels();
	else
		joinSingleChannel();
}

void Server::whithPassword(void){
	size_t foundComma = this->args.find_first_of(",");
	if (foundComma != std::string::npos)
		joinMultiChannels();
	else
		joinSingleChannel();
}
