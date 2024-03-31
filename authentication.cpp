
#include "Client.hpp"

void    tolowercase(std::string &str){
    for (unsigned int i = 0; i < str.size(); i++)
        str[i] = std::tolower(str[i]);
}

bool isValidNickName(std::string nickname){
    if (!nickname.empty()){
        if (nickname.size() > 9)
            return false;//Nicknames can have a maximum length of 9 characters.
        for (unsigned int i = 0; i < nickname.size(); i++){
            if (!isdigit(nickname[i]) && nickname[i] != '-' && nickname[i] != '_' && nickname[i] != '\\'
            && !(nickname[i] >= 'a' && nickname[i] <= 'z')) //no need to check uppercase alphabets since nichname is already converted to lowercase in this step
                return false; //consist of letters (a-z, A-Z), numbers (0-9), and certain special characters, such as underscore (_), hyphen (-), and backslash ().
        }
        if (isdigit(nickname[0])) //The first character of a nickname must be a letter or a special character
            return false;
        if (nickname == "nickserv" || nickname == "chanserv") //are often reserved or restricted by the IRC server for other server services.
            return false ;
    }
    else
        return(false);//empty nickname
    return true;
}

void    nickCommand(std::string &args, Client &c){
    std::istringstream iss(args);
    std::string nn;
    if (!args.empty() && iss >> nn && iss.eof()){
        tolowercase(nn); //Nicknames are generally case-insensitive
        if (!isValidNickName(nn))
            c.server.sendMsg(c.getSocketDescriptor(), "Erroneous Nickname\n");
        if (c.server.isInUseNickname(nn))
            c.server.sendMsg(c.getSocketDescriptor(), "Nickname is in use.\n");
        c.setNickname(nn); //in both case: choosing a nname for the first time or changing a nname 
    }
    else
        std::cerr << "Not enough parameters.\n"; exit (1);
    c.registerClient();//client become registred in the server if the condition inside registerClient is true
}

void    userCommand(std::string &args, Client &c){
    std::istringstream iss(args);
    std::string un, hn, sn, rn;
    iss >> un >> hn >> sn;
    iss >>std::ws;
    if (iss.peek() == ':'){
        std::string restOfStrem;
        getline(iss, restOfStrem);
        restOfStrem = restOfStrem.substr(restOfStrem.find_first_not_of(": "));
        rn = restOfStrem.substr(0, restOfStrem.find_first_of(':'));
    }
    else
        iss >> rn;
    if (!iss.fail() && !un.empty() && !hn.empty() && !sn.empty() && !rn.empty()){
        c.setUsername(un);
        c.setHostname(hn);
        c.setServername(sn);
        c.setRealname(rn);
        std::cout << "i am printing in userCommand:\n" << "un = " << un << "hn = " << hn << "sn = " << sn << "rn = " << rn << "\n";
    }
    else
        c.server.sendMsg(c.getSocketDescriptor(), "Not enough parameters.\n");
    c.registerClient();//client become registred in the server if the condition inside registerClient is true
}

void    passCommand(std::string &args, Client &c){
    if (!c.isRegistered() && !c.isPasswordSended()){ //if the client is not yet registred and didn't give password before
        std::istringstream iss(args);
        std::string param;
        if (iss >> param && iss.eof()){
            param = param.substr(param.find_first_not_of(": ")); //skip : and space
            if (param != c.server.getPassword())
                c.server.sendMsg(c.getSocketDescriptor(), "Incorrect password.\n");
            c.setPasswordSended(true);  //leave a mark if pass cmd succeed the first time
        }
        else
            c.server.sendMsg(c.getSocketDescriptor(), "Invalid syntax for user command.\n");
    }
    else if ((!c.isRegistered() && c.isPasswordSended())) //in case a password has already been set but the client attempts to send the PASS command multiple times during the connection process
        c.server.sendMsg(c.getSocketDescriptor(), "PASS command should only be sent once.\n");
    else
        c.server.sendMsg(c.getSocketDescriptor(), "You may not reregister.\n"); //the client is already registred
    c.registerClient();//client become registred in the server if the condition inside registerClient is true
}