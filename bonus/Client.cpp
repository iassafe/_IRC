
#include "Client.hpp"

Client::Client(){
    PasswordSended = registered = false;
    clientFD = -1;
}

Client::Client(Client const& obj){
    *this = obj;
}

Client& Client::operator=(Client const& obj){
    if (this != &obj){
        clientFD = obj.clientFD;
        clientIP = obj.clientIP;
        buffer = obj.buffer;
        nickname = obj.nickname;
        username = obj.username;
        hostname = obj.hostname;
        servername = obj.servername;
        realname = obj.realname;
        registered = obj.registered;
        PasswordSended = obj.PasswordSended;
        invited2channels = obj.invited2channels;
    }
    return (*this);
}

Client::~Client(){
    this->invited2channels.clear();
}

void	Client::setClientIP(std::string IPaddr){
	this->clientIP = IPaddr;
}
void	Client::setBuffer(std::string rec){
	this->buffer += rec;
}
void	Client::setClientFD(int fd){
	this->clientFD = fd;
}

void Client::clearBuffer(){
    this->buffer.clear();
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

std::string Client::getBuffer() const{
    return (this->buffer);
}
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
        s.sendMsg(getClientFD(), "\r\n");
        s.sendMsg(getClientFD(), ":ircserv 001 " + getNickname() + " :Welcome to the ft_irc IRC network " + getNickname() + "!\r\n");
        s.sendMsg(getClientFD(), ":ircserv 002 " + getNickname() + " :Your host is ircserv\r\n");
        s.sendMsg(getClientFD(), ":ircserv 003 " + getNickname() + " :This server was created Avr 2024\r\n");
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
    }
}

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
        if (tolowercase(this->invited2channels[i]) == tolowercase(ChannelName))
            return true;
    }
    return false;
}

void Client::removeInvitedCh(std::string ChannelName){
    for (unsigned int i = 0; i < this->invited2channels.size(); i++){
        if (tolowercase(this->invited2channels[i]) == tolowercase(ChannelName)){
            this->invited2channels.erase(this->invited2channels.begin() + i);
            break ;
        }
    }
}

void    Client::invite2channel(std::string chName){
    if (!isInUseInvitedCh(chName))
        invited2channels.push_back(chName);
}


