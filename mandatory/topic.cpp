#include"Server.hpp"

int Server::validArgsTopic(void){
	this->args = skipSpaces(this->args);
	if (this->args == "")
		return (1);
	int count_args = 1;
	for(size_t i =0; i < this->args.length(); ++i){
		if (this->args[i] == ' '|| this->args[i] == '\r' || this->args[i] == '\t'){
			for(; this->args[i] == ' ' || this->args[i] == '\r' || this->args[i] == '\t'; ++i){}
			if(this->args[i])
				count_args++;
		}
	}
	size_t found = this->args.find_first_of(" \t\r");
	std::string temp_args = this->args;
	if (found != std::string::npos)
		this->ChannelTopic = temp_args.substr(0, found);
	else
		this->ChannelTopic = temp_args.substr(0, temp_args.length());
	this->topic = "";
	if (count_args == 1)
		return (0);
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
	this->topic = temp_args;
	return (0);
}

static std::string getCurrentTime(void)
{
	std::time_t currentTime = std::time(NULL);
	std::stringstream result;
	result << currentTime;
	return result.str();
}

void Server::execTopicCommand(Client &c){
    if (!this->isInUseChName(this->ChannelTopic))
        sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(this->ChannelTopic, c.getNickname()));
    else{
        Channel &findingChannel = this->findChannel(this->ChannelTopic);
		if (findingChannel.isMember(c)){
			if (this->topic != ""){
				if (findingChannel.isOperator(c)){
					if (findingChannel.getTopic() != this->topic){
						findingChannel.setStringTime(getCurrentTime());
						findingChannel.setTopic(this->topic);
						findingChannel.sendMsgTopic(c, *this);
					}
				}
				else if (findingChannel.isRegularuser(c)){
					if (!findingChannel.isTopiclocked()){
						findingChannel.setStringTime(getCurrentTime());
						if (findingChannel.getTopic() != this->topic){
							findingChannel.setTopic(this->topic);
							findingChannel.sendMsgTopic(c, *this);
						}
					}
					else
						this->sendMsg(c.getClientFD(), ERR_NOACCCHTOPIC(c.getNickname(), this->ChannelTopic));
				}
			}
			else{
				if (findingChannel.getTopic() == "")
					sendMsg(c.getClientFD(), ERR_NOTOPIC(this->ChannelTopic, c.getNickname()));
				else{
					sendMsg(c.getClientFD(), RPL_TOPICDISPLAY(c.getHostname(), c.getNickname(), findingChannel.getName(), findingChannel.getTopic()));
					sendMsg(c.getClientFD(), RPL_TOPICWHOTIME(findingChannel.getTopic(), findingChannel.getStringTime(), c.getNickname(), findingChannel.getName()));
				}
			}
		}
		else
			sendMsg(c.getClientFD(), ERR_NOTONCHANNEL(c.getNickname(), findingChannel.getName()));
    }
    
}

void Server::topicCommand(Client &c){
	this->args = skipSpaces(this->args);
	int check = validArgsTopic();
	if(!check)
		execTopicCommand(c);
	else
		sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), this->command));
}
