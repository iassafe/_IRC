/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   authentication.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khanhayf <khanhayf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 15:39:46 by khanhayf          #+#    #+#             */
/*   Updated: 2024/04/16 16:22:44 by khanhayf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    tolowercase(std::string &str){
    for (unsigned int i = 0; i < str.size(); i++)
        str[i] = std::tolower(str[i]);
}

bool isValidNickName(std::string nickname){
    if (!nickname.empty()){
        if (nickname.size() > 30)
            return false;//Nicknames can have a maximum length of 9 characters.
        std::string special = "-[]`\\^{_}";
        for (unsigned int i = 0; i < nickname.size(); i++){
            if (!isdigit(nickname[i]) && special.find(nickname[i]) == std::string::npos
            && !(nickname[i] >= 'a' && nickname[i] <= 'z')) //no need to check uppercase alphabets since nichname is already converted to lowercase in this step
                return false; //consist of letters (a-z, A-Z), numbers (0-9), and certain special characters, such as underscore (_), hyphen (-), and backslash ().
        }
        if (isdigit(nickname[0])) //The first character of a nickname must be a letter or a special character
            return false;
        if (nickname == "nickserv" || nickname == "chanserv") //are often reserved or restricted by the IRC server for other server services.
            return false ;
    }
    else
        return(false);//empty nickname
    return true;
}

void    Server::nickCommand(std::string &args, Client &c){
    if (args.empty()){
        sendMsg(c.getClientFD(), ERR_NONICKNAMEGIVEN(c.getNickname()));
        return ;
    }
    tolowercase(args); //Nicknames are generally case-insensitive
    std::string param;
    if (args[0] == ':')
        param = args.substr(1); //starting from index 1
    else
        param = args.substr(0, args.find_first_of(' '));
    if (!isValidNickName(param)){
        sendMsg(c.getClientFD(), ERR_ERRONEUSNICKNAME(c.getNickname()));
        return;}
    if (isInUseNickname(param)){// check this only if new client
        if (c.getNickname() == param) //in case a client try to change his nn with the same nickname setted before
            return;
    sendMsg(c.getClientFD(), ERR_NICKNAMEINUSE(c.getNickname()));
    return ;}
    c.setNickname(param); //in both case: choosing a nname for the first time or changing a nname 
    if (!c.isRegistered()) //he can change the nick name after registration 
    c.registerClient(*this);//client become registred in the server if the condition inside registerClient is true
}

void    Server::userCommand(std::string &args, Client &c){
    if (c.isRegistered()){
        sendMsg(c.getClientFD(), ERR_ALREADYREGISTERED(c.getNickname()));
        return;}
    if (args[0] == ':'){//it's not standard practice to use colons before every parameter in IRC commands.//the colon is reserved specifically for the trailing(last) parameter.
        sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname()));
        return;
    }
    std::istringstream iss(args);
    std::string un, hn, sn, rn;
    iss >> un >> hn >> sn;
    if (hn[0] == ':' || sn[0] == ':'){
        sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname()));
        return ;
    }
    iss >> std::ws;
    if (iss.peek() == ':'){
        getline(iss, rn);
        rn = rn.substr(1);
    }
    else
        iss >> rn;
    if (!iss.fail() && !un.empty() && !hn.empty() && !sn.empty() && !rn.empty()){
        c.setUsername(un);
        c.setHostname(hn);
        c.setServername(sn);
        c.setRealname(rn);
        c.registerClient(*this);//client become registred in the server if the condition inside registerClient is true
    }
    else
        sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname()));
}

void    Server::passCommand(std::string &args, Client &c){
    c.clearAuthentication();
    if (c.isRegistered()){
        sendMsg(c.getClientFD(), ERR_ALREADYREGISTERED(c.getNickname()));
        return ;}
    if (!args.empty() && args[0]){
        std::string param;
        if (args[0] == ':')
            param = args.substr(1);
        else
            param = args.substr(0, args.find_first_of(' '));
        if (param != getPassword()){
            sendMsg(c.getClientFD(), ERR_PASSWDMISMATCH(c.getNickname()));
            return;
        }
        else{
            c.setPasswordSended(true);  //leave a mark if pass cmd succeed the first time
            c.registerClient(*this);//client become registred in the server if the condition inside registerClient is true
        }
    }
    else
        sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname()));
}
