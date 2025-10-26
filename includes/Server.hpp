#ifndef SERVER_HPP
#define SERVER_HPP

#define GREEN    "\x1b[32m" 
#define RED      "\x1b[31m"
#define BOT_NAME "bot"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <string>
#include "Command.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
class Server;
#include "Client.hpp"
#include "Command.hpp"
#include "Command.hpp"

class Server {
public:

  Server(const int port, const std::string password);
  void run();
  void commandDispatcher(Client *client, std::string commandLine);
  void checkRegistration(Client * client);
  bool handleOutgoingData(int clientFd);
  bool isValidNickName(std::string  nickname);
  e_cmd_type getCommandType(std::string command);
  std::vector<struct pollfd> & getPollfds();
  void disconnectClient(int currentFd);
  void handlePrivmsgCommand(Client *client, std::vector<std::string>args); 
  void initialBot();
  time_t getStartTime() const ;
  std::map<std::string, Client *> GetNickNames() const;

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
  time_t _StartTime;

};
void processBotCommand(Client * client, std::string & message);
std::vector<std::string> split_string_to_vector(const std::string &input_string,
                                                char delimiter);

#endif
