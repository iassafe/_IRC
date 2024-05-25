#include "Server.hpp"

bool Server::isValidNickName(std::string nickname){
    nickname = tolowercase(nickname);
    if (!nickname.empty()){
        if (nickname.size() > 30)
            return false;
        std::string special = "-[]`\\^{_}";
        for (unsigned int i = 0; i < nickname.size(); i++){
            if (!isdigit(nickname[i]) && special.find(nickname[i]) == std::string::npos
            && !(nickname[i] >= 'a' && nickname[i] <= 'z'))
                return false;
        }
        if (isdigit(nickname[0]))
            return false;
    }
    else if (nickname.empty() || nickname[0] == '\0')
        return(false);
    return true;
}

void    Server::nickCommand(std::string &args, Client &c){
    if (!c.isPasswordSended())
        return;
    std::stringstream ss(args);
    std::ws(ss);
    std::string param;
    if (ss.peek() != ':')
        ss >> param;
    else{
        getline(ss, param);
        param = param.substr(1);
    }
    if (ss.fail() || !isValidNickName(param)){
        sendMsg(c.getClientFD(), ERR_ERRONEUSNICKNAME(c.getNickname()));
        return;}
    if (c.getNickname() == param)
        return;
    for (unsigned int i = 0;  i < clients.size(); i++){
        if ((tolowercase(clients[i].getNickname()) == tolowercase(param)) && !clients[i].isRegistered())
            clients[i].setNickname("");
    }
    if (isInUseNickname(param)){
        sendMsg(c.getClientFD(), ERR_NICKNAMEINUSE(c.getNickname()));
        return ;}
    if (c.isRegistered()){
        std::string msg = ":" + c.getNickname() + "!~" + c.getUsername() + "@" + c.getClientIP() + " NICK :" + param + "\n";
        sendMsg(c.getClientFD(), msg);
        nickMsgRecievers.push_back(c.getNickname());
        for (unsigned int i = 0; i < channels.size(); i++){
		    if (channels[i].isMember(c)){
                channels[i].sendNickMsg2All(*this, msg);
                channels[i].updateAmemNickName(c, param);
            }
        }
        nickMsgRecievers.clear();
    }
    c.setNickname(param);
    if (!c.isRegistered())
        c.registerClient(*this);
}

void    Server::userCommand(std::string &args, Client &c){
    if (args[0] == ':'){
        sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), "USER"));
        if (c.isRegistered())
			handleError(c);
        return;
    }
    std::istringstream iss(args);
    std::string un, hn, sn, rn;
    iss >> un >> hn >> sn;
    if (iss.fail() || hn[0] == ':' || sn[0] == ':'){
        sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), "USER"));
        if (c.isRegistered())
			handleError(c);
        return ;
    }
    iss >> std::ws;
    if (iss.peek() == ':'){
        getline(iss, rn);
        rn = rn.substr(1);
    }
    else
        iss >> rn;
    if (iss.fail() || un.empty() || hn.empty() || sn.empty() || rn.empty()){
        sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), "USER"));
        if (c.isRegistered())
			handleError(c);
        return;
    }
    else{
        if (!c.isPasswordSended())
            return;
        if (c.isRegistered()){
            sendMsg(c.getClientFD(), ERR_ALREADYREGISTERED(c.getNickname()));
            return;}
        c.setUsername(un);
        c.setHostname(hn);
        c.setServername(sn);
        c.setRealname(rn);
        c.registerClient(*this);
    }
}

void    Server::passCommand(std::string &args, Client &c){
    if (!c.isRegistered())
        c.clearAuthentication();
    if (c.isRegistered()){
        sendMsg(c.getClientFD(), ERR_ALREADYREGISTERED(c.getNickname()));
        return ;}
        std::stringstream ss(args);
        std::ws(ss);
        std::string param;
        if (ss.peek() != ':')
            ss >> param;
        else{
            getline(ss, param);
            param = param.substr(1);
        }
        if (ss.fail() || param != getPassword()){
            sendMsg(c.getClientFD(), ERR_PASSWDMISMATCH(c.getNickname()));
            return;
        }
        else
            c.setPasswordSended(true);
}