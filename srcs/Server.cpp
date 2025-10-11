#include "../includes/Server.hpp"

Server::Server(const int port, const std::string password)
    : _port(port), _password(password), _listener_fd(-1) {

  _listener_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  fcntl(_listener_fd, F_SETFL, O_NONBLOCK);

  struct sockaddr_in serv_addr;
  if (bind(_listener_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ==
      -1) {
    std::cerr << "Error: Failed to bind socket to port " << _port << "."
              << std::endl;

    close(_listener_fd);

    throw std::runtime_error(
        "Fatal server initialization error (bind failed).");
  }
  listen(_listener_fd, SOMAXCONN);
  // TODO to check for failer later -1
  if (_listener_fd > 0) {

    struct pollfd listener_poll_fd;

    listener_poll_fd.fd = _listener_fd;

    listener_poll_fd.events = POLLIN;
    listener_poll_fd.revents = 0;
    _poll_fds.push_back(listener_poll_fd);
  }
}

void Server::run() {
  while (true) {
    poll(_poll_fds.data(), _poll_fds.size(), -1);
    for (int i = 0; i < _poll_fds.size(); ++i) {
      int current_fd = _poll_fds[i].fd;

      if (_poll_fds[i].revents & POLLIN) {
        if (current_fd == _listener_fd)
          handle_new_connection();
        else
          handle_client_command(current_fd);
      }
    }
  }
}

void handle_new_connection() {
  // TODO create the socket
  // TODO add the fd to the main fds and for the fd in the client object
}
