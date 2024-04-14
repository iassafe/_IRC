
#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>

#include "Server.hpp"
#include "responses.hpp"

class Server;

class	Client{
	private:
        int			clientFD;//The socket descriptor for the client's connection
		std::string	clientIP;
		std::string	buffer;

        std::string nickname; //the user's visible identity in IRC
        std::string username; //less visible identifier primarily used for server-side purposes
        std::string hostname; //the hostname of the client's machine
        std::string servername; //the IRC server to which the user is connecting
        std::string realname; //the real name of the client
        bool registered; //Indicates whether the client succeeded to register to the server or not
        bool PasswordSended; //set to true the first time the correct password is given by the client
	public:
        Client();
        ~Client();

        //setters
        void	setIP(std::string IPaddr);
		void	setBuffer(std::string rec);
		void	setClientFD(int fd);
        void    setNickname(std::string nn);
        void    setUsername(std::string un);
        void    setHostname(std::string hn);
        void    setServername(std::string sn);
        void    setRealname(std::string rn);
        void    setRegistered(bool b);
        void    setPasswordSended(bool b);
    
        //getters
        int         getClientFD();
        std::string getNickname() const;
        std::string getUsername() const;
        std::string getHostname() const;
        std::string getServername() const;
        std::string getRealname() const;
        bool		isRegistered() const; //return registred
        bool		isPasswordSended(); //return passwordsended
		
		//other
        void    clearAuthentication();
		void registerClient(Server &s);

};

#endif