#include "Server.hpp"
int	ft_count(std::string str, size_t start){
	size_t	count = 0;
	size_t t;
	for (size_t i = start; i < str.size(); i++){
			if (str[i] == ','){
				for (t = i; str[t] == ','; t++)
				i = t;
				std::cout << "where the i is-" << str[t] << "-after skipping consecutive commas\n";
				count++;
			}
		}
		return (count);
}


int	Server::validArgsPriv(std::string &args, Client &cli){
	size_t	count = 0;
	size_t	ind = 0;
	bool	isMessage = false;
	size_t q = 0;

	if (args[0] == ',')
		for (; args[q] == ','; ++q){};
	size_t	start = q;
	size_t	index = args.find_first_of(" \t\r','", q);
		
	if (index == std::string::npos || (args[0] == ':')){
		return (2);
	}
	if (args[index] == ','){
		count = ft_count(args, start);
		ind = index;
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
				std::cout << "vc_cl" << args.substr(start, ind - start) << "#########@" << std::endl;
				this->vec_cl.push_back(args.substr(start, ind - start));

			}
			// std::cout << "args[ind]"<< args[ind] << "------args[ind+1]" << args[ind+1] << "--------------------\n";
			if (args[ind] == ' ')
				break ;
			start = ind + 1;
			if (args[start] == ',')
				for (; args[start] == ','; ++start){};
			// std::cout << "args[start]" << &args[start]<< "-----------------------\n";
			
			ind = args.find_first_of(", ", start);
			// if (args[ind] == ' ')
			// 	break ;
			std::cout << "________________________ind__[" << &args[ind] << "]\n";
			if (ind == std::string::npos){
				ind = args.find_first_of(" \t\r");
				size_t k = ind;
				if (ind != std::string::npos){
					for (; args[k] == ' ' || args[k] == '\r' || args[k] == '\t'; ++k){
					};
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
	//GET THE MESSAGE AFTER ":" IN CASE OF MANY RECEPIENTS
	if (args[index] == ','){
		comma = ind;
		for(; (args[comma] == ' ' || args[comma] == '\r' || args[comma] == '\t'); comma++)
			std::cout << "args[comma]" << args[comma + 2] << "--------------********//////********" << std::endl;

		if (args[comma] == ':'){
			this->message = (args.substr(comma + 1 , args.size()));
			isMessage = true;
			std::cout << "ismessage" << isMessage << "-------)" << std::endl;
		}
		for (size_t	M = 0; M < vec_cl.size(); ++M){
			if ((isInUseNickname(vec_cl[M]) == true)){
				if (isMessage == false)
					this->message = (args.substr(msg_begin + 1, args.size()));//GETTING LAST PART
			sendMsg(findClient(vec_cl[M]).getClientFD(), this->message);
			sendMsg(findClient(vec_cl[M]).getClientFD(), "\n");
		}
		else
			sendMsg(cli.getClientFD(), ERR_NOSUCHNICK(cli.getNickname(),vec_cl[M]));
	}
		for (size_t M = 0; M < vec_ch.size(); ++M){
			if ((isInUseChName(vec_ch[M]) == true)){
				if (isMessage == false)
					this->message = (args.substr(msg_begin + 1, args.size()));//GETTING LAST PART
				Channel	&chan = findChannel(vec_ch[M]);
				if (chan.isMember(cli)){
					chan.sendmsg2chanOperators(*this, this->message);
					chan.sendmsg2chanRegulars(*this, this->message);
				}
				else
					sendMsg(cli.getClientFD(), ERR_CANNOTSENDTOCHANNEL(vec_ch[M], cli.getNickname()));

			}
			else
				sendMsg(cli.getClientFD(), ERR_NOSUCHCHANNEL(cli.getNickname(), vec_ch[M]));
	}
		vec_ch.clear();
		vec_cl.clear();
	}

	else{
		for(; (args[comma] == ' ' || args[comma] == '\r' || args[comma] == '\t'); comma++)
		if (args[comma] == ':'){
			isMessage = true;
			this->message = (args.substr(comma + 1 , args.size()));

		}
		std::cout << "index in case of one element" << args[index] << "-----------------------------\n";
		this->target = args.substr(q, index - q);
		if (isMessage == false)
			this->message = (args.substr(msg_begin + 1, args.size()));//TODO: NEED TO GET ONLY FIRST PART//max 150 characters?
		std::cout << "target[0]:" << this->target << "------+++++" << std::endl;
		std::cout << "message in case of one element:" << this->message << "------+++++" << std::endl;
		if (this->target[0] == '#'){
			std::cout << ":return dial isInUseChName(this->target)" << isInUseChName(this->target) << "&&&&&&&//////" << std::endl;
			if (isInUseChName(this->target) == true){
				Channel	&chan = findChannel(this->target);
				std::cout << "chan" << chan.getName() << "----------*" << std::endl;
				if (chan.isMember(cli)){
					chan.sendmsg2chanOperators(*this, this->message);
					chan.sendmsg2chanRegulars(*this, this->message);
				}
				else
					sendMsg(cli.getClientFD(), ERR_CANNOTSENDTOCHANNEL(this->target, cli.getNickname()));
				
				return (1);
			}
			else{
				std::cout << "NO SUCH CHANNEL" << cli.getNickname() << "33333333333333333333\n";
				sendMsg(cli.getClientFD(), ERR_NOSUCHCHANNEL(cli.getNickname(), this->target));
			}
		}
		else if (this->target[q] != '#'){
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