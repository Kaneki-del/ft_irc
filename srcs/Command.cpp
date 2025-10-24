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
  //if the target is the bot handle it in ist own 
  //else check if the client target is in the map then send it to it
  //if not check if i need to send specific msg
}
