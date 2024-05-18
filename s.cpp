// #include "server.hpp"
// #include <iostream>
// #include <cstring>
// #include <cstdio>
// #include <cstdlib>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <sys/poll.h>

// const int MAX_CLIENTS = 10; // Maximum number of clients

// int main() {
//     int serverSocket, clientSockets[MAX_CLIENTS];
//     struct sockaddr_in serverAddr, clientAddr;
//     socklen_t clientAddrLen = sizeof(clientAddr);
//     struct pollfd fds[MAX_CLIENTS + 1]; // Include server socket

//     // Create server socket
//     serverSocket = AF_INET, SOCK_STREAM, 0);
//     if (serverSocket == -1) {
//         std::cerr << "Error creating server socket" << std::endl;
//         return 1;
//     }

//     // Bind server socket
//     memset(&serverAddr, 0, sizeof(serverAddr));
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(6667);
//     serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
//     if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
//         std::cerr << "Error binding server socket" << std::endl;
//         close(serverSocket);
//         return 1;
//     }

//     // Listen for incoming connections
//     if (listen(serverSocket, MAX_CLIENTS) == -1) {
//         std::cerr << "Error listening on server socket" << std::endl;
//         close(serverSocket);
//         return 1;
//     }

//     // Initialize pollfd structures
//     fds[0].fd = serverSocket;
//     fds[0].events = POLLIN; // Listen for incoming connections on server socket
//     int numClients = 0; // Current number of connected clients

//     while (1) {
//         // Call poll to wait for events
//         int numReady = poll(fds, numClients + 1, -1);
//         if (numReady == -1) {
//             std::cerr << "Error in poll" << std::endl;
//             break;
//         }

//         // Check events on server socket
//         if (fds[0].revents & POLLIN) {
//             // Accept new connection
//             clientSockets[numClients] = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
//             if (clientSockets[numClients] == -1) {
//                 std::cerr << "Error accepting connection" << std::endl;
//             } else {
//                 std::cout << "Accepted connection from " << inet_ntoa(clientAddr.sin_addr) << std::endl;
//                 fds[numClients + 1].fd = clientSockets[numClients];
//                 fds[numClients + 1].events = POLLIN; // Listen for data from client
//                 ++numClients;
//             }
//         }

//         // Check events on client sockets
//         for (int i = 0; i < numClients; ++i) {
//             if (fds[i + 1].revents & POLLIN) {
//                 char buffer[1024];
//                 memset(buffer, 0, sizeof(buffer));
//                 int bytesRead = recv(clientSockets[i], buffer, sizeof(buffer), 0);
//                 if (bytesRead <= 0) {
//                     if (bytesRead == 0) {
//                         std::cout << "Client closed connection" << std::endl;
//                     } else {
//                         std::cerr << "Error reading from client" << std::endl;
//                     }
//                     close(clientSockets[i]);
//                     // Remove client socket from pollfd list
//                     for (int j = i + 1; j < numClients + 1; ++j) {
//                         fds[j] = fds[j + 1];
//                     }
//                     --numClients;
//                 } else {
//                     std::cout << "Received from client: " << buffer << std::endl;
//                 }
//             }
//         }
//     }

//     // Close server socket
//     close(serverSocket);

//     return 0;
// }



// int	main(){
// 	try{
// 		int	socketId = socket(AF_INET, SOCK_STREAM, 0);
// 	if (socketId == -1){
// 		std::cerr << "Server not created!" << std::endl;
// 		return (1);
// 	}
// 	struct	sockaddr_in	sockaddress;
// 	memset(&sockaddress, 0, sizeof(sockaddress));
// 	sockaddress.sin_family = AF_INET;
// 	sockaddress.sin_port = htons(6667);
// 	sockaddress.sin_addr.s_addr = inet_addr("127.0.0.1");
// 	if(bind(socketId, (struct sockaddr *  )&sockaddress, sizeof(sockaddress)) == -1){
// 		std::cerr << "ERror at binding" << std::endl;
// 		close(socketId);
// 		return (1);
// 	}
// 	std::cout << "Server is binded to port : 6667" << std::endl; 
// 	if (listen (socketId, 200) == -1){
// 		std::cerr << "Not listening!" << std::endl;
// 		return (1);
// 	}
// 	std::cout << "Server is listening ..." << std::endl;
// 	while (1){
// 		struct sockaddr_in	connectedClientAddr;
// 		memset(&connectedClientAddr, 0, sizeof(connectedClientAddr));
// 		socklen_t	ClientAdrLength = 0;
// 		int	connectionServerId = accept(socketId, (struct sockaddr *) &connectedClientAddr, &ClientAdrLength);
// 		if (connectionServerId == -1){
// 			close (socketId);
// 			throw ("Failed to accept connection with client!");
// 		}
// 		std::cout << "accept a request at socket ID : " << connectionServerId << std::endl;
// 		char	receivedMsg[1024];
// 		memset(receivedMsg, 0, sizeof(receivedMsg));
// 		recv(connectionServerId, receivedMsg, 1024, 0);
// 		std::cout << "Server received : " << receivedMsg << std::endl;
// 		close (connectionServerId);
// 	}
// 		close (socketId);
// 	}
// 	catch(...){

// 	}

	
// }