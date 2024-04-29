/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responses.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khanhayf <khanhayf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:03:28 by khanhayf          #+#    #+#             */
/*   Updated: 2024/04/29 15:48:47 by khanhayf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSES_HPP
#define RESPONSES_HPP


#define RPL_WELCOME(nick, user) ":ircserv 001 " + nick + " :Welcome to the ft_irc IRC network " + nick + "!~" + user + "@127.0.0.1\r\n"
#define ERR_ALREADYREGISTERED(nick) ":ircserv 462 " + nick + " :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH(nick) ":ircserv 464 " + nick + " :Password incorrect!\r\n"
#define ERR_NEEDMOREPARAMS(nick, cmd) ":ircserv 461 " + nick + " " + cmd + " :Not enough parameters!\r\n"
#define ERR_NICKNAMEINUSE(nick) ":ircserv 433 " + nick + " :Nickname is already in use!\r\n"
// #define ERR_NONICKNAMEGIVEN(nick) ":ircserv 431 " + nick + " :No nickname given!\r\n"
#define ERR_ERRONEUSNICKNAME(nick) ":ircserv 432 " + nick + " :Erroneus nickname!\r\n"
#define ERR_NOTREGISTERED(cmd) ":ircserv 451 * " + cmd + " :You have not registered.\r\n" /////////UPPPPPPPPP
#define ERR_NOSUCHCHANNEL(channel, nick) ":ircserv 403 " + nick + " " + channel + " :No such channel\r\n"
#define ERR_NOSUCHNICK(nick, argument) ":ircserv 401 " + nick + " " + argument + " :No such nick\r\n" 
#define ERR_NOTONCHANNEL(nick, channel) ":ircserv 442 " + channel + " " + ":You're not on that channel\r\n"
#define ERR_NOTOPMODES(nick, channel, mode) ":ircserv 482 " + nick + " " + channel + " " + ":You must have channel halfop access or above to set channel mode " + mode + "\r\n"
#define ERR_NOTOP(nick, channel) ":ircserv 482 " + nick + " " + channel + " " + ":You must be a channel half-operator\r\n"
#define ERR_USERONCHANNEL(channel, guest, nick) ":ircserv 443 " + nick + " " + guest + " " + channel + "  :is already on channel\r\n"
#define RPL_INVITE(nick, username, clienthostname, invited, channel) ":" + nick + "!~" + username + "@" + clienthostname + " INVITE " + invited + " :" + channel + "\r\n"
#define RPL_ENDOFINVITE(nick) ":ircserv 337 " + nick + " :END of INVITE list\r\n"
#define RPL_INVITING(nick, invited, channel) ":ircserv 341 " + nick + " " + invited + " :" + channel + "\r\n"
#define ERR_UNKNOWNMODE(nick, channel, character) ":ircserv 472 " + nick + " " + channel + " " + character + " :is not a recognised channel mode.\r\n"
#define ERR_INVALIDMODEPARAM(nick, channel, flag, mode) ":ircserv 696 " + nick + " "  + channel + " " + flag + " * you must specifiy a parameter for the " + mode + " mode.\r\n"
#define ERR_INVALIDMODELIMITPARAM(nick, channel, argument) ":ircserv 696 " + nick + " " + channel + " " + argument + " :Invalid limit mode parameter.\r\n"
#define ERR_USERNOTINCHANNEL(nick, user, channel) ":ircserv 441 " + nick + " " + user + " " + channel + " " + ":they aren't on that channel\r\n"
#define ERR_KEYALREADYSET(nick, channel) ":ircserv 467 " + nick + " " + channel + " " + ":Channel key already set\r\n"
#define ERR_NOACCCHTOPIC(nick, channel) ":ircserv 482 " + nick + " " + channel + " :You do not have access to change the topic on this channel\r\n" ///////
#define ERR_CANNOTKICK(nick, channel) ":ircserv 482 " + nick + " " + channel + " :You must be a channel half-operator\r\n" ///////
#define RPL_CHANNELMODES(channelName, nick, channelmode) ":ircserv 324 " + nick + " " + channelName + " +" + channelmode + "\r\n"//M


#define RPL_YOURHOST(nick) ":" + "ircserv" + " 002 " + nick + " :Your host is " + "ircserv" + " running version 1.0 !\r\n"
#define RPL_CREATED(nick) ":" + "ircserv" + " 003 " + nick + " :This server was created 2023-9-15 !\r\n"
#define RPL_MYINFO(nick) ":" + "ircserv" + " 004 " + nick + " :Host: " + "ircserv" + ", Version: 1.0, User mode: none, Channel modes: o, t, k, i !\r\n"

//AZMARA
#define ERR_NO_RECIPIENT(nick) "ircserv 411 " + nick + " " + ":No recipient given (PRIVMSG)\r\n"
#define ERR_NO_TEXT(nick) "ircserv 412 " + nick + " " + ":No text to send\r\n"

#define RPL_NICKCHANGE(oldNick, nick) ":" + oldNick + " NICK " + nick + "\r\n"

// #define ERR_ALREADYREGISTERED(nick) ":" + "ircserv" + " 462 " + nick + " :You may not reregister !\r\n"

#define ERR_BADCHANNELNAME(nick, channelname) ":" + "ircserv" + " 476 " + nick + " " + channelname + " :Invalid channel name." + "\r\n"
#define ERR_CHANNELISFULL(nick, channelName) ":" + nick + " 471 " + channelName + " :Cannot join channel (+l)\r\n"
#define ERR_INVITEONLY(nick, channelName) ":" + nick + " 473 " + channelName + " :Cannot join channel (+i)\r\n"

#define RPL_TOPICDISPLAY(hostname, nick, channel, topic) ":" + "ircserv" + " 332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_TOPIC(hostname, nick, channel, setter, topic) ":" + "ircserv" + " 332 " + nick + " " + channel + " :" + setter + " has set a new topic: " + topic + "\r\n"

#define RPL_TOPICWHOTIME(topicsetter, time, nick, channelName) ":" + "ircserv" + " 333 " + nick + " " + channelName + " " + topicsetter + "!~" + topicsetter + "@" + "ircserv" + " " + time + "\r\n"

#define RPL_MODEIS(channel, mode) ":" + "ircserv" + " MODE " + channel + " " + mode + "\r\n"
#define RPL_MODEISLIMIT(channel, mode, newlimit) ":" + "ircserv" + " MODE " + channel + " " + mode + " " + newlimit + "\r\n"
#define RPL_MODEISOP(channel, mode, target) ":" + "ircserv" + " MODE " + channel + " " + mode + " " + target + "\r\n"

#define RPL_CREATIONTIME(hostname, channelName, nick, time) ":" + "ircserv" + " 329 " + nick + " " + channelName + " " + time + "\r\n"



#define ERR_INPUTTOOLONG(nick) ":" + "ircserv" + " 417 " + nick + " :Input line was too long !\r\n"

#define RPL_VIEWTOPIC(hostname, nick, channel, topic) ":" + "ircserv" + " 332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_VIEWTIME(hostname, nick, channel, time) ":" + "ircserv" + " 333 " + nick + " " + channel + " " + nick + " " + time + "\r\n"
#define RPL_SETTOPIC(nick, channel, topic) ":" + nick + "!" + nick + "@" + "ircserv" + " TOPIC " + channel + " :" + topic + "\r\n"

#define RPL_BOT(hostname, nick, message) ":" + "ircserv" + " 001 " + nick + " Dad joke: " + message + "\r\n"
#define ERR_BOT(hostname, nick) ":" + "ircserv" + " 450 " + nick + "enter [BOT generate] to generate a joke" + "\r\n"
#define RPL_UMODEIS(hostname, channelname) ":" + "ircserv" + " MODE " + channelname + " +nt\r\n"

#define RPL_YOUREOPER(hostname, nick) ":" + "ircserv" + " 381 " + nick + ":You are now an IRC operator\r\n"
#define RPL_KICK(kicker, username, host, channel, targetuser) ":" + kicker + "!~" + username + "@" + host + " KICK " + channel + " " + targetuser + " :" + kicker + "\r\n"
#define PRIVMSG_FORMAT(senderNick, senderUsername, senderHostname, receiver, message) ":" + senderNick + "!~" + senderUsername + "@" + senderHostname + " PRIVMSG " + receiver + " :" + message + "\r\n"




///////////
#define RPL_JOIN(nick, username, channelname, ipaddress) ":" + nick + "!~" + username + "@" + ipaddress + " JOIN :" + channelname + "\r\n"
#define RPL_NAMREPLY(clients, channelname, nick) ":ircserv 353 " + nick + " = " + channelname + " :" + clients + "\r\n"
#define RPL_ENDOFNAMES(hostname, nick, channelname) ":ircserv 366 " + nick + " " + channelname + " :END of /NAMES list\r\n"
#define ERR_UNKNOWNCOMMAND(nick, command) (":ircserv 421 " + nick + " " + command + " :Unknown command\r\n")
#define ERR_BADCHANNELKEY(nick, channelName) ":ircserv 475 " + nick + " " + channelName + " :Cannot join channel (+K) - bad key\r\n"
#define ERR_NOTOPIC(channel, nick) ":ircserv 331 " + nick + " " + channel + " :No topic is set.\r\n"
#define ERR_USAGE(nick, command, msg) ":ircserv 650 " + nick + " " + command + " :" + msg + "\r\n"

#endif
