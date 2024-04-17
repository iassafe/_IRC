#include"Server.hpp"


int countComma(std::string str){
	int count = 0;
	for(size_t i=0; i < str.length(); i++){
		if(str[i] == ',')
			count++;
	}
	return (count);
}

void Server::execJoinCommand(void){
	if (send(this->connectionID, "Valid Command\n", 14, 0) == -1)
		throw (std::runtime_error("failed to send to client"));
}

void Server::execKickCommand(void){
	if (send(this->connectionID, "Valid Command\n", 14, 0) == -1)
		throw (std::runtime_error("failed to send to client"));
}

void Server::execTopicCommand(void){
	std::cout << "----------[" << this->command << "][" << this->args << "]\n";
	if (send(this->connectionID, "Topic Valid Command\n", 20, 0) == -1)
		throw (std::runtime_error("failed to send to client"));
	size_t found = this->args.find_first_of(" \t\r");
    std::string channelName = this->args.substr(0, found);

    // Find the channel
    std::vector<Channel>::iterator it = channels.begin();
    for (; it != channels.end(); ++it) {
        if (it->getName() == channelName){
            break;
        }
    }
    // Check if the channel was found
    if (it != channels.end()) {
		it->setTopic(this->args.substr(found + 1, this->args.length()));
		std::cout << "New topic is:------->" << it->getTopic() << std::endl;
    } else {
        // Channel not found
    }
}

int Server::joinSingleChannel(int pass){
	size_t found = this->args.find_first_of(" \r\t");
	std::string temp_args = this->args;
	this->joinChannel.push_back(this->args.substr(0, found));
	std::cout <<"channel:------->" << this->args.substr(0, found) << std::endl;
	if (pass){
		while(temp_args[found] == ' '){
			found++;
		};
		temp_args = temp_args.substr(found, temp_args.length());
		size_t found_sp = temp_args.find_first_of(" \r\t");
		if (found_sp != std::string::npos){
			this->joinPassword.push_back(this->args.substr(found, found_sp));
			std::cout <<"pass:------->" << this->args.substr(found, found_sp) << std::endl;
		}
		else{
			this->joinPassword.push_back(this->args.substr(found, this->args.length()));
			std::cout <<"pass:------->" << this->args.substr(found, this->args.length()) << std::endl;
		}
	}
	return(1);
}

std::string	skipSpaces(std::string str){
	size_t i;
	for (i = 0; str[i] == ' '; ++i){
	}
	return (&str[i]);
}

void Server::joinMultiChannels(int pass){
	std::string temp_args = this->args;
	size_t found = this->args.find_first_of(" \r\t");
		std::string channels = temp_args.substr(0, found);
		int count_ch = countComma(channels);
		size_t found_commach = channels.find_first_of(",");
		this->joinChannel.push_back(channels.substr(0, found_commach));
		std::cout << "[" << channels.substr(0, found_commach) <<"]\n";
		channels = channels.substr(found_commach + 1, channels.length());
		for(int i = 0; i < count_ch; ++i){
			found_commach = channels.find_first_of(",");
			this->joinChannel.push_back(channels.substr(0, found_commach));
			std::cout << "[" << channels.substr(0, found_commach) <<"]\n";
			channels = channels.substr(found_commach + 1, channels.length());
		}
		if (pass){
			std::string passWord = temp_args.substr(found + 1, temp_args.length());
			int count_ps = countComma(passWord);
			passWord = skipSpaces(passWord);
			size_t found_commaps = passWord.find_first_of(",");
			passWord = passWord.substr(found_commaps + 1, passWord.length());
			for(int i = 0; i < count_ps; ++i){
				found_commaps = passWord.find_first_of(",");
				this->joinPassword.push_back(passWord.substr(0, found_commaps));
				std::cout <<"pass["<< passWord.substr(0, found_commaps) << "]\n";
				passWord = passWord.substr(found_commaps + 1, passWord.length());
			}
		}
}

int Server::validArgsJoin(void){
	this->args = skipSpaces(this->args);
	if (this->args == "")
		return (0);
	std::cout << "-------------------"<< this->args << "-------------------" << std::endl;
	size_t found = this->args.find_first_of(" ");
	if (found == std::string::npos)       
		return (2);
	else{
		while (this->args[found] == ' '){
			found++;
		}
		if (this->args[found])
		{
			std::string passWord = &this->args[found];
			size_t foundSpace = passWord.find_first_of(" ");
			if (passWord[foundSpace] == ' '){
				while (this->args[foundSpace] == ' '){
					foundSpace++;
				}
				// if (passWord[foundSpace])
				// 	return (0);
			}
			return (3);
		}
		else
			return (2);
	}
}

int Server::validArgsTopic(void){
	if (!this->args[0])
		return (0);
	size_t found = this->args.find_first_of(" \t\r\n");
	if (found == std::string::npos || this->args[found] == '\n')
		return (0);
	std::string temp_args = this->args; 
	while(temp_args[found] == ' '){
		found++;
	};
	temp_args = temp_args.substr(found, temp_args.length());
	size_t found_sp = temp_args.find_first_of(" ");
	if (found_sp != std::string::npos){
		temp_args = temp_args.substr(found_sp + 1, temp_args.length());
		size_t i;
		for(i=0; i < temp_args.length() && temp_args[i] == ' '; ++i){
		};
		// if (temp_args[i] != '\n')
		// 	return (0);
	}
	return (1);
}

int Server::validArgsKick(void){
	if (!this->args[0])
		return (0);
	size_t found = this->args.find_first_of(" \t\r\n");
	if (found == std::string::npos || this->args[found] == '\n')
		return (0);
	std::string temp_args = this->args;
	while(found < temp_args.length() && temp_args[found] == ' '){
		found++;
	};
	temp_args = temp_args.substr(found, temp_args.length());
	size_t found_sp = temp_args.find_first_of(" ");
	if (found_sp != std::string::npos){
		temp_args = temp_args.substr(found_sp + 1, temp_args.length());
		size_t i;
		for(i=0; i < temp_args.length() && temp_args[i] == ' '; ++i){
		};
		if (temp_args[i] != '\n')
			return (0);
	}
	return (1);
}

void Server::whithoutPassword(void){
	size_t foundComma = this->args.find_first_of(",");
	if (foundComma != std::string::npos)
		joinMultiChannels(0);
	else
		joinSingleChannel(0);
}

void Server::whithPassword(void){
	size_t foundComma = this->args.find_first_of(",");
	if (foundComma != std::string::npos)
		joinMultiChannels(1);
	else
		joinSingleChannel(1);
}


void Server::joinCommand(Client &c){
	int check = validArgsJoin();
	if(check){
		if (check == 2)
			whithoutPassword();
		else if (check == 3)
			whithPassword();
		execJoinCommand();
	}
	else{
		if (send(c.getClientFD(), "Invalid args join\n", 18, 0) == -1)
			throw (std::runtime_error("failed to send to client"));
	}
}

void Server::topicCommand(Client &c){
	if(validArgsTopic())
			execTopicCommand();
	else{
		if (send(c.getClientFD(), "Invalid args topic\n", 19, 0) == -1)
			throw (std::runtime_error("failed to send to client"));
	}
}

void Server::kickCommand(Client &c){
	if(validArgsKick())
			execKickCommand();
	else{
		if (send(c.getClientFD(), "Invalid args kick\n", 18, 0) == -1)
			throw (std::runtime_error("failed to send to client"));
	}
}

