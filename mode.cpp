
#include "Server.hpp"

// void    xec_mode(std::string &modes, std::string &key, std::string &limit, std::string &user, Channel &ch){
//     char sign = '+';
//     for (unsigned int i = 0; i < modes.size(); i++){
//         if (modes[i] == '+' || modes[i] == '-')
//             sign = modes[i++];
//         if (modes[i] == 'k'){
//             ch.setHasKey(true);
//             ch.setKey(key);
//         }
//         else if (modes[i] == 'l'){
//             unsigned int nb;
//             std::stringstream ss(limit);
//             if(ss >> nb){
//                 ch.setLimit(nb);
//                 ch.setHasLimit(true);}
//         }
//         else if (modes[i] == 'i'){
//             ch.setMode("invite-only");
//         }
//         else if (modes[i] == 't'){
//             ch.setTopicLock(true);
//             //set the topic if there is a param after +t
//         }
//         else if (modes[i] == 'o'){
//             if (s.isInUseNickname(user) && s.isRegistered(user))
//                 ch.addOperator(s.findClient(user)); //check if the nickname is in use and if its registered then add it in addoprator
//         }
//     }
// }

void    modeCommand(std::string &args, Client &c, Server &s){
    Channel ch(c, "gogo", s);//for test
    if (args.empty() || !args[0]){
        s.sendMsg(c.getClientFD(), ERR_NEEDMOREPARAMS(c.getNickname()));
        return;}
    else if (args[0] == ':' && args[1] != '#'){
        s.sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(args.substr(1), c.getNickname()));
        return ;}
    else if (args[0] != '#' && args[0] != ':'){
        s.sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(args.substr(0, args.find_first_of(" ")), c.getNickname()));
        return ;}
    std::stringstream ss(args);
    std::string chan;
    if (args[0] == ':' && args[1] == '#'){
        args = args.substr(2);
        ss.clear();
        ss << args;
        getline(ss, chan);}
    else if (args[0] == '#'){
        args = args.substr(1);
        ss.clear();
        ss << args;
        ss >> chan;}
        std::cout << chan << "|\n";
        std::cout << s.channels[0].getName() << "|\n";
    if (!s.isInUseChName(chan)){
        s.sendMsg(c.getClientFD(), ERR_NOSUCHCHANNEL(("#" + chan), c.getNickname()));
        return ;
    }
    if (ss.eof()) //there is no mode //mode #ch
        return ;
    Channel &channel = s.findChannel(chan);
    if (!channel.isOperator(c)){
        s.sendMsg(c.getClientFD(), ERR_NOTOP(c.getNickname(), ("#" + chan)));
        return ;
    }
    std::string modestring, key, limit, user, list;
    int k, o, l;
    k = o = l = 0;
    std::string modes = "+-litok";
    while (ss >> modestring){
    for (unsigned i = 0; i < modestring.size(); i++){
        if (modes.find(modestring[i]) == std::string::npos){
            s.sendMsg(c.getClientFD(), ERR_UNKNOWNMODE(c.getNickname(), ("#" + chan), modestring[i]));
            return ;
        }
        else{
            while (modestring[i] == modestring[i + 1])//ADD the last element when repeated
                i++;
            if (modestring[i] == 'k' && k++ == 0) //read from iss only the first time k is found
                ss >> key;
            if (modestring[i] == 'l'  && l++ == 0)
                ss >> limit;
            if (modestring[i] == 'o' && o++ == 0)
                ss >> user;
            }
            list += modestring[i];
        }
    }
    //xec part 
    char sign = '+';
    for (unsigned int i = 0; i < list.size(); i++){
        if (list[i] == '+' || list[i] == '-')
            sign = list[i++];
        if (list[i] == 'k'){
            if (!key.empty()){
                if (sign == '+'){
                    if (!channel.hasAKey()){
                        channel.setHasKey(true);
                        channel.setKey(key);}
                    else
                        continue ; //do nothing if the Key already set
                }
                else{
                    channel.setHasKey(false);
                    channel.setKey("");
                }
            }
            else
                s.sendMsg(c.getClientFD(), ERR_INVALIDMODEPARAM(c.getNickname(), ("#" + chan), 'k', "key"));
        }
        else if (list[i] == 'l'){
            if (!limit.empty()){
                unsigned int nb;
                std::stringstream ss(limit);
                if (ss >> nb){
                    if (sign == '+'){
                        channel.setLimit(nb);
                        channel.setHasLimit(true);
                    }
                    else{
                        channel.setLimit(0);
                        channel.setHasLimit(false);}
                }
            }
            else
                s.sendMsg(c.getClientFD(), ERR_INVALIDMODEPARAM(c.getNickname(), ("#" + chan), 'l', "limit"));
        }
        else if (list[i] == 'i'){
            if (sign == '+')
                channel.setMode("invite-only");
            else
                channel.setMode("");
        }
        else if (list[i] == 't'){
            if (sign == '+')
                channel.setTopicLock(true);
            else
                channel.setTopicLock(false);
        }
        else if (list[i] == 'o'){
            if (!user.empty()){
                if (!s.isInUseNickname(user))
                    s.sendMsg(c.getClientFD(), ERR_NOSUCHNICK(c.getNickname(), user));
                if (channel.isMember(s.findClient(user))){
                    if (sign == '+')
                        channel.addOperator(s.findClient(user)); //check if the nickname is in use and if its registered then add it in addoprator
                    else
                        channel.addRegularUser(s.findClient(user)); //check if the nickname is in use and if its registered then add it in addoprator
                }
                else
                    s.sendMsg(c.getClientFD(), ERR_USERNOTINCHANNEL(c.getNickname(), user, ("#" + chan)));
            }
            else
                s.sendMsg(c.getClientFD(), ERR_INVALIDMODEPARAM(c.getNickname(), ("#" + chan), 'o', "op"));
        }
    }
}
