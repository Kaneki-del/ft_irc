#include "../includes/Server.hpp"


void Server::handle_pass_command(Client *client, std::vector<std::string>args){
  if (args.size() < 2) { 
        client->send_reply("461", "PASS :Not enough parameters");
        return;
  }
  std::string client_password = args[1];
  if (_password == client_password){
    client->setPassState(true); 
    std::cout << GREEN 
      << "[SUCCESS] " << " authenticated password successfully." << std::endl;
  }
  else 
    std::cerr << "[LOG] Client FD " << client->getFd() << " provided incorrect password." << std::endl;
}

void Server::handleNickCommand(Client *client, std::vector<std::string>args){
  if (args.size() < 2) { 
        client->send_reply("431", "USER :Not enough parameters");
        return;
    }
  if (client->isRegistered()) {
        client->send_reply("462", ":You may not reregister");
        return;
  }
    std::cout << GREEN 
      << "[SUCCESS] " << " Nick  successfully." << std::endl;
  client->_nickName = args[1];
  client->setNickState(true);
  checkRegistration(client);
}



void Server::handleUserCommand(Client *client, std::vector<std::string>args){
  if (args.size() < 5) { 
        client->send_reply("461", " :Not enough parameters");
        return;
    }
  if (client->isRegistered()) {
        client->send_reply("462", ":You may not reregister");
        return;
  }
  client->_userName = args[1];
  client->_realName = args[4];
  client->setUserState(true); 
    std::cout << GREEN 
      << "[SUCCESS] " << " User  successfully." << std::endl;
  checkRegistration(client);
}


