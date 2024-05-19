/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iassafe <iassafe@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 18:16:57 by khanhayf          #+#    #+#             */
/*   Updated: 2024/05/19 19:11:13 by iassafe          ###   ########.fr       */
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
        int			             clientFD;
		std::string	             clientIP;
		std::string	             buffer;
        std::string              nickname;
        std::string              username;
        std::string              hostname;
        std::string              servername;
        std::string              realname;
        bool                     registered;
        bool                     PasswordSended;
        std::vector<std::string> invited2channels;

	public:
        Client();
        Client(Client const& obj);
        Client& operator=(Client const& obj);
        ~Client();

		void	setBuffer(std::string rec);
		void	setClientFD(int fd);
        void    setNickname(std::string nn);
        void    setUsername(std::string un);
        void    setHostname(std::string hn);
        void    setServername(std::string sn);
        void    setRealname(std::string rn);
        void    setRegistered(bool b);
        void    setPasswordSended(bool b);
        void    setClientIP(std::string ip);

        int         getClientFD();
        std::string getNickname() const;
        std::string getUsername() const;
        std::string getHostname() const;
        std::string getServername() const;
        std::string getRealname() const;
        std::string getBuffer() const;
        bool		isRegistered() const;
        bool		isPasswordSended();
        std::string getClientIP() const;

        void            clearAuthentication();
		void            registerClient(Server &s);
        void            invite2channel(std::string chName);
        bool            isInUseInvitedCh(std::string ChannelName);
        std::string     tolowercase(std::string str);
        void            removeInvitedCh(std::string ChannelName);

        void clearBuffer();

};

#endif