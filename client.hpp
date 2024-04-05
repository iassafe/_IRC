
# ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <cctype> //tolower fun
#include <sstream>

#include "Channel.hpp"
#include "Server.hpp"

class Channel;
class Server;

class Client{
    private:
        int			clientID;
		std::string	clientIP;
		std::string	buffer;

        std::string nickname; //the user's visible identity in IRC
        std::string username; //less visible identifier primarily used for server-side purposes
        std::string hostname; //the hostname of the client's machine
        std::string servername; //the IRC server to which the user is connecting
        std::string realname; //the real name of the client
        int socket; //The socket descriptor for the client's connection
        bool registered; //Indicates whether the client succeeded to register to the server or not
        bool PasswordSended; //set to true the first time the correct password is given by the client

    public:
        static Server &server;
        Client();
        ~Client();

        //setters
        void	setIP(std::string IPaddr);
		void	setBuffer(std::string rec);
		void	setClientID(int fd);
        
        void    setNickname(std::string nn);
        void    setUsername(std::string un);
        void    setHostname(std::string hn);
        void    setServername(std::string sn);
        void    setRealname(std::string rn);
        void    setSocketDescriptor(int sd);
        void    setRegistered(bool b);
        void    setPasswordSended(bool b);
    
        //getters
        int		getClientID();

        std::string getNickname() const;
        std::string getUsername() const;
        std::string getHostname() const;
        std::string getServername() const;
        std::string getRealname() const;
        int getSocketDescriptor() const;
        bool isRegistered() const; //return registred
        bool    isPasswordSended(); //return passwordsended
    
        //other
        void registerClient(); //register the client into the server if all the attributes related to the client's autentication are set
};

//authentication utils
// void    tolowercase(std::string &str);
// bool    isValidNickName(std::string nickname);
// void    nickCommand(std::string &args, Client &c);
// void    userCommand(std::string &args, Client &c);
// void    passCommand(std::string &args, Client &c);

#endif