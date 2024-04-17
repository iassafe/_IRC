/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khanhayf <khanhayf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:26:29 by khanhayf          #+#    #+#             */
/*   Updated: 2024/04/16 15:20:02 by khanhayf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::inviteCommand(std::string &args, Client &c){
    if (!c.isRegistered()){
        sendMsg(c.getClientFD(), ERR_NOTREGISTERED(c.getNickname()));
        return ;
    }
    std::stringstream ss(args);
    std::string guest;
    std::string chan;
    if (args[0] == ':'){
        args = args.substr(1);
        ss.clear();
        ss << args;
        getline(ss, guest);}
    else
        ss >> guest;
    getline(ss, chan);
    if (!chan.empty() && chan[0] == ':' && chan[1] == '#'){
        chan = chan.substr(2);
        ss.clear();
        ss << chan;
        chan.clear();
        getline(ss, chan);
    }
    else if (!chan.empty() && chan[1] == '#'){
        chan = chan.substr(1);
        ss.clear();
        ss << chan;
        chan.clear();
        ss >> chan;
    }
    if (guest.empty() || (guest == c.getNickname() && chan.empty())){
        sendMsg(c.getClientFD(), RPL_ENDOFINVITE(c.getNickname()));//End of Invite List
        return ;
    }
    if (isInUseNickname(guest)){
        Client &clt = findClient(guest);
        if (!clt.isRegistered()){
            sendMsg(c.getClientFD(), ERR_NOSUCHNICK(c.getNickname(), guest));
            return ;}
        if (chan[0] == '#')
            chan = chan.substr(1);
        else{
            sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(chan, c.getNickname()));
            return ;
        }
        if (!isInUseChName(chan)){
            sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(("#" + chan), c.getNickname()));
                return ;
        }
        else{
            Channel &ch = findChannel(chan);//If a channel is set to invite-only (mode +i), a channel operator can use the INVITE command to invite a client to join the channel.
            if (!ch.isMember(c)){
                sendMsg(c.getClientFD(), ERR_NOTONCHANNEL(c.getNickname(), ("#" + chan)));
                return ;
            }
            if (ch.isMember(clt)){
                sendMsg(c.getClientFD(), ERR_USERONCHANNEL(("#" + chan), guest, c.getNickname()));
                return ;
            }
            if (!ch.isOperator(c) && ch.getMode() == "invite-only"){
                sendMsg(c.getClientFD(), ERR_NOTOP(c.getNickname(), ("#" + chan)));
                return ;
            }
            // ch.addInvited(clt);
            sendMsg(clt.getClientFD(), RPL_INVITE(c.getNickname(), c.getUsername(), clt.getHostname(), guest, ("#" + chan)));
        }
    }
    else{
        sendMsg(getServerFD(), RPL_INVITING(c.getNickname(), guest, ("#" + chan)));
        sendMsg(c.getClientFD(), ERR_NOSUCHNICK(c.getNickname(), guest));
        return ;
    }
}
