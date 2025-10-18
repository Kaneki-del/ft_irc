#include "../includes/Server.hpp"


void Server::handle_pass_command(Client *client, std::vector<std::string>splitedCommand){
  if (splitedCommand.size() < 2) { 
        client->send_reply("461", "PASS :Not enough parameters");
        return;
  }
  std::string client_password = splitedCommand[1];
  if (_password == client_password){
    client->setPassBool(true); 
    std::cout << GREEN 
      << "[SUCCESS] " << " authenticated password successfully." << std::endl;
  }
  else 
    std::cerr << "[LOG] Client FD " << client->getFd() << " provided incorrect password." << std::endl;
}
