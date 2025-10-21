#ifndef SERVER_HPP
#define SERVER_HPP

#define GREEN    "\x1b[32m" 
#define RED      "\x1b[31m"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
class Server;
#include "Client.hpp"

enum e_cmd_type {
    CMD_UNKNOWN = 0,
    CMD_PASS    = 1,
    CMD_NICK    = 2,
    CMD_USER    = 3,
};

class Server {

public:

  Server(const int port, const std::string password);
  void run();
  void commandDispatcher(Client *client, std::string commandLine);
  void checkRegistration(Client * client);
  bool handleOutgoingData(int clientFd);
  // bool isValidNickName(std::string  nickname);
  e_cmd_type getCommandType(std::string command);
  std::vector<struct pollfd> & getPollfds();
  void disconnectClient(int currentFd);

private:

  void handleNewConnection();
  bool handleClientCommand(const int currentFd);
  //Authentication commands 
  void handlePassCommand(Client *client, std::vector<std::string>args); 
  void handleUserCommand(Client *client, std::vector<std::string>args); 
  void handleNickCommand(Client *client, std::vector<std::string>args);
  std::string _password;
  int _port;
  int _listenerFd;
  std::string _serverName;
  std::vector<struct pollfd> _pollFds;
  std::map<int, Client *> _clients;
  std::map<std::string, Client *> _nicknames;
  std::map<std::string, e_cmd_type> _commandMap;

};
#endif
