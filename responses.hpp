

#ifndef RESPONSES_HPP
#define RESPONSES_HPP

#define ERR_ALREADYREGISTERED(nick) ":ircserv 462 " + nick + " :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH(nick) ":ircserv 464 " + nick + " :Password incorrect!\r\n"
#define ERR_NEEDMOREPARAMS(nick, cmd) ":ircserv 461 " + nick + " " + cmd + " :Not enough parameters!\r\n"
#define ERR_NICKNAMEINUSE(nick) ":ircserv 433 " + nick + " :Nickname is already in use!\r\n"
#define ERR_ERRONEUSNICKNAME(nick) ":ircserv 432 " + nick + " :Erroneus nickname!\r\n"
#define ERR_NOTREGISTERED(cmd) ":ircserv 451 * " + cmd + " :You have not registered.\r\n"
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
#define ERR_NOACCCHTOPIC(nick, channel) ":ircserv 482 " + nick + " " + channel + " :You do not have access to change the topic on this channel\r\n"
#define ERR_CANNOTKICK(nick, channel, msg) ":ircserv 482 " + nick + " " + channel + msg
#define RPL_CHANNELMODES(channelName, nick, channelmode) ":ircserv 324 " + nick + " " + channelName + " :" + channelmode + "\r\n"

#define MESSAGE(sender, reciever, message, user, IP) ":" + sender + "!~" + user + "@" + IP + " PRIVMSG " + reciever + " :" + message + "\r\n"
#define ERR_NO_TEXT(nick) "ircserv 412 " + nick + " " + ":No text to send\r\n"
#define ERR_CANNOTSENDTOCHANNEL(channel, nick) ":ircserv 404 " + nick + " " + channel + " :Cannot send to channel\r\n"

#define ERR_BADCHANNELNAME(nick, channelname) ":ircserv 476 " + nick + " " + channelname + " :Invalid channel name." + "\r\n"
#define ERR_CHANNELISFULL(nick, channelName) ":" + nick + " 471 " + channelName + " :Cannot join channel (+l)\r\n"
#define RPL_TOPICDISPLAY(hostname, nick, channel, topic) ":ircserv 332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_TOPICWHOTIME(topicsetter, topictime, nick, channelName) ":ircserv 333 " + nick + " " + channelName + " " + topicsetter + "!~" + topicsetter + "@ircserv " + topictime + "\r\n"

#define ERR_INPUTTOOLONG(nick) ": ircserv 417 " + nick + " :Input line too long\r\n"
#define RPL_SETTOPIC(nick, channel, topic) ":" + nick + "!" + nick + "@" + "ircserv" + " TOPIC " + channel + " :" + topic + "\r\n"
#define RPL_KICK(kicker, username, channel, targetuser, reason, ip) ":" + kicker + "!~" + username + "@" + ip + " KICK " + channel + " " + targetuser + " :" + reason + "\r\n"

#define RPL_JOIN(nick, username, channelname, ipaddress) ":" + nick + "!~" + username + "@" + ipaddress + " JOIN :" + channelname + "\r\n"
#define RPL_NAMREPLY(clients, channelname, nick) ":ircserv 353 " + nick + " = " + channelname + " :" + clients + "\r\n"
#define RPL_ENDOFNAMES(hostname, nick, channelname) ":ircserv 366 " + nick + " " + channelname + " :END of /NAMES list\r\n"
#define ERR_UNKNOWNCOMMAND(nick, command) (":ircserv 421 " + nick + " " + command + " :Unknown command\r\n")
#define ERR_BADCHANNELKEY(nick, channelName) ":ircserv 475 " + nick + " " + channelName + " :Cannot join channel (+K) - bad key\r\n"
#define ERR_NOTOPIC(channel, nick) ":ircserv 331 " + nick + " " + channel + " :No topic is set.\r\n"
#define ERR_USAGE(nick, command, msg) ":ircserv 650 " + nick + " " + command + " :" + msg + "\r\n"


#endif