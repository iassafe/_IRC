
#include "Client.hpp"

Client::Client(){
    nickname = username = hostname = servername = realname = "\0";
    registered = PasswordSended = false;
    socket = -1;
}
Client::~Client(){
    server.removeUser(*this);//client's fd is closed in removeuser
    PasswordSended = false; //to reautenticate the next time he connect to the server
}

//setters
void	Client::setIP(std::string IPaddr){
	this->clientIP = IPaddr;
}
void	Client::setBuffer(std::string rec){
	this->buffer = rec;
}
void	Client::setClientID(int fd){
	this->clientID = fd;
}

void Client::setNickname(std::string nn){
    nickname = nn;
}
void Client::setUsername(std::string un){
    username = un;
}
void Client::setHostname(std::string hn){
    hostname = hn;
}
void Client::setServername(std::string sn){
    servername = sn;
}
void Client::setRealname(std::string rn){
    realname = rn;
}
void Client::setSocketDescriptor(int sd){
    socket = sd;
}
void Client::setRegistered(bool b){
    registered = b;
}

void Client::setPasswordSended(bool b){
    PasswordSended = b;
}

//getters
int Client::getClientID(){
	return (clientID);
}

std::string Client::getNickname() const{
    return (nickname);
}
std::string Client::getUsername() const{
    return (username);
}
std::string Client::getHostname() const{
    return (hostname);
}
std::string Client::getRealname() const{
    return (realname);
}
std::string Client::getServername() const{
    return (servername);
}
int Client::getSocketDescriptor() const{
    return (socket);
}
bool Client::isRegistered() const{
    return (registered);
} 
bool    Client::isPasswordSended(){
    return true;
}

//others

void Client::registerClient(){
    if (PasswordSended && !nickname.empty() && !username.empty()
    && !hostname.empty() && !servername.empty() && !realname.empty()){
        registered = true;
        server.addUser(*this);
    }
}


//     return 0;
// }
// // Certainly! After calling listen(), which sets up the server to accept incoming connections, the server enters a loop where it continuously waits for and handles incoming client connections. Here are the steps after listen():

// // Initialize poll() Structures: Before entering the main loop, initialize a vector of pollfd structures to store file descriptors and events to monitor. Initially, add the server socket to this vector.

// // Main Loop: Enter a loop that runs indefinitely, continuously checking for events on the monitored file descriptors using poll().

// // Call poll(): Inside the loop, call poll() to wait for events on the file descriptors. The poll() function blocks until an event occurs or until the specified timeout (in this case, -1 indicates no timeout, meaning it will block indefinitely).

// // Handle Events: After poll() returns, iterate through the pollfd structures to check which file descriptors have events pending.

// // New Client Connection: If an event occurs on the server socket (indicating a new client connection), accept the connection using accept() function. Upon successful acceptance, add the new client socket to the vector of pollfd structures.

// // Existing Client Socket: If an event occurs on an existing client socket, indicating data is available for reading, read from the socket using recv(). Process the received data as needed.

// // Error Handling: Check for errors while reading from the client socket. If an error occurs or if the client closes the connection, handle the error gracefully by closing the client socket and removing it from the vector of pollfd structures.

// // Loop Continuation: After handling the events, continue to the next iteration of the loop, where poll() will again be called to wait for new events.

// // Clean-up: Upon exiting the loop (which in this case is infinite), close the server socket to release the associated resources.

// // This loop effectively allows the server to handle multiple client connections concurrently without blocking on any single client's I/O operation. The server can respond to new client connections and incoming data from existing clients without being limited by the number of threads or processes.
