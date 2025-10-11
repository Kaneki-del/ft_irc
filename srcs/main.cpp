// #include "../includes/Client.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Error: Usage: " << argv[0]
              << " \"./ircserv <port> <password>\"" << std::endl;
    return 1;
  }
  try {
    // TODO create the server class
    // TODO pars the port and the password
    //  Server irc_server(port, password);
    //  irc_server.run();

  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "Error: An unknown exception occurred during RPN processing."
              << std::endl;
    return 1;
  }
  return 0;
}
