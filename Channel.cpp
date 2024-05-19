#include "Channel.hpp"

Channel::Channel(Client &creator, std::string chname, Server &s)
:name(chname), topicLock(true), hasLimit(false), hasKey(false){
    this->operators.push_back(creator);
    this->mode = "";
    s.addChannel(*this);
}

Channel::~Channel(){
    this->regularUsers.clear();
    this->operators.clear();
}


void Channel::setMode(std::string newMode){
    this->mode = newMode;
}
void Channel::setTopic(std::string newTopic){
    this->topic = newTopic;
}
void Channel::setKey(std::string k){
    this->key = k;
}

void Channel::setTopicLock(bool b){
    this->topicLock = b;
}

void Channel::setHasLimit(bool b){
    this->hasLimit = b;
}
void Channel::setLimit(unsigned int l){
    this->limit = l;
}

void Channel::setHasKey(bool b){
    this->hasKey = b;
}

std::string Channel::getName() const{
    return this->name;
}
std::string Channel::getTopic() const{
    return this->topic;
}
std::string Channel::getMode() const{
    return this->mode;
}
std::string Channel::getKey() const{
    return this->key;
}
int Channel::getLimit() const{
    return this->limit;
}

bool Channel::isTopiclocked() const{
    return this->topicLock;
}

bool Channel::getHasLimit(){
    return this->hasLimit;
}
bool Channel::getHasKey(){
    return this->hasKey;
}

void Channel::removeRegularUser(Client & c){
    for (unsigned int i = 0; i < this->regularUsers.size(); i++){
        if (toLowerCase(this->regularUsers[i].getNickname()) == toLowerCase(c.getNickname())){
            this->regularUsers.erase(this->regularUsers.begin() + i);
            break ;
        }
    }
}

void Channel::addOperator(Client & c){
    if (c.isRegistered()){
        if (isRegularuser(c))
            removeRegularUser(c);
        this->operators.push_back(c);
    }
}

void Channel::removeOperator(Client & c){
    for (unsigned int i = 0; i < this->operators.size(); i++){
        if (toLowerCase(this->operators[i].getNickname()) == toLowerCase(c.getNickname())){
            this->operators.erase(this->operators.begin() + i);
            break ;
        }
    }
}

void Channel::addRegularUser(Client &c){
    if (c.isRegistered()){
        if (isOperator(c))
            removeOperator(c);
        this->regularUsers.push_back(c);
    }
}

bool Channel::isOperator(Client const& c){
    for (unsigned int i = 0; i < this->operators.size(); ++i){
        if (toLowerCase(this->operators[i].getNickname()) == toLowerCase(c.getNickname()))
            return true;
    }
    return false;
}
bool Channel::isRegularuser(Client const& c){
    for (unsigned int i = 0; i < this->regularUsers.size(); ++i){
        if (toLowerCase(this->regularUsers[i].getNickname()) == toLowerCase(c.getNickname()))
            return true;
    }
    return false;
}
bool Channel::isMember(Client const& c){
    if (isOperator(c) || isRegularuser(c))
        return true;
    return false;
}

void Channel::sendMsg2Members(Server &s, Client &c){
    for(size_t i = 0; i < this->regularUsers.size(); ++i){
        if (toLowerCase(this->regularUsers[i].getNickname()) != toLowerCase(c.getNickname()))
            s.sendMsg(this->regularUsers[i].getClientFD(), RPL_JOIN(c.getNickname(), \
                c.getUsername(), this->getName(), this->regularUsers[i].getClientIP()));
    }
    for(size_t i = 0; i < this->operators.size(); ++i){
        if (toLowerCase(this->operators[i].getNickname()) != toLowerCase(c.getNickname()))
            s.sendMsg(this->operators[i].getClientFD(), RPL_JOIN(c.getNickname(), \
                c.getUsername(), this->getName(), this->operators[i].getClientIP()));
    }
}

void	Channel::sendmsg2chanRegulars(Server s, Client cli, std::string& message, Channel ch){
	for (size_t i = 0; i < this->regularUsers.size(); ++i){
        if (cli.getNickname() != this->regularUsers[i].getNickname())
		    s.sendMsg(regularUsers[i].getClientFD(), MESSAGE(cli.getNickname(), ch.getName(), message, cli.getUsername(), cli.getClientIP()));
	}
}

void	Channel::sendmsg2chanOperators(Server s, Client cli, std::string& message, Channel ch){
	for (size_t i = 0; i < this->operators.size(); ++i){
        if (cli.getNickname() != this->operators[i].getNickname())
		    s.sendMsg(operators[i].getClientFD(), MESSAGE(cli.getNickname(), ch.getName(), message, cli.getUsername(), cli.getClientIP()));
	}
}

void Channel::sendNickMsg2All(Server &s, std::string message){
	for (size_t i = 0; i < this->regularUsers.size(); ++i){
        if (!s.msgAlreadyRecieved(this->regularUsers[i].getNickname())){
		    s.sendMsg(this->regularUsers[i].getClientFD(), message);
            s.nickMsgRecievers.push_back(this->regularUsers[i].getNickname());
        }
	}
    for (size_t i = 0; i < this->operators.size(); ++i){
        if (!s.msgAlreadyRecieved(this->operators[i].getNickname())){
		    s.sendMsg(this->operators[i].getClientFD(), message);
            s.nickMsgRecievers.push_back(this->regularUsers[i].getNickname());
        }
	}
}

void Channel::sendModeMsg2All(Server &s, std::string message){
	for (size_t i = 0; i < this->regularUsers.size(); ++i)
		s.sendMsg(this->regularUsers[i].getClientFD(), message);
    for (size_t i = 0; i < this->operators.size(); ++i)
		    s.sendMsg(this->operators[i].getClientFD(), message);
}

bool Channel::hasLimitCanJ(void){
    if((this->operators.size() + this->regularUsers.size()) >= this->limit)
        return (false);
    return (true);
}

std::string Channel::makeStringMember(void){
	std::string member = "";
	for(size_t i = 0; i < this->operators.size(); ++i){
		if (i != 0)
			member += " ";
		member += "@" + this->operators[i].getNickname();
	}
	for(size_t i = 0; i < this->regularUsers.size(); ++i){
		member += " " + this->regularUsers[i].getNickname();
	}
	return (member);
}

std::string Channel::channelModes(){
    std::string str = "";
    if (this->getHasKey())
        str += "k";
    if (this->getHasLimit())
        str += "l";
    if (getMode() == "invite-only")
        str += "i";
    if (this->isTopiclocked())
        str += "t";
    if (!str.empty()){
        for (unsigned int i = 0; str[i] != ' '; i++){
            if (str[i] == 'k' && getHasKey()){
                str += " ";
                str += getKey();
            }
            else if (str[i] == 'l' && getHasLimit()){
                str += " ";
                std::stringstream ss;
                ss << this->getLimit();
                str += ss.str();
            }
        }
    }
    return (str);
}

void	Channel::updateAmemNickName(Client &c, std::string newNick){
    for (unsigned int i = 0; i < this->operators.size(); i++){
        if (toLowerCase(this->operators[i].getNickname()) == toLowerCase(c.getNickname())){
            this->operators[i].setNickname(newNick);
            return ;
        }
    }
    for (unsigned int i = 0; i < this->regularUsers.size(); i++){
        if (toLowerCase(this->regularUsers[i].getNickname()) == toLowerCase(c.getNickname())){
            this->regularUsers[i].setNickname(newNick);
            return ;
        }
    }
}

std::string    Channel::toLowerCase(std::string str){
    if (!str.empty()){
        for (unsigned int i = 0; i < str.size(); ++i)
            str[i] = std::tolower(str[i]);
    }
    return (str);
}

void Channel::sendMsgKick2Members(Server &s, Client &c, std::string name, std::string reason){
    for (size_t i = 0; i < this->regularUsers.size(); ++i){
        if(reason == "")
            reason = c.getNickname();
		s.sendMsg(this->regularUsers[i].getClientFD(), RPL_KICK(c.getNickname(), \
        c.getUsername(), this->getName(), name, reason, this->regularUsers[i].getClientIP()));
	}
    for (size_t i = 0; i < this->operators.size(); ++i){
        if(reason == "")
            reason = c.getNickname();
		s.sendMsg(this->operators[i].getClientFD(), RPL_KICK(c.getNickname(), \
        c.getUsername(), this->getName(), name, reason, this->operators[i].getClientIP()));
	}
}

size_t Channel::getSizeMembers(void){
    return (this->operators.size() + this->regularUsers.size());
}
void Channel::setStringTime(std::string str){
    this->stringtime = str;
}

std::string Channel::getStringTime(void) const{
    return (this->stringtime);
}
