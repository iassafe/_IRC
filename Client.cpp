/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khanhayf <khanhayf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 18:17:06 by khanhayf          #+#    #+#             */
/*   Updated: 2024/04/29 14:16:05 by khanhayf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"


Client::Client(){
    PasswordSended = registered = false;
    clientFD = -1;
    // std::cout << "++++++++was in constructor\n+++++++\n";
}
Client::~Client(){
}

//setters
void	Client::setIP(std::string IPaddr){
	this->clientIP = IPaddr;
}
void	Client::setBuffer(std::string rec){
	this->buffer = rec;
}
void	Client::setClientFD(int fd){
	this->clientFD = fd;
}

void Client::setNickname(std::string nn){
    nickname = nn;
}
void Client::setUsername(std::string un){
    username = un;
}
void Client::setHostname(std::string hn){
    hostname = hn;
}
void Client::setServername(std::string sn){
    servername = sn;
}
void Client::setRealname(std::string rn){
    realname = rn;
}

void Client::setRegistered(bool b){
    registered = b;
}
void Client::setPasswordSended(bool b){
    PasswordSended = b;
}

//getters
int Client::getClientFD(){
	return (clientFD);
}

std::string Client::getNickname() const{
    return (nickname);
}
std::string Client::getUsername() const{
    return (username);
}
std::string Client::getHostname() const{
    return (hostname);
}
std::string Client::getRealname() const{
    return (realname);
}
std::string Client::getServername() const{
    return (servername);
}
bool Client::isRegistered() const{
    return (registered);
} 
bool    Client::isPasswordSended(){
    return PasswordSended;
}

//others
void    Client::clearAuthentication(){
    nickname.clear();
    username.clear();
    hostname.clear();
    servername.clear();
    realname.clear();
    setPasswordSended(false);
    
}
void Client::registerClient(Server &s){
    if (isPasswordSended() && !nickname.empty() && !username.empty()
    && !hostname.empty() && !servername.empty() && !realname.empty()){
        registered = true;
        s.sendMsg(getClientFD(), RPL_WELCOME(getNickname(), getUsername()));
        s.sendMsg(getClientFD(), ":ircserv 375 " + getNickname() + " ::ircserv message of the day\r\n");
        s.sendMsg(getClientFD(), ":ircserv 372 " + getNickname() + " :  Hello, World!\r\n");
        s.sendMsg(getClientFD(), ":ircserv 372 " + getNickname() + " :\r\n");
        s.sendMsg(getClientFD(), ":ircserv 372 " + getNickname() + " :  Welcome to the\r\n");
        s.sendMsg(getClientFD(), ":ircserv 372 " + getNickname() + " :        __  .______        ______     _______. _______ .______     ____    ____ \r\n");
        s.sendMsg(getClientFD(), ":ircserv 372 " + getNickname() + " :       |  | |   _  \\      /      |   /       ||   ____||   _  \\    \\   \\  /   / \r\n");
        s.sendMsg(getClientFD(), ":ircserv 372 " + getNickname() + " :       |  | |  |_)  |    |  ,----'  |   (----`|  |__   |  |_)  |    \\   \\/   /  \r\n");
        s.sendMsg(getClientFD(), ":ircserv 372 " + getNickname() + " :       |  | |      /     |  |        \\   \\    |   __|  |      /      \\      /   \r\n");
        s.sendMsg(getClientFD(), ":ircserv 372 " + getNickname() + " :       |  | |  |\\  \\----.|  `----.----)   |   |  |____ |  |\\  \\----.  \\    /    \r\n");
        s.sendMsg(getClientFD(), ":ircserv 372 " + getNickname() + " :       |__| | _| `._____| \\______|_______/    |_______|| _| `._____|   \\__/     \r\n");
        s.sendMsg(getClientFD(), ":ircserv 372 " + getNickname() + " :                                                               AUTONOMOUS ZONE\r\n");
        s.sendMsg(getClientFD(), ":ircserv 372 " + getNickname() + " :                                                                                      \r\n");
        s.sendMsg(getClientFD(), ":ircserv 372 " + getNickname() + " :  Thank you for using ircserv!\r\n");
        s.sendMsg(getClientFD(), ":ircserv 372 " + getNickname() + " :  End of message of the day.\r\n");
    }
        std::cout << "------registered successfully------\n";
        std::cout << "nn = " << getNickname() << "\n";
        std::cout << "un = " << getUsername() << "\n";
        std::cout << "hn = " << getHostname() << "\n";
        std::cout << "sn = " << getServername() << "\n";
        std::cout << "rn = " << getRealname() << "\n";
        std::cout << "pw = " << isPasswordSended() << "\n";
        std::cout << "registered = " << isRegistered() << "\n";
}

/////////////////IK
std::string Client::getClientIP() const{
    return (this->clientIP);
}

std::string    Client::tolowercase(std::string str){
    if (!str.empty()){
        for (unsigned int i = 0; i < str.size(); ++i)
            str[i] = std::tolower(str[i]);
    }
    return (str);
}

bool    Client::isInUseInvitedCh(std::string ChannelName){
   ChannelName = tolowercase(ChannelName);
    for (unsigned int i = 0; i < this->invited2channels.size(); ++i){
        if (tolowercase(this->invited2channels[i]) == ChannelName)
            return true;
    }
    return false;
}

// std::string& Client::findingInvitedCh(std::string ChannelName){
//     unsigned int i;
// 	ChannelName = tolowercase(ChannelName);
// 	for (i = 0; i < this->invited2channels.size(); i++){
// 		if (tolowercase(this->invited2channels[i]) == ChannelName)
// 			return (this->invited2channels[i]);
// 	}
// 	return (this->invited2channels[i]);//channels end if not found
// }

void Client::removeInvitedCh(std::string ChannelName){
    for (unsigned int i = 0; i < this->invited2channels.size(); i++){
        if (this->invited2channels[i] == ChannelName){
            this->invited2channels.erase(this->invited2channels.begin() + i);
            break ;
        }
    }
}

void    Client::invite2channel(std::string chName){
    if (!isInUseInvitedCh(chName))
        invited2channels.push_back(chName);
}
