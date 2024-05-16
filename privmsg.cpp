#include "Server.hpp"
int	ft_count(std::string str, size_t start){
	size_t	count = 0;
	size_t t;
	for (size_t i = start; i < str.size(); i++){
			if (str[i] == ','){
				for (t = i; str[t] == ','; t++)
				i = t;
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
			}
			else{
				if (ind - start == 0)
					break ;
				this->vec_cl.push_back(args.substr(start, ind - start));

			}
			if (args[ind] == ' ')
				break ;
			start = ind + 1;
			if (args[start] == ',')
				for (; args[start] == ','; ++start){};
			
			ind = args.find_first_of(", ", start);
			// if (args[ind] == ' ')
			// 	break ;
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
					return (2);
				}
			}
		}
	}
	size_t comma = index;
	size_t end = this->args.length() - 1;
	for(; this->args[end] == ' ' ; --end){
	}
	this->args = this->args.substr(0, end);
	size_t msg_begin = (args.find_last_of(" \t\r"));
	//GET THE MESSAGE AFTER ":" IN CASE OF MANY RECEPIENTS
	if (args[index] == ','){
		comma = ind;
		for(; (args[comma] == ' ' || args[comma] == '\r' || args[comma] == '\t'); comma++){
		};
		if (args[comma] == ':'){
			this->message = (args.substr(comma + 1 , args.size()));
			isMessage = true;
		}
		for (size_t	M = 0; M < vec_cl.size(); ++M){
			if ((isInUseNickname(vec_cl[M]) == true)){
				if (isMessage == false)
					this->message = (args.substr(msg_begin + 1, args.size()));//GETTING LAST PART
			sendMsg(findClient(vec_cl[M]).getClientFD(), MESSAGE(cli.getNickname(), vec_cl[M], this->message, cli.getUsername(), cli.getClientIP()));
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
					chan.sendmsg2chanOperators(*this,cli, this->message, chan);
					chan.sendmsg2chanRegulars(*this,cli, this->message, chan);
				}
				else
					sendMsg(cli.getClientFD(), ERR_CANNOTSENDTOCHANNEL(vec_ch[M], cli.getNickname()));

			}
			else
				sendMsg(cli.getClientFD(), ERR_NOSUCHCHANNEL(vec_ch[M], cli.getNickname()));
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
		this->target = args.substr(q, index - q);
		if (isMessage == false){
			this->message = (args.substr(msg_begin + 1, args.length()));//TODO: NEED TO GET ONLY FIRST PART//max 150 characters?
		}
		if (this->target[0] == '#'){
			if (isInUseChName(this->target) == true){
				Channel	&chan = findChannel(this->target);
				if (chan.isMember(cli)){
					chan.sendmsg2chanOperators(*this,cli, this->message, chan);
					chan.sendmsg2chanRegulars(*this,cli, this->message, chan);
				}
				else
					sendMsg(cli.getClientFD(), ERR_CANNOTSENDTOCHANNEL(this->target, cli.getNickname()));
				return (1);
			}
			else
				sendMsg(cli.getClientFD(), ERR_NOSUCHCHANNEL(this->target, cli.getNickname()));
		}
		else if (this->target[q] != '#'){
			if (isInUseNickname(this->target) == true){
				sendMsg(findClient(this->target).getClientFD(), MESSAGE(cli.getNickname(),this->target,message, cli.getUsername(), cli.getClientIP()));
				return (1);
			}
			else
				return (3);
		}

		
	}
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