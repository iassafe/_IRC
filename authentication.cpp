/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassafe <iassafe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 15:39:46 by khanhayf          #+#    #+#             */
/*   Updated: 2024/05/12 13:02:45 by iassafe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
        if (nickname == "nickserv" || nickname == "chanserv") //are often reserved or restricted by the IRC server for other server services.
            return false ;
    }
    else
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
    if (c.getNickname() == param) //in case a client try to change his nn with the same nickname setted before
        return;
    if (!isValidNickName(param)){
        sendMsg(c.getClientFD(), ERR_ERRONEUSNICKNAME(c.getNickname()));
        return;}
    for (unsigned int i = 0;  i < clients.size(); i++){
        if ((tolowercase(clients[i].getNickname()) == tolowercase(param)) && !clients[i].isRegistered())
            clients[i].setNickname("");
    }
    if (isInUseNickname(param)){// check this only if new client 
        sendMsg(c.getClientFD(), ERR_NICKNAMEINUSE(c.getNickname()));
        return ;}
    if (c.isRegistered()){
        std::string msg = ":" + c.getNickname() + "!~" + c.getUsername() + "@" + " NICK :" + param + "\n";
        sendMsg(c.getClientFD(), msg);
        sendNickMsg2Mem(msg, c);
        for (unsigned int i = 0; i < channels.size(); i++){
		    if (channels[i].isMember(c))
                channels[i].updateAmemNickName(c, param);
        }
    }
    c.setNickname(param);
    if (!c.isRegistered())
        c.registerClient(*this);
}

void    Server::userCommand(std::string &args, Client &c){
    if (args[0] == ':'){//it's not standard practice to use colons before every parameter in IRC commands.//the colon is reserved specifically for the trailing(last) parameter.
        sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname(), "USER"));
        if (c.isRegistered())
			handleError(c);
        return;
    }
    std::istringstream iss(args);
    std::string un, hn, sn, rn;
    iss >> un >> hn >> sn;
    if (hn[0] == ':' || sn[0] == ':'){
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
        if (c.isRegistered()){
            for (unsigned int i = 0;  i < clients.size(); i++){
                if ((tolowercase(clients[i].getNickname()) == tolowercase(c.getNickname())) && !clients[i].isRegistered())
                    clients[i].setNickname("");
            }
        }
    }
}

void    Server::passCommand(std::string &args, Client &c){
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
        if (param != getPassword()){
            sendMsg(c.getClientFD(), ERR_PASSWDMISMATCH(c.getNickname()));
            return;
        }
        else
            c.setPasswordSended(true);
}