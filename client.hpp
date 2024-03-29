#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>

class	Client{
	private:
		int			clientID;
		std::string	clientIP;
		std::string	nickname;
		std::string	username;
		std::string	buffer;
		// bool		authenticated;
		// bool		registered;
	public:
		Client();
		int		getClientID();
		void	setClientID(int fd);
		void	setIP(std::string IPaddr);
		void	setBuffer(std::string rec);
		~Client();
};

#endif