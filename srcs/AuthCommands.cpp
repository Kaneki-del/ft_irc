#include "../includes/Server.hpp"

std::string trim(const std::string &str) {
  size_t first = str.find_first_not_of(" \t\n\r");
  if (std::string::npos == first) {
    return str;
  }
  size_t last = str.find_last_not_of(" \t\n\r");
  return str.substr(first, (last - first + 1));
}

bool Server::isValidNickName(std::string nickname) {
    //TODO check if this line is correct
    if (nickname.empty() || nickname.length() < 1)
        return false;
    char first_char = nickname[0];
    if (nickname.find(' ') != std::string::npos) {
        return false;
    }
    if (first_char == '#' || first_char == ':' || std::isdigit(first_char))
        return false;
    
    for (size_t i = 0; i < nickname.size(); i++) {
        char c = nickname[i];
        if (! (std::isalnum(c) || 
               c == '[' || c == ']' || c == '{' || c == '}' || 
               c == '\\' || c == '|')) 
        {
            return false;
        }
    }
    return true; 
}

void Server::handlePassCommand(Client *client, std::vector<std::string>args){
  
  if (client->isRegistered()) {
        client->send_reply("462", ":You may not reregister");
        return;
  }
  if (args.size() < 2) { 
        client->send_reply("461", "PASS :Not enough parameters");
        return;
  }
  std::string client_password = trim(args[1]);

  std::cout << "the password the client send *" << client_password << "*" <<  std::endl;
  if (_password == client_password){
    client->setPassState(true); 
    std::cout << GREEN 
      << "[SUCCESS] " << " authenticated password successfully." << std::endl;
  }
  else
    client->send_reply("464", ":Password incorrect");
}

void Server::handleNickCommand(Client *client, std::vector<std::string>args){
  if (args.size() < 2) { 
        client->send_reply("431", ":No nickname given");
        return;
  }
  if (client->isRegistered()) {
        client->send_reply("462", ":You may not reregister");
        return;
  }
  std::string new_nick = trim(args[1]);
  std::map<std::string, Client*>::iterator it = _nicknames.find(new_nick);
  if (it != _nicknames.end()) {
    client->send_reply("433", new_nick + " :Nickname is already in use");
    return;
  }
  else if (!isValidNickName(new_nick)){
      client->send_reply("432", new_nick + ":Erroneus nickname");
      return;
  }
  else{
      client->_nickName = new_nick;
      _nicknames[new_nick] = client;
      client->setNickState(true);
      checkRegistration(client);
  }
}

void Server::handleUserCommand(Client *client, std::vector<std::string>args){
  if (args.size() < 4) { 
    client->send_reply("461", " :Not enough parameters");
    return;
  }
  if (client->isRegistered()) {
    client->send_reply("462", ":You may not reregister");
    return;
  }
  client->_userName = args[1];
  std::cout << "_userName: " << client->_userName << std::endl;
  client->setUserState(true); 
  std::cout << GREEN 
    << "[SUCCESS] " << " User  successfully." << std::endl;
  checkRegistration(client);
}


