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
  void handleNickCommand(Client *client, const std::string &new_nick);
   void commandDispatcher(Client *client, std::string commandLine);
  e_cmd_type getCommandType(std::string command);


private:
  std::string _password;
  int _port;
  void handle_new_connection();
  void handle_client_command(const int current_fd);
  int _listener_fd;
  std::string _server_name;
  std::vector<struct pollfd> _poll_fds;
  std::map<int, Client *> _clients;
  std::map<std::string, Client *> _nicknames;
  std::map<std::string, e_cmd_type> _command_map;
  void handle_pass_command(Client *client, std::vector<std::string>splitedCommand); 

};
#endif
