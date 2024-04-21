#include"Server.hpp"

int Server::validArgsTopic(void){
	this->args = skipSpaces(this->args);
	int count_args = 1;
	for(size_t i =0; i < this->args.length(); ++i){
		if (this->args[i] == ' '){
			count_args++;
			for(; this->args[i] == ' ' || this->args[i] == '\r' || this->args[i] == '\t'; ++i){}
		}
	}
	if (count_args < 2)
		return(0);
	size_t found = this->args.find_first_of(" \t\r");
	std::string temp_args = this->args;
	this->ChannelTopic = temp_args.substr(0, found);
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
	return (1);
}


void Server::execTopicCommand(Client &c){
    if (this->ChannelTopic[0] != '#' || (this->ChannelTopic[0] == '#' && \
			!isprint(this->ChannelTopic[1])))
			sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(this->ChannelTopic, c.getNickname()));
    if (!this->isInUseChName(this->ChannelTopic))
        sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(this->ChannelTopic, c.getNickname()));
    else{
        Channel &findingChannel = this->findChannel(this->ChannelTopic);
        if (findingChannel.isOperator(c)){
            std::cout << "old: " << findingChannel.getTopic() << "  new: " << this->topic << std::endl;
            if (findingChannel.getTopic() != this->topic){
                findingChannel.setTopic(this->topic);
                this->sendMsg(c.getClientFD(), RPL_SETTOPIC(c.getNickname(), this->ChannelTopic, this->topic));
            }
        }
        else if (findingChannel.isRegularuser(c)){
            std::cout << "old: " << findingChannel.getTopic() << "  new: " << this->topic << std::endl;
            if (!findingChannel.isTopiclocked()){
                if (findingChannel.getTopic() != this->topic){
                    findingChannel.setTopic(this->topic);
                    this->sendMsg(c.getClientFD(), RPL_SETTOPIC(c.getNickname(), this->ChannelTopic, this->topic));
                }
            }
            else
                this->sendMsg(c.getClientFD(), ERR_NOACCCHTOPIC(c.getNickname(), this->ChannelTopic));
        }
    }
    
}


void Server::topicCommand(Client &c){
	this->args = skipSpaces(this->args);
	if(validArgsTopic())
		execTopicCommand(c);
	else
		sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), this->command));
}
