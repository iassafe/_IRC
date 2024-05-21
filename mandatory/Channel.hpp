#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <vector>

#include "Client.hpp"
#include "Server.hpp"

class Client;
class Server;

class Channel{
    private:
        std::string         name; 
        std::string         topic;
        std::string         mode;
        std::string         key;
        unsigned int        limit;
        bool                topicLock;
        bool                hasLimit;
        bool                hasKey;
        std::vector<Client> regularUsers;
        std::vector<Client> operators;
        std::string         stringtime;
    
    public:
        Channel(Client &creator, std::string chname, Server &s);
        Channel(Channel const& obj);
        Channel& operator=(Channel const& obj);
        ~Channel();
        
        void        setMode(std::string newMode);
        void        setTopic(std::string newTopic);
        void        setKey(std::string k);
        void        setHasKey(bool b);
        void        setTopicLock(bool b);
        void        setHasLimit(bool b);
        void        setLimit(unsigned int l);
        void        setStringTime(std::string str);
        
        std::string getName() const;
        std::string getTopic() const;
        std::string getMode() const;
        std::string getKey() const;
        std::string getStringTime(void) const;
        int         getLimit() const;
        bool        isTopiclocked() const;
        bool        getHasLimit();
        bool        getHasKey();

        void        addOperator(Client & c);
        void        addRegularUser(Client & c);

        void        removeOperator(Client & c);
        void        removeRegularUser(Client & c);

        bool        isOperator(Client const& c);
        bool        isRegularuser(Client const& c);
        bool        isMember(Client const& c);

        void        sendMsg2Members(Server &s, Client &c);
        void        sendNickMsg2All(Server &s, std::string message);
        void        updateAmemNickName(Client &c, std::string newNick);
        void        sendMsgKick2Members(Server &s, Client &c, std::string name, std::string reason);
        void        sendModeMsg2All(Server &s, std::string message);

        void        sendmsg2chanRegulars(Server s, Client cli, std::string& message, Channel ch);
		void        sendmsg2chanOperators(Server s, Client cli, std::string& message, Channel ch);

        bool        hasLimitCanJ(void);
        std::string channelModes();
        std::string makeStringMember(void);
        std::string toLowerCase(std::string str);
        size_t      getSizeMembers(void);
        void        sendMsgTopic(Client &c, Server &s);
};
#endif