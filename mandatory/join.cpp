#include"Server.hpp"

void Server::createChannel(Client &c, int i){
	Channel newChannel(c, this->channelPass[i].first, *this);
	newChannel.addOperator(c);
	sendMsg(c.getClientFD(), RPL_JOIN(c.getNickname(), c.getUsername(), newChannel.getName(), c.getClientIP()));
	sendMsg(c.getClientFD(), RPL_NAMREPLY(("@" + c.getNickname()), newChannel.getName(),c.getNickname()));
	sendMsg(c.getClientFD(), RPL_ENDOFNAMES(c.getHostname(), c.getNickname(), newChannel.getName()));
}

void Server::addChannel(Client& c, int i){
	Channel &findingChannel = findChannel(this->channelPass[i].first);
	std::string member = findingChannel.makeStringMember();
	if (c.isInUseInvitedCh(findingChannel.getName())){
		if (!findingChannel.isMember(c)){
			member += " " + c.getNickname();
			c.removeInvitedCh(this->channelPass[i].first);
			findingChannel.addRegularUser(c);
			sendMsg(c.getClientFD(), RPL_JOIN(c.getNickname(), c.getUsername(), findingChannel.getName(), c.getClientIP()));
			sendMsg(c.getClientFD(), RPL_NAMREPLY(member, findingChannel.getName(), c.getNickname()));
			sendMsg(c.getClientFD(), RPL_ENDOFNAMES(c.getHostname(), c.getNickname(), findingChannel.getName()));
			findingChannel.sendMsg2Members(*this, c);
		}
	}
	else{
		if(findingChannel.getHasLimit()){
			if(!findingChannel.hasLimitCanJ()){
				sendMsg(c.getClientFD(), ERR_CHANNELISFULL(c.getNickname(), findingChannel.getName()));
				return ;
			}
		}
		if(!findingChannel.getHasKey()){
			if (!findingChannel.isMember(c) && findingChannel.getMode() != "invite-only"){
				member += " " + c.getNickname();
				findingChannel.addRegularUser(c);
				sendMsg(c.getClientFD(), RPL_JOIN(c.getNickname(), c.getUsername(), findingChannel.getName(), c.getClientIP()));
				sendMsg(c.getClientFD(), RPL_NAMREPLY(member, findingChannel.getName(), c.getNickname()));
				sendMsg(c.getClientFD(), RPL_ENDOFNAMES(c.getHostname(), c.getNickname(), findingChannel.getName()));
				findingChannel.sendMsg2Members(*this, c);
			}
			else if (!findingChannel.isMember(c))
				sendMsg(c.getClientFD(), ERR_CANTJOIN(c.getNickname(), findingChannel.getName()));
		}
		else if (findingChannel.getKey() == this->channelPass[i].second){
			if (!findingChannel.isMember(c) && findingChannel.getMode() != "invite-only"){
				member += " " + c.getNickname();
				findingChannel.addRegularUser(c);
				sendMsg(c.getClientFD(), RPL_JOIN(c.getNickname(), c.getUsername(), findingChannel.getName(), c.getClientIP()));
				sendMsg(c.getClientFD(), RPL_NAMREPLY(member, findingChannel.getName(), c.getNickname()));
				sendMsg(c.getClientFD(), RPL_ENDOFNAMES(c.getHostname(), c.getNickname(), findingChannel.getName()));
				findingChannel.sendMsg2Members(*this, c);
			}
			else if (!findingChannel.isMember(c))
				sendMsg(c.getClientFD(), ERR_CANTJOIN(c.getNickname(), findingChannel.getName()));
		}
		else if (!findingChannel.isMember(c))
			sendMsg(c.getClientFD(), ERR_BADCHANNELKEY(c.getNickname(), findingChannel.getName()));
	}
}

void Server::execJoinCommand(Client &c){
	this->channelPass.resize(this->joinChannel.size());
	for(size_t i = 0; i < this->joinChannel.size(); ++i){
		this->channelPass[i].first = this->joinChannel[i];
		if (i < this->joinPassword.size())
			this->channelPass[i].second = this->joinPassword[i];
		else
			this->channelPass[i].second = "";
	}
	for(size_t i=0; i < this->channelPass.size(); ++i){
		if (this->channelPass[i].first[0] != '#' || (this->channelPass[i].first[0] == '#' && !this->channelPass[i].first[1]))
			sendMsg(c.getClientFD(), ERR_BADCHANNELNAME(c.getNickname(), this->channelPass[i].first));
		else if (this->channelPass[i].first[0] == '#' && !isprint(this->channelPass[i].first[1]))
			sendMsg(c.getClientFD(), RPL_ENDOFNAMES(c.getHostname(), c.getNickname(), this->channelPass[i].first));
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

std::string skipCommas(std::string s){
	std::string str = "";
	size_t i = 0;
	for (;s[i] && s[i] == ','; ++i){
	}
	for (; i < s.size() ; ++i){
		if (s[i] == ','){
			for (;s[i] && s[i] == ','; ++i){
			}
			if (s[i] && s[i] != ' ' && s[i] != '\r' && s[i] != '\t')
				i--;
		}
		str += s[i];
	}
	return (str);
}

void Server::joinSingleChannel(void){
	size_t found = this->args.find_first_of(" \r\t\0");
	this->joinChannel.push_back(this->args.substr(0, found));
	if (this->existPassword){
		std::string pass = this->args.substr(found, this->args.length());
		pass = skipSpaces(pass);
		if (pass[0] == ':')
			this->joinPassword.push_back(pass.substr(1, pass.length()));
		else{
			pass = skipCommas(pass);
			size_t found_sp = pass.find_first_of(", \r\t\0");
			this->joinPassword.push_back(pass.substr(0, found_sp));
		}
	}
}


void Server::joinMultiChannels(void){
	std::string temp_args = this->args;
	size_t found = this->args.find_first_of(" \r\t\0");
	std::string channels = temp_args.substr(0, found);
	channels = skipCommas(channels);
	int count_ch = countComma(channels);
	size_t found_commach = channels.find_first_of(",");
	this->joinChannel.push_back(channels.substr(0, found_commach));
	channels = channels.substr(found_commach + 1, channels.length());
	for(int i = 0; i < count_ch; ++i){
		found_commach = channels.find_first_of(",");
		this->joinChannel.push_back(channels.substr(0, found_commach));
		channels = channels.substr(found_commach + 1, channels.length());
	}
	if (this->existPassword){
		std::string passWord = temp_args.substr(found + 1, temp_args.length());
		int count_ps = countComma(passWord);
		passWord = skipSpaces(passWord);
		if (passWord[0] == ':')
			this->joinPassword.push_back(passWord.substr(1, passWord.length()));
		else{
			size_t fSpace = passWord.find_first_of(" \r\t");
			if(fSpace == std::string::npos)
				passWord = passWord.substr(0, passWord.length());
			else
				passWord = passWord.substr(0, fSpace);
			passWord = skipCommas(passWord);
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
}

int Server::argsJoin(void){
	this->args = skipSpaces(this->args);
	size_t found = this->args.find_first_of(" ");
	if (found == std::string::npos) 
		return (0);
	else{
		for (;this->args[found] == ' ' || this->args[found] == '\r' || this->args[found] == '\t'; ++found){
		}
		if (this->args[found])
			return (1);
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
	std::string channels = this->args.substr(0, this->args.find_first_of(" \r\t"));
	size_t foundComma = channels.find_first_of(",");
	if (foundComma != std::string::npos)
		joinMultiChannels();
	else
		joinSingleChannel();
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
