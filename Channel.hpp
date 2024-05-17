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
        std::string name; //the channel's name
        std::string topic; //the topic of the channel
        std::string mode; // the mode seted for the channel
        std::string key; //The key required to join the channel (If the mode is not invite-only)
    
        unsigned int limit; //The maximum number of clients allowed in the channel
    
        bool topicLock; // true if only chanop can change the channel's topic
        // bool modeLock; // true if only chanop can change the channel's mode 
        bool hasLimit; // to be setted with true using mode +l or mode -l o let the number of members unlimited
        bool hasKey;
        //containers
        std::vector<Client> regularUsers; //the list of non-operator users in the channel
        std::vector<Client> operators; // the list of clients who have operator status in the channel
        // std::vector<Client> invited; //list of invited client to this channel

        std::string stringtime;
    
    public:
        Channel(Client &creator, std::string chname, Server &s);
        ~Channel();
        
        //setters
        void setMode(std::string newMode);
        void setTopic(std::string newTopic);
        void setKey(std::string k);
        void setHasKey(bool b);
        // void setModeLock(bool b); //lock or unlock the mode
        void setTopicLock(bool b); //lock or unlock the Topic
        void setHasLimit(bool b);
        void setLimit(unsigned int l);

        void setStringTime(std::string str);
        
        //getters
        std::string getName() const;
        std::string getTopic() const;
        std::string getMode() const; // return mode
        std::string getKey() const;
        int getLimit() const;
        // bool isModelocked() const; // return modeLock
        bool isTopiclocked() const; // return topicLock
        bool getHasLimit();
        bool getHasKey();

        std::string getStringTime(void) const;

        void addOperator(Client & c); //Add a client as an operator of the channel
        void removeOperator(Client & c); //Remove a client from operators list
        
        void addRegularUser(Client & c);
        void removeRegularUser(Client & c);
        
        // void    addInvited(Client & c);
        // bool    isInvited(Client & c);

        bool isOperator(Client const& c); //true if a given client is an operator in the channel
        bool isRegularuser(Client const& c); //true if a given client is a member in the channel 
        bool isMember(Client const& c); //the client is member if it is an operator or a regular user
        // bool isfull(); // true if operators.size() + regularUsers.size() == limit


        ///////ik
        void sendMsg2Members(Server &s, Client &c);
        void sendNickMsg2All(Server &s, std::string message);
         //AZMARA
        void	sendmsg2chanRegulars(Server s, Client cli, std::string& message, Channel ch);//NEW
		void	sendmsg2chanOperators(Server s, Client cli, std::string& message, Channel ch);//NEW

        // void channelStatusMsg(Server &s, std::string modestring, std::string newOp);
        std::string channelModes();

        ////////UPPPPPPPP
        bool hasLimitCanJ(void);
        std::string makeStringMember(void);

        void	updateAmemNickName(Client &c, std::string newNick);

        //$$$$$$$$
        std::string toLowerCase(std::string str);
        ///////&&&&&&&&&&&&&&
        void        sendMsgKick2Members(Server &s, Client &c, std::string name, std::string reason);
        size_t      getSizeMembers(void);

        void sendModeMsg2All(Server &s, std::string message); //KH
};
#endif