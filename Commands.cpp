#include"Server.hpp"


int countComma(std::string str){
	int count = 0;
	for(size_t i=0; i < str.length(); i++){
		if(str[i] == ',')
			count++;
	}
	return (count);
}

void Server::joinCommand(void){
	if (send(this->connectionID, "Valid Command\n", 14, 0) == -1)
		throw (std::runtime_error("failed to send to client"));
}

void Server::kickCommand(void){
	if (send(this->connectionID, "Valid Command\n", 14, 0) == -1)
		throw (std::runtime_error("failed to send to client"));
}

void Server::topicCommand(void){
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

int Server::joinSingleChannel(size_t found){
	std::string temp_args = this->args;
	if (found != std::string::npos){
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
			if (temp_args[i] != '\n')
				return (0);
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

int Server::joinMultiChannels(size_t found){
	std::string temp_args = this->args;
		std::string channels = temp_args.substr(0, found);
		size_t found_commaps = 0;
		int count_ch = 0;
		std::string passWord = "";
		int count_ps = 0;
		count_ch = countComma(channels);
		if (found != std::string::npos){
			passWord = temp_args.substr(found + 1, temp_args.length());
			count_ps = countComma(passWord);
			if (count_ch != count_ps)
				return (0);
			passWord = skipSpaces(passWord);
			found_commaps = passWord.find_first_of(",");

		}
		size_t found_commach = channels.find_first_of(",");
		this->joinChannel[channels.substr(0, found_commach)] = passWord.substr(0, found_commaps);
		std::cout << "[" << channels.substr(0, found_commach) <<"][";
		if (passWord != "")
			std::cout << passWord.substr(0, found_commaps) <<"]\n";
		channels = channels.substr(found_commach + 1, channels.length());
		if (passWord != "")
			passWord = passWord.substr(found_commaps + 1, passWord.length());
		for(int i = 0; i < count_ch; ++i){
			found_commach = channels.find_first_of(",");
			if (passWord != "")
				found_commaps = passWord.find_first_of(",");
			this->joinChannel[channels.substr(0, found_commach)] = passWord.substr(0, found_commaps);
			std::cout << "[" << channels.substr(0, found_commach) <<"]["<< passWord.substr(0, found_commaps) <<"]\n";
			channels = channels.substr(found_commach + 1, channels.length());
			if (passWord != "")
				passWord = passWord.substr(found_commaps + 1, passWord.length());
		}
		return (1);
}

int Server::validArgsJoin(void){
	this->args = skipSpaces(this->args);
	if (this->args == "")
		return(0);
	std::cout << "-------------------"<< this->args << "-------------------" << std::endl;
	size_t found = this->args.find_first_of(" ");
	if (found == std::string::npos)
		return (2);
	else{
		while (this->args[found] == ' '){
			found++;
		}
		if (this->args[found])
			return (3);
		else
			return (2);
	}
	/*
	if (!this->args[0] || this->args[0]== '\n' || this->args[0] != '#'
		|| (this->args[0] == '#' && (!isalpha(this->args[1]) && !isdigit(this->args[1])))){
		return (0);
	}
	std::cout << "#################--[" << args << "]\n";
	size_t found = this->args.find_first_of(" \r\t");
	size_t foundComma = this->args.find_first_of(",");
	if (foundComma == std::string::npos){
		if(!joinSingleChannel(found))
			return (puts("-----------2222"),0);
	}
	else{
		if(!joinMultiChannels(found))
			return (0);
	}
	return (1);
	*/
	return (1);
}

int Server::validArgsTopic(void){
	if (!this->args[0] || this->args[0]== '\n' || this->args[0] != '#'
		|| (this->args[0] == '#' && !isalpha(this->args[1]))){
		return (0);
	}
	size_t found = this->args.find_first_of(" \t\r\n");
	if (found == std::string::npos || this->args[found] == '\n')
		return (0);
	std::string temp_args = this->args; 
	this->joinChannel[temp_args.substr(0, found)] = temp_args.substr(found + 1, temp_args.length());
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
		if (temp_args[i] != '\n')
			return (0);
	}
	return (1);
}

int Server::validArgsKick(void){
	if (!this->args[0] || this->args[0]== '\n' || this->args[0] != '#'
		|| (this->args[0] == '#' && !isalpha(this->args[1]))){
		return (0);
	}
	size_t found = this->args.find_first_of(" \t\r\n");
	if (found == std::string::npos || this->args[found] == '\n')
		return (0);
	std::string temp_args = this->args;
	this->joinChannel[temp_args.substr(0, found)] = temp_args.substr(found + 1, temp_args.length());
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


void Server::handleCommands1(void){
	if (this->command == "join"){
			int check = validArgsJoin();
			if(check){
				// if (check == 2)
				// 	whithoutPassword();
				// else if (check == 3)
				// 	whithPassword();
				joinCommand();
			}
			else{
				if (send(this->connectionID, "Invalid args join\n", 18, 0) == -1)
					throw (std::runtime_error("failed to send to client"));
			}
	}
	else if (this->command == "topic"){
		if(validArgsTopic())
				topicCommand();
			else{
				if (send(this->connectionID, "Invalid args topic\n", 19, 0) == -1)
					throw (std::runtime_error("failed to send to client"));
			}
	}
	else if (this->command == "kick"){
		if(validArgsKick())
				kickCommand();
			else{
				if (send(this->connectionID, "Invalid args kick\n", 18, 0) == -1)
					throw (std::runtime_error("failed to send to client"));
			}
	}
}