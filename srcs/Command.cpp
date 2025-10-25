#include "../includes/Server.hpp"

void Server::handlePrivmsgCommand(Client *client, std::vector<std::string>args){
  if (args.size() < 2) { 
    client->send_reply("411", ":No recipient given (" + args[0] + ")");
    return; 
  }
  if (args.size() < 3) {
    client->send_reply("412", ":No text to send");
    return;
  }
  std::string target = args[1];
  std::string message = args[2];
  if (target == BOT_NAME)
  {
    std::map<std::string, Client*>::iterator it = _nicknames.find(BOT_NAME);
    if (it != _nicknames.end()) {
      processBotCommand(client, message); 
      return;
    }
    else {
      std::cerr << "[ERROR] Bot not found in map, skipping command dispatch." << std::endl;
      client->send_reply("401", "bot :No such nick/channel");
      return;
    }
  }
  else
  {
    std::map<std::string, Client*>::iterator it = _nicknames.find(target);
    if (it != _nicknames.end()) {
      Client *senderClient = it->second; 
      std::string messageee =  "PRIVMSG " + senderClient->_nickName + " :" + message + "\r\n" ;
      std::cout << "outgoing message: " << messageee << std::endl;
      send(senderClient->getFd(), messageee.c_str(), messageee.length(), 0);
      return;
    }
    else
    {
      std::cout << "no client found" << std::endl;
      return;
    }
  }
}

