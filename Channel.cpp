
#include "Channel.hpp"

Channel::Channel(Client &creator, std::string chname):name(chname), limit(10), topicLock(false), modeLock(false){
    operators.push_back(creator); //the channel creator is considered an operator by default
    server.addChannel(*this); //add channel in the server container
}

Channel::~Channel(){
    server.removeChannel(*this);
}

//setters
void Channel::setMode(std::string newMode){
    mode = newMode;
}
void Channel::setTopic(std::string newTopic){
    topic = newTopic;
}
void Channel::setKey(std::string k){
    key = k;
}
void Channel::setModelock(bool b){
    modeLock = b;
}
void Channel::setTopiclock(bool b){
    topicLock = b;
}

//getters
std::string Channel::getName() const{
    return name;
}
std::string Channel::getTopic() const{
    return topic;
}
std::string Channel::getMode() const{
    return mode;
}
std::string Channel::getKey() const{
    return key;
}
int Channel::getlimit() const{
    return limit;
}
bool Channel::isModelocked() const{
    return modeLock;
}
bool Channel::isTopiclocked() const{
    return topicLock;
}
void Channel::addOperator(Client & c){
    if (isRegularuser(c))
        removeRegularUser(c);
    operators.push_back(c); 
}
void Channel::removeOperator(Client & c){
    for (unsigned int i = 0; i < operators.size(); i++){
        if (operators[i].getNickname() == c.getNickname()){
            operators.erase(operators.begin() + i);
            break ;
        }
    }
}

void Channel::addRegularUser(Client & c){
    if (isOperator(c))
        removeOperator(c);
    regularUsers.push_back(c);
}

void Channel::removeRegularUser(Client & c){
    for (unsigned int i = 0; i < regularUsers.size(); i++){
        if (regularUsers[i].getNickname() == c.getNickname()){
            regularUsers.erase(regularUsers.begin() + i);
            break ;
        }
    }
}

bool Channel::isOperator(Client const& c) const{
    for (unsigned int i = 0; i < operators.size(); i++){
        if (operators[i].getNickname() == c.getNickname())
        return true;
    }
    return false;
}
bool Channel::isRegularuser(Client const& c) const{
    for (unsigned int i = 0; i < regularUsers.size(); i++){
        if (regularUsers[i].getNickname() == c.getNickname())
        return true;
    }
    return false;
}
bool Channel::isMember(Client const& c){
    if (isOperator(c) || isRegularuser(c))
        return true;
    return false;
}
bool Channel::isfull(){
    if (operators.size() + regularUsers.size() == limit)
        return true;
    return false;
}
