#include "Server.hpp"

int	Server::validArgsPriv(std::string &args, Client &cli){
	size_t		                count = 0;
	size_t		                ind;
(void)cli;
	size_t	index = args.find_first_of(" \t\r','");
	if (index == std::string::npos || (args[0] == ':')){
		return (2);
	}
	//*****IN CASE OF MANY CLIENTS/CHANNELS
	//------count commas------//
	if (args[index] == ','){
		for (size_t i = 0; i < args.size(); i++){
			if (args[i] == ',')
				count++;
		}
		std::cout << "count of commas:" << count << std::endl;
		size_t	start = 0;
		ind = index;
		//------fill vector with clients/channels------//
		for (size_t i = 0; i <= count; i++){
			std::cout << "args flwel :" << args[start] << "$$$$" << std::endl;
			if (args[start] == '#'){
				this->vec_ch.push_back(args.substr(start, ind - start));
				std::cout << "channels" << vec_ch[i] << std::endl;
				std::cout << "i=" << i << "---" << "vec_ch[i]:" << vec_ch[i] << "----0" << std::endl;
			}
			else{
				this->vec_cl.push_back(args.substr(start, ind - start));

			}
			puts("inside the loop");
			start = ind + 1;
		std::cout << "args[start]" << &args[start] << "****" << std::endl;
			ind = args.find_first_of("','", start);
			if (ind == std::string::npos)
				ind = args.find_first_of(" \t\r");	
				if (ind == std::string::npos)
					std::cout << "failed to find ind" << std::endl;
		std::cout << "args[ind]" << &args[ind] << "****" << std::endl;
		}
	}
	size_t i = index;
	std::cout << "args[i] in all cases" << args[i] << "######" << std::endl;
	size_t M;
	size_t msg_begin = (args.find_last_of(" \t\r"));
	if (args[index] == ','){
		i = ind;
		for(; (args[i] == ' ' || args[i] == '\r' || args[i] == '\t'); i++)
			//IN CASE OF CLIENT1, CLIENT2,jkkj  ?!
		// //------compare vec with clients------//
		// if (args[index] == ','){
			//***send to clients
			for (M = 0; M < vec_cl.size(); ++M){
				std::cout << "M=" << M << "---" << "vec_cl[M]:" << vec_cl[M] << "----0" << std::endl;
				std::cout << "return of isInUseNickname" << isInUseNickname(vec_cl[M]) << std::endl;
				// for(j = 0; j < clients.size(); ++j){
					// if (isInUseChName(vec[M]) == true){
					if ((isInUseNickname(vec_cl[M]) == true)){
						this->message = (args.substr(msg_begin + 1, args.size()));//GETTING LAST PART
						std::cout << "where message should end:" << msg_begin << "----+*" << args[msg_begin] << "====="<< std::endl;
						std::cout << "i" << i << "---" << args[i] << "----message to many clients:" << this->message << "-----/" << std::endl; 
						sendMsg(findClient(vec_cl[M]).getClientFD(), this->message);
						sendMsg(findClient(vec_cl[M]).getClientFD(), "\n");
					}
					else{
						puts("sup");
						sendMsg(cli.getClientFD(), ERR_NOSUCHNICK(vec_cl[M], this->target));
					}
				}
			//***send to channels
			for (M = 0; M < vec_ch.size(); ++M){
				std::cout << "return of isInUseChannel" << isInUseChName(vec_ch[M]) << std::endl;
				// for(j = 0; j < clients.size(); ++j){
					// if (isInUseChName(vec[M]) == true){
					if ((isInUseChName(vec_ch[M]) == true)){
						this->message = (args.substr(msg_begin + 1, args.size()));//GETTING LAST PART
						std::cout << "where message should end:" << msg_begin << "----+*" << args[msg_begin] << "====="<< std::endl;
						std::cout << "i" << i << "---" << args[i] << "----message to many channels:" << this->message << "-----/" << std::endl;
						Channel	chan = findChannel(vec_ch[M]);
						chan.sendmsg2chanOperators(*this, this->message);
						chan.sendmsg2chanRegulars(*this, this->message);
						// sendMsg(findChannel(vec_ch[M]), this->message);
						// sendMsg(findClient(vec_ch[M]).getClientFD(), "\n");
					}
					else{
						puts("sup");
						sendMsg(cli.getClientFD(), ERR_NOSUCHCHANNEL(vec_ch[M], this->target));
					}
				}
			// }

	}

	else{
						
	//------skip white spaces------//
		for(; (args[i] == ' ' || args[i] == '\r' || args[i] == '\t'); i++)
		this->target = args.substr(0, index);
		this->message = (args.substr(msg_begin + 1, args.size()));//TODO: NEED TO GET ONLY FIRST PART// max 150 characters?
		std::cout << "target[0]:" << this->target[0] << "------+++++" << std::endl;
		std::cout << "message in case of one element:" << this->message << "------+++++" << std::endl;
		// size_t m;
		// for(m = 0; m < clients.size(); m++){
		if (this->target[0] == '#'){
			std::cout << ":return dial isInUseChName(this->target)" << isInUseChName(this->target) << "&&&&&&&//////" << std::endl;
			if (isInUseChName(this->target) == true){
				Channel	chan = findChannel(this->target);
				std::cout << "chan" << chan.getName() << "----------*" << std::endl;
				chan.sendmsg2chanOperators(*this, this->message);
				puts("truuuuuuuuuuuue");
				chan.sendmsg2chanRegulars(*this, this->message);
				return (1);
			}
			else
				sendMsg(cli.getClientFD(), ERR_NOSUCHCHANNEL(this->target, cli.getNickname()));
		}
		else if (this->target[0] != '#'){
			if (isInUseNickname(this->target) == true){
				puts("found target");
				sendMsg(cli.getClientFD(), this->message);sendMsg(cli.getClientFD(), "\n");
				return (1);
			}
		}
		else
			return (3);

		
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
	
	this->vec_ch.clear();
	this->vec_cl.clear();
	// else if(validArgsPriv(args) == 8){
	// 	sendMsg(cli.getClientFD(), "\n");
	// }
}