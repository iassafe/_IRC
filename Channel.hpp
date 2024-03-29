#include "client.hpp"
#include "server.hpp"

class Channel{
    private:
        std::string name; //the channel's name
        std::string topic; //the topic of the channel
        std::string mode; // the mode seted for the channel
        std::string key; //The key required to join the channel (If the mode is not invite-only)
    
        int limit; //The maximum number of clients allowed in the channel
    
        bool topicLock; // true if only chanop can change the channel's topic
        bool modeLock; // true if only chanop can change the channel's mode
    
        //containers
        std::map<std::string, Client &> regularUsers; //the list of non-operator users in the channel
        std::map<std::string, Client &> operators; // the list of clients who have operator status in the channel
    public:
        Channel(Client const& creator, std::string chname, Server server);
        ~Channel();
        
        //setters
        void setMode(std::string newMode);
        void setTopic(std::string newTopic);
        void setKey(std::string k);
        void setModelock(bool b); //lock or unlock the mode
        void setTopiclock(bool b); //lock or unlock the Topic
        
        //getters
        std::string getName() const;
        std::string getTopic() const;
        std::string getMode() const; // return mode
        std::string getKey() const;
        int getlimit() const;
        bool isModelocked() const; // return modeLock
        bool isTopiclocked() const; // return topicLock

        void addOperator(Client const& client); //Add a client as an operator of the channel
        void removeOperator(Client const& client); //Remove a client from operators list
        
        void addRegularUser(Client const& client);
        void removeRegularUser(Client const& client);
        
        bool isOperator(Client const& client) const; //true if a given client is an operator in the channel
        bool isRegularuser(Client const& client) const; //true if a given client is a member in the channel 
        bool isMember(Client const& client); //the client is member if it is an operator or a regular user
        bool isfull(); // true if operators.size() + regularUsers.size() == limit
        
        // void invite(std::string nichname);
};
#endif