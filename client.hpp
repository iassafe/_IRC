/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassafe <iassafe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 18:16:57 by khanhayf          #+#    #+#             */
/*   Updated: 2024/04/20 11:16:06 by iassafe          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <string>

#include "Server.hpp"
#include "responses.hpp"

class Server;

class	Client{
	private:
        int			clientFD;//The socket descriptor for the client's connection //M (clientFD instead of clientID)
		std::string	clientIP;
		std::string	buffer;

        std::string nickname; //the user's visible identity in IRC
        std::string username; //less visible identifier primarily used for server-side purposes
        std::string hostname; //the hostname of the client's machine
        std::string servername; //the IRC server to which the user is connecting
        std::string realname; //the real name of the client
        bool registered; //Indicates whether the client succeeded to register to the server or not
        bool PasswordSended; //set to true the first time the correct password is given by the client //M

	public:
        Client();
        ~Client();

        //setters
        void	setIP(std::string IPaddr);
		void	setBuffer(std::string rec);
		void	setClientFD(int fd);//M (setClientFD instead setClientID)
        void    setNickname(std::string nn);//M
        void    setUsername(std::string un);//M
        void    setHostname(std::string hn);//M
        void    setServername(std::string sn);//M
        void    setRealname(std::string rn);//M
        void    setRegistered(bool b);//M
        void    setPasswordSended(bool b);//M
    
        //getters
        int         getClientFD();//M
        std::string getNickname() const;//M
        std::string getUsername() const;//M
        std::string getHostname() const;//M
        std::string getServername() const;//M
        std::string getRealname() const;//M
        bool		isRegistered() const; //return registred //M
        bool		isPasswordSended(); //return passwordsended //M

        ////////ik
        std::string getClientIP() const;//M
		
		//other
        void    clearAuthentication();//M
		void    registerClient(Server &s);//M

};

#endif