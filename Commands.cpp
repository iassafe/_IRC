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
		std::string password = temp_args.substr(found + 1, temp_args.length());
		int count_ch = countComma(channels);
		int count_ps = countComma(password);
		if (count_ch != count_ps)
			return (0);
		password = skipSpaces(password);
		size_t found_commach = channels.find_first_of(",");
		size_t found_commaps = password.find_first_of(",");
		this->joinChannel[channels.substr(0, found_commach)] = password.substr(0, found_commaps);
		std::cout << "[" << channels.substr(0, found_commach) <<"]["<< password.substr(0, found_commaps) <<"]\n";
		channels = channels.substr(found_commach + 1, channels.length());
		password = password.substr(found_commaps + 1, password.length());
		for(int i = 0; i < count_ch; ++i){
			found_commach = channels.find_first_of(",");
			found_commaps = password.find_first_of(",");
			this->joinChannel[channels.substr(0, found_commach)] = password.substr(0, found_commaps);
			std::cout << "[" << channels.substr(0, found_commach) <<"]["<< password.substr(0, found_commaps) <<"]\n";
			channels = channels.substr(found_commach + 1, channels.length());
			password = password.substr(found_commaps + 1, password.length());
		}
		return (1);
}

int Server::validArgsJoin(void){
	if (!this->args[0] || this->args[0]== '\n' || this->args[0] != '#'
		|| (this->args[0] == '#' && !isalpha(this->args[1]))){
		return (0);
	}
	this->args = skipSpaces(this->args);
	size_t found = this->args.find_first_of(" \t\r\n");
	if (found == std::string::npos || this->args[found] == '\n')
		return (0);
	size_t foundComma = this->args.find_first_of(",");
	if (foundComma == std::string::npos){
		if(!joinSingleChannel(found))
			return (0);
	}
	else{
		if(!joinMultiChannels(found))
			return (0);
	}
	return (1);
}

int Server::validArgsTopic(void){
	if (!this->args[0] || this->args[0]== '\n' || this->args[0] != '#'
		|| (this->args[0] == '#' && !isalpha(this->args[1]))){
		return (0);
	}
	size_t fond = this->args.find_first_of(" \t\r\n");
	if (fond == std::string::npos || this->args[fond] == '\n')
		return (0);
	std::string temp_args = this->args;
	this->joinChannel[temp_args.substr(0, fond)] = temp_args.substr(fond + 1, temp_args.length());
	while(fond < temp_args.length() && temp_args[fond] == ' '){
		fond++;
	};
	temp_args = temp_args.substr(fond, temp_args.length());
	size_t fond_sp = temp_args.find_first_of(" ");
	if (fond_sp != std::string::npos){
		temp_args = temp_args.substr(fond_sp + 1, temp_args.length());
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
	size_t fond = this->args.find_first_of(" \t\r\n");
	if (fond == std::string::npos || this->args[fond] == '\n')
		return (0);
	std::string temp_args = this->args;
	this->joinChannel[temp_args.substr(0, fond)] = temp_args.substr(fond + 1, temp_args.length());
	while(fond < temp_args.length() && temp_args[fond] == ' '){
		fond++;
	};
	temp_args = temp_args.substr(fond, temp_args.length());
	size_t fond_sp = temp_args.find_first_of(" ");
	if (fond_sp != std::string::npos){
		temp_args = temp_args.substr(fond_sp + 1, temp_args.length());
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
			if(validArgsJoin())
				joinCommand();
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