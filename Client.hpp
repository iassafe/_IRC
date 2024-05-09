/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassafe <iassafe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 18:16:57 by khanhayf          #+#    #+#             */
/*   Updated: 2024/05/09 15:29:49 by iassafe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


 
#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>

#include "responses.hpp"
#include "Server.hpp"
#include <vector>

class Server;

class	Client{
	private:
        int			clientFD;//The socket descriptor for the client's connection (clientFD instead of clientID)
		std::string	clientIP;
		std::string	buffer;

        std::string nickname; //the user's visible identity in IRC
        std::string username; //less visible identifier primarily used for server-side purposes
        std::string hostname; //the hostname of the client's machine
        std::string servername; //the IRC server to which the user is connecting
        std::string realname; //the real name of the client
        bool registered; //Indicates whether the client succeeded to register to the server or not
        bool PasswordSended; //set to true the first time the correct password is given by the client 
        std::vector<std::string> invited2channels;

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
        std::string getBuffer() const;
        bool		isRegistered() const; //return registred 
        bool		isPasswordSended(); //return passwordsended 

        ////////ik
        std::string getClientIP() const;
		
		//other
        void        clearAuthentication();
		void        registerClient(Server &s);
        void    invite2channel(std::string chName);// add a new channel to the invited2channels list

        bool            isInUseInvitedCh(std::string ChannelName);
        std::string     tolowercase(std::string str);
        // std::string&    findingInvitedCh(std::string ChannelName);
        void            removeInvitedCh(std::string ChannelName);

        void clearBuffer();

};

#endif