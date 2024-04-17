/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khanhayf <khanhayf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 18:17:33 by khanhayf          #+#    #+#             */
/*   Updated: 2024/04/16 16:40:48 by khanhayf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(Client &creator, std::string chname, Server &s)
:name(chname), topicLock(false), modeLock(false), hasLimit(false), hasKey(false){
    operators.push_back(creator); //the channel creator is considered an operator by default
    s.addChannel(*this);
}

Channel::~Channel(){
    // server->clearChannel(*this);
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
void Channel::setModeLock(bool b){
    modeLock = b;
}
void Channel::setTopicLock(bool b){
    topicLock = b;
}

void Channel::setHasLimit(bool b){
    hasLimit = b;
}
void Channel::setLimit(unsigned int l){
    limit = l;
}

void Channel::setHasKey(bool b){
    hasKey = b;
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

bool Channel::hasALimit(){
    return hasLimit;
}
bool Channel::hasAKey(){
    return hasKey;
}

void Channel::removeRegularUser(Client & c){
    for (unsigned int i = 0; i < regularUsers.size(); i++){
        if (regularUsers[i].getNickname() == c.getNickname()){
            regularUsers.erase(regularUsers.begin() + i);
            break ;
        }
    }
}

void Channel::addOperator(Client & c){
    if (c.isRegistered()){
        if (isRegularuser(c))
            removeRegularUser(c);
        operators.push_back(c);
    }
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
    if (c.isRegistered()){
        if (isOperator(c))
            removeOperator(c);
        regularUsers.push_back(c);
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
// bool Channel::isfull(){
//     if (operators.size() + regularUsers.size() == limit)
//         return true;
//     return false;
// }

// void    Channel::addInvited(Client & c){
//     invited.push_back(c);
// }

// bool    Channel::isInvited(Client & c){
//     if (isMember(c)){
//         for (unsigned int i = 0; i < invited.size(); i++)
//             if (invited[i].getNickname() == c.getNickname())
//                 return true;
//     }
//     return false;
// }
