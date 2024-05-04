#include "Server.hpp"
int	ft_count(std::string str){
	size_t	count = 0;
	for (size_t i = 0; i < str.size(); i++){
			if (str[i] == ',')
				count++;
		}
		return (count);
}

// void	Server::store_clients_channels(std::string &args, size_t count, size_t ind, size_t start, Client &cli){
// 	for (size_t i = 0; i <= count; i++){
// 			if (args[start] == '#'){
// 				if (ind - start == 0)
// 					break ;
// 				this->vec_ch.push_back(args.substr(start, ind - start));
// 			}
// 			else{
// 				if (ind - start == 0)
// 					break ;
// 				this->vec_cl.push_back(args.substr(start, ind - start));

// 			}
// 			start = ind + 1;
// 			ind = args.find_first_of("','", start);
// 			if (ind == std::string::npos)
// 				ind = args.find_first_of(" \t\r");	
// 				if (ind == std::string::npos)
// 					sendMsg(cli.getClientFD(), ERR_NO_TEXT(cli.getNickname()));
// 		}
// }

// void	Server::sendToClients(size_t msg_begin, Client &cli, bool isMessage){
	
	
// 	for (size_t	M = 0; M < vec_cl.size(); ++M){
// 		if ((isInUseNickname(vec_cl[M]) == true)){
// 			if (isMessage == false)
// 				this->message = (args.substr(msg_begin + 1, args.size()));//GETTING LAST PART
// 			sendMsg(findClient(vec_cl[M]).getClientFD(), this->message);
// 			sendMsg(findClient(vec_cl[M]).getClientFD(), "\n");
// 		}
// 		else
// 			sendMsg(cli.getClientFD(), ERR_NOSUCHNICK(vec_cl[M], this->target));
// 	}
// }

// void	Server::sendToChannels(size_t msg_begin, Client &cli, bool isMessage){
// 	for (size_t M = 0; M < vec_ch.size(); ++M){
// 		if ((isInUseChName(vec_ch[M]) == true)){
// 			if (isMessage == false)
// 				this->message = (args.substr(msg_begin + 1, args.size()));//GETTING LAST PART
// 			Channel	chan = findChannel(vec_ch[M]);
// 			chan.sendmsg2chanOperators(*this, this->message);
// 			chan.sendmsg2chanRegulars(*this, this->message);
// 		}
// 		else
// 			sendMsg(cli.getClientFD(), ERR_CANNOTSENDTOCHANNEL(this->target, cli.getNickname()));
// 	}
// }

int	Server::validArgsPriv(std::string &args, Client &cli){
	size_t	count = 0;
	size_t	ind = 0;
	size_t	start = 0;
	bool	isMessage = false;

	size_t	index = args.find_first_of(" \t\r','");
		
	if (index == std::string::npos || (args[0] == ':')){
		return (2);
	}
	if (args[index] == ','){
		count = ft_count(args);
		ind = index;
		// store_clients_channels(args, count, ind, start);
		for (size_t i = 0; i <= count; i++){
			if (args[start] == '#'){
				if (ind - start == 0)
					break ;
				this->vec_ch.push_back(args.substr(start, ind - start));
				std::cout << "vc_ch" << vec_ch[i] << "$$$$$$$$" << std::endl;
			}
			else{
				if (ind - start == 0){
					puts("uuuu");
					break ;
				}
				this->vec_cl.push_back(args.substr(start, ind - start));
				std::cout << "vc_cl" << vec_cl[i] << "#########@" << std::endl;

			}
			start = ind + 1;
			ind = args.find_first_of(",", start);
			std::cout << "________________________start__[" << args[start] << "]\n";
			if (ind == std::string::npos){
				ind = args.find_first_of(" \t\r");
				size_t k = ind;
				if (ind != std::string::npos){
					for (; args[k] == ' ' || args[k] == '\r' || args[k] == '\t'; ++k){
					}
				}
				if (ind == std::string::npos || args[k] == '\0'){
					vec_ch.clear();
					vec_cl.clear();
				std::cout << "________________________ind__[" << args[k] << "]\n";
					return (2);
				}
			}
		}
	}
	size_t comma = index;
		std::cout << "comma" << comma << "----" << args[comma] << "^^^^^^^^^^\n"; 
	size_t msg_begin = (args.find_last_of(" \t\r"));
	if (args[index] == ','){
		comma = ind;
		for(; (args[comma] == ' ' || args[comma] == '\r' || args[comma] == '\t'); comma++)
			std::cout << "args[comma]" << args[comma + 2] << "--------------********//////********" << std::endl;

		if (args[comma] == ':'){
			this->message = (args.substr(comma + 1 , args.size()));
			isMessage = true;
		std::cout << "ismessage" << isMessage << "-------)" << std::endl;
		}
		// sendToClients(msg_begin, cli, isMessage);
		for (size_t	M = 0; M < vec_cl.size(); ++M){
		if ((isInUseNickname(vec_cl[M]) == true)){
			if (isMessage == false)
				this->message = (args.substr(msg_begin + 1, args.size()));//GETTING LAST PART
			sendMsg(findClient(vec_cl[M]).getClientFD(), this->message);
			sendMsg(findClient(vec_cl[M]).getClientFD(), "\n");
		}
		else
			sendMsg(cli.getClientFD(), ERR_NOSUCHNICK(vec_cl[M], this->target));
	}
		// sendToChannels(msg_begin, cli, isMessage);
		for (size_t M = 0; M < vec_ch.size(); ++M){
		if ((isInUseChName(vec_ch[M]) == true)){
			if (isMessage == false)
				this->message = (args.substr(msg_begin + 1, args.size()));//GETTING LAST PART
			Channel	chan = findChannel(vec_ch[M]);
			chan.sendmsg2chanOperators(*this, this->message);
			chan.sendmsg2chanRegulars(*this, this->message);
		}
		else
			sendMsg(cli.getClientFD(), ERR_CANNOTSENDTOCHANNEL(this->target, cli.getNickname()));
	}
		vec_ch.clear();
		vec_cl.clear();
	}

	else{
		for(; (args[comma] == ' ' || args[comma] == '\r' || args[comma] == '\t'); comma++)
		if (args[comma] == ':'){
			this->message = (args.substr(comma + 1 , args.size()));
			isMessage = true;
		}
		this->target = args.substr(0, index);
		std::cout << "ismessage145" << isMessage << "------------145\n";
		if (isMessage == false)
			this->message = (args.substr(msg_begin + 1, args.size()));//TODO: NEED TO GET ONLY FIRST PART// max 150 characters?
		std::cout << "target[0]:" << this->target << "------+++++" << std::endl;
		std::cout << "message in case of one element:" << this->message << "------+++++" << std::endl;
		if (this->target[0] == '#'){
			std::cout << ":return dial isInUseChName(this->target)" << isInUseChName(this->target) << "&&&&&&&//////" << std::endl;
			if (isInUseChName(this->target) == true){
				Channel	&chan = findChannel(this->target);
				std::cout << "chan" << chan.getName() << "----------*" << std::endl;
				chan.sendmsg2chanOperators(*this, this->message);
				chan.sendmsg2chanRegulars(*this, this->message);
				return (1);
			}
			else
				sendMsg(cli.getClientFD(), ERR_CANNOTSENDTOCHANNEL(this->target, cli.getNickname()));
		}
		else if (this->target[0] != '#'){
			std::cout << "isInuse------------" << isInUseNickname(this->target) << "------------" << std::endl;
			if (isInUseNickname(this->target) == true){
				sendMsg(findClient(this->target).getClientFD(), this->message);
				sendMsg(findClient(this->target).getClientFD(), "\n");
				return (1);
			}
			else
				return (3);
		}

		
	}
	std::cout << "this->target:" << this->target << "---;" << std::endl;
		std::cout << "this->message:" << this->message << "---;" << std::endl;
	return (1);
}

void	Server::privmsgCommand(std::string &args, Client &cli){
    
	if (validArgsPriv(args, cli) == 1)
			;
	else if(validArgsPriv(args, cli) == 2){
		sendMsg(cli.getClientFD(), ERR_NO_TEXT(cli.getNickname()));
	}
	else if(validArgsPriv(args, cli) == 3){
		sendMsg(cli.getClientFD(), ERR_NOSUCHNICK(cli.getNickname(), this->target));
	}
	
	
	// else if(validArgsPriv(args) == 8){
	// 	sendMsg(cli.getClientFD(), "\n");
	// }
}