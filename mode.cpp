/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khanhayf <khanhayf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:33:00 by khanhayf          #+#    #+#             */
/*   Updated: 2024/05/17 15:45:36 by khanhayf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::modeCommand(std::string &args, Client &c){
    std::stringstream ss(args);
    std::string chan;
    if (args[0] == ':'){
        if (args[1] != '#'){
            sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(args.substr(1), c.getNickname()));
            return ;}
        ss.clear();
        ss << args.substr(1);
        getline(ss, chan);
        }
    else{
        if (args[0] != '#' && args[0] != ':'){
            sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(args.substr(0, args.find_first_of(" ")), c.getNickname()));
            return ;}
        ss >> chan;}
    if (!isInUseChName(chan)){
        sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(chan, c.getNickname()));
        return ;
    }
    Channel &channel = findChannel(chan);
    if (ss.eof()){ //there is no mode // mode #ch
        sendMsg(c.getClientFD(), RPL_CHANNELMODES(channel.getName(), c.getNickname(), channel.channelModes()));
        return ;
    }
    std::string modestring, key, limit, user, list = "";
    int k, o, l;
    k = o = l = 0;
    std::string modes = "+-litok";
    std::string sign = "+";
    ss >> std::ws;
    if (ss.peek() != ':'){
        while (ss >> modestring){
            ss >>std::ws;
            for (unsigned i = 0; i < modestring.size(); i++){
                if (modes.find(modestring[i]) == std::string::npos)
                    sendMsg(c.getClientFD(), ERR_UNKNOWNMODE(c.getNickname(), chan, modestring[i]));
                else if (!channel.isOperator(c)){
                    std::string m = "";
                    sendMsg(c.getClientFD(), ERR_NOTOPMODES(c.getNickname(), chan, (m += modestring[i])));
                    return ;
                }
                if (modestring[i] == '+' || modestring[i] == '-')
                    sign[0] = modestring[i++];
                if (modestring[i] == 'k'){
                    if (ss.peek() == ':'){
                        getline(ss, key);
                        key = key.substr(1, (key.find_first_of(" ") - 1));}
                    else
                        ss >> key;
                    if (!key.empty()){
                        if (sign[0] == '+'){
                            if (channel.getHasKey()){
                                key.clear();
                                continue ; //do nothing if the Key already set
                                }
                            else{
                                channel.setKey(key);
                                channel.setHasKey(true);
                                std::string msg = ":" + c.getNickname() + "!~" + c.getUsername() + " " + getCommand() + " " + channel.getName() + " " + (sign + modestring[i] + " :") + key + "\n";
                                channel.sendModeMsg2All(*this, msg);
                            }
                        }
                        else{
                            if (channel.getHasKey() && key == channel.getKey()){
                                std::string msg = ":" + c.getNickname() + "!~" + c.getUsername() + " " + getCommand() + " " + channel.getName() + " " + (sign + modestring[i] + " :") + key + "\n";
                                channel.sendModeMsg2All(*this, msg);
                                channel.setKey("");
                                channel.setHasKey(false);
                            }
                            else if (channel.getHasKey() && key != channel.getKey())
                                sendMsg(c.getClientFD(), ERR_KEYALREADYSET(c.getNickname(), chan));
                        }
                        key.clear();
                    }
                    else
                        sendMsg(c.getClientFD(), ERR_INVALIDMODEPARAM(c.getNickname(), chan, 'k', "key"));
                }
                else if (modestring[i] == 'l'){
                    if (ss.peek() == ':'){
                        getline(ss, limit);
                        limit = limit.substr(1);
                    }
                    else
                        ss >> limit;
                    if (!limit.empty() && sign[0] == '+'){
                        unsigned int nb;
                        std::stringstream ss(limit);
                        ss >> nb;
                        if (!ss.fail()){
                            if (!channel.getHasLimit()){
                                channel.setLimit(nb);
                                channel.setHasLimit(true);
                                std::string msg = ":" + c.getNickname() + "!~" + c.getUsername() + " " + getCommand() + " " + channel.getName() + " " + (sign + modestring[i] + " :") + limit + "\n";
                                channel.sendModeMsg2All(*this, msg);}
                        }
                        else
                            sendMsg(c.getClientFD(), ERR_INVALIDMODELIMITPARAM(c.getNickname(), chan, limit));
                    }
                    else if (sign[0] == '-'){
                        if (channel.getHasLimit()){
                            channel.setLimit(0);
                            channel.setHasLimit(false);
                            std::string msg = ":" + c.getNickname() + "!~" + c.getUsername() + " " + getCommand() + " " + channel.getName() + " " + (sign + modestring[i] + "\n");
                            channel.sendModeMsg2All(*this, msg);}
                    }
                    else
                        sendMsg(c.getClientFD(), ERR_INVALIDMODEPARAM(c.getNickname(), chan, 'l', "limit"));
                    limit.clear();
                }
                else if (modestring[i] == 'i'){
                    if (sign[0] == '+'){
                        if (channel.getMode().empty()){
                            channel.setMode("invite-only");
                            std::string msg = ":" + c.getNickname() + "!~" + c.getUsername() + " " + getCommand() + " " + channel.getName() + " " + (sign + modestring[i] + "\n");
                            channel.sendModeMsg2All(*this, msg);}
                    }
                    else{
                            if (!channel.getMode().empty()){
                                channel.setMode("");
                                std::string msg = ":" + c.getNickname() + "!~" + c.getUsername() + " " + getCommand() + " " + channel.getName() + " " + (sign + modestring[i] + "\n");
                                channel.sendModeMsg2All(*this, msg);}
                    }
                }
                else if (modestring[i] == 't'){
                    if (sign[0] == '+'){
                        if (!channel.isTopiclocked()){//KH
                            channel.setTopicLock(true);
                            std::string msg = ":" + c.getNickname() + "!~" + c.getUsername() + " " + getCommand() + " " + channel.getName() + " " + (sign + modestring[i] + "\n");
                            channel.sendModeMsg2All(*this, msg);}
                    }
                    else{
                        if (channel.isTopiclocked()){//KH
                            channel.setTopicLock(false);
                            std::cout << "topic--" << channel.isTopiclocked() << "--\n";
                            std::string msg = ":" + c.getNickname() + "!~" + c.getUsername() + " " + getCommand() + " " + channel.getName() + " " + (sign + modestring[i] + "\n");
                            channel.sendModeMsg2All(*this, msg);}
                    }
                }
                else if (modestring[i] == 'o'){
                    if (ss.peek() == ':'){
                        getline(ss, user);
                        user = user.substr(1);}
                    else
                        ss >> user;
                    if (!user.empty()){
                        if (!isInUseNickname(user))
                            sendMsg(c.getClientFD(), ERR_NOSUCHNICK(c.getNickname(), user));
                        else if (channel.isMember(findClient(user))){
                            if (sign[0] == '+'){
                                if (channel.isRegularuser(findClient(user))){
                                    channel.addOperator(findClient(user)); //check if the nickname is in use and if its registered then add it in addoprator
                                    std::string msg = ":" + c.getNickname() + "!~" + c.getUsername() + " " + getCommand() + " " + channel.getName() + " " + (sign + modestring[i] + " :" + user) + "\n";
                                    channel.sendModeMsg2All(*this, msg);}
                            }
                            else{
                                if (channel.isOperator(findClient(user))){
                                    channel.addRegularUser(findClient(user)); //check if the nickname is in use and if its registered then add it in addoprator
                                    std::string msg = ":" + c.getNickname() + "!~" + c.getUsername() + " " + getCommand() + " " + channel.getName() + " " + (sign + modestring[i] + " :") + user + "\n";
                                    channel.sendModeMsg2All(*this, msg);}
                            }
                        }
                        else
                            sendMsg(c.getClientFD(), ERR_USERNOTINCHANNEL(c.getNickname(), user, chan));
                        user.clear();
                    }
                    else
                        sendMsg(c.getClientFD(), ERR_INVALIDMODEPARAM(c.getNickname(), chan, 'o', "op"));
                }
                if (modestring[i + 1] == '\0')
                    return;
            }
        }
    }
    else{
        getline (ss, modestring);
        modestring = modestring.substr(1);
        for (unsigned int i = 0; i < modestring.size(); i++){
            if (modes.find(modestring[i]) == std::string::npos){
                sendMsg(c.getClientFD(), ERR_UNKNOWNMODE(c.getNickname(), chan, modestring[i]));
                continue;
            }
            if (modestring[i] == '+' || modestring[i] == '-')
                sign[0] = modestring[i++];
            if (modestring[i] == 'k' || modestring[i] == 'l' || modestring[i] == 'o'){
                if (modestring[i] == 'k')
                    sendMsg(c.getClientFD(), ERR_INVALIDMODEPARAM(c.getNickname(), chan, 'k', "key"));
                else if (modestring[i] == 'l')
                    sendMsg(c.getClientFD(), ERR_INVALIDMODEPARAM(c.getNickname(), chan, 'l', "limit"));
                else if (modestring[i] == 'o')
                    sendMsg(c.getClientFD(), ERR_INVALIDMODEPARAM(c.getNickname(), chan, 'o', "op"));
            }
            else{
                if (!channel.isOperator(c)){
                    std::string m = "";
                    sendMsg(c.getClientFD(), ERR_NOTOPMODES(c.getNickname(), chan, (m += modestring[i])));
                    continue ;
                }
                if (modestring[i] == 'i'){
                    if (sign[0] == '+'){
                        channel.setMode("invite-only");
                        std::cout << "mode--" << channel.getMode() << "--\n";
                    }
                    else{
                        channel.setMode("");
                        std::cout << "mode--" << channel.getMode() << "--\n";
                    }
                }
                else if (modestring[i] == 't'){
                    if (sign[0] == '+'){
                        channel.setTopicLock(true);
                        std::cout << "topic--" << channel.isTopiclocked() << "--\n";
                    }
                    else{
                        channel.setTopicLock(false);
                        std::cout << "topic--" << channel.isTopiclocked() << "--\n";
                    }
                }
                
            }
        }
    }
}
