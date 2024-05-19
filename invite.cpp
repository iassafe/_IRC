/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khanhayf <khanhayf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:26:29 by khanhayf          #+#    #+#             */
/*   Updated: 2024/05/19 14:05:47 by khanhayf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::inviteCommand(std::string &args, Client &c){
    std::stringstream ss(args);
    std::string guest;
    std::string chan;
    if (args[0] == ':'){
        ss.clear();
        ss << args.substr(1);
        getline(ss, guest);}
    else
        ss >> guest;
    if (guest.empty() || ss.eof()){
        sendMsg(c.getClientFD(), RPL_ENDOFINVITE(c.getNickname()));
        return ;
    }
    else{
        if (ss.peek() == ':'){
            getline(ss, chan);
            chan = chan.substr(1);
        }
        else
            ss >> chan;
    }
    if (chan[0] != '#' || !isInUseChName(chan)){
        sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(chan, c.getNickname()));
        return ;
    }
    if (!isInUseNickname(guest)){
        sendMsg(c.getClientFD(), ERR_NOSUCHNICK(c.getNickname(), guest));
        return ;}
    else{
        Client &clt = findClient(guest);
        Channel &ch = findChannel(chan);
        if (!ch.isMember(c)){
            sendMsg(c.getClientFD(), ERR_NOTONCHANNEL(c.getNickname(), chan));
            return ;
        }
        if (ch.isMember(clt)){
            sendMsg(c.getClientFD(), ERR_USERONCHANNEL(chan, guest, c.getNickname()));
            return ;
        }
        if (!ch.isOperator(c)){
            sendMsg(c.getClientFD(), ERR_NOTOP(c.getNickname(), chan));
            return ;
        }
        sendMsg(c.getClientFD(), RPL_INVITING(c.getNickname(), guest, ch.getName())); 
        sendMsg(clt.getClientFD(), RPL_INVITE(c.getNickname(), c.getUsername(), clt.getClientIP(), guest, ch.getName()));
        clt.invite2channel(chan);
    }
}