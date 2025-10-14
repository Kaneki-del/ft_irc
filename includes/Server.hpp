#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Server {
public:
  Server(const int port, const std::string password);
  void run();
  void handleNickCommand(Client *client, const std::string &new_nick);
  void commandDispatcher(Client *client, std::string commandLine);

private:
  std::string _password;
  int _port;
  void handle_new_connection();
  void handle_client_command(const int current_fd);
  int _listener_fd;
  std::vector<struct pollfd> _poll_fds;
  std::map<int, Client *> _clients;
  std::map<std::string, Client *> _nicknames;
};
#endif
