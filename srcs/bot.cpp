#include "../includes/Server.hpp"

e_cmd_bot_type getCmdTtype(const std::string & cmdName)
{
  const std::string botCmdsNames[] = {
    "help",
    "time",
    "uptime",
    "ping",
  };
    for (size_t i = 0; i < 4; i++) {
      if (cmdName == botCmdsNames[i])
        return e_cmd_bot_type(i);
    }
  return BOT_CMD_UNKNOWN;
}

void HelpCmd(Client *Sendclient, std::vector<std::string> arguments){
  if (arguments.size() > 2)
  {
    Sendclient->send_reply("NOTICE",
                           "Syntax Error: Too many arguments for the Bot command.\
    Use help for command list.");
    return;
  }
  std::string message;
  std::vector<std::string> help_messages;
  help_messages.push_back("-- FTBot Command List --");

  help_messages.push_back("help :Displays this command list.");
  help_messages.push_back("time :Displays the current server time and date.");
  help_messages.push_back("uptime :Shows how long the IRC server process has been running.");

  help_messages.push_back("ping <nick> : Checks if a specified user is currently connected (Network state lookup).");
  help_messages.push_back("------------------------");

  for (size_t i = 0; i < help_messages.size(); i++)
  {
      message =  "PRIVMSG " + Sendclient->_nickName + " :" + help_messages[i] + "\r\n" ;
      send(Sendclient->getFd(), message.c_str(), message.length(), 0);
  }

}

// void timeCmd(Client *Sendclient, Client * bot_client,
//              std::vector<std::string> arguments){
//   if (arguments.size() > 2)
//   {
//     Sendclient->send_reply("NOTICE",
//                            "Syntax Error: Too many arguments for the Bot command.\
//     Use help for command list.");
//     return;
//   }
//
// }

void processBotCommand(Client * client, std::string & message){
  std::vector<std::string> splitedCommand =
    split_string_to_vector(message, ' ');

  if (splitedCommand.empty()) {
    return; 
  }
  else if (splitedCommand.size() > 2)
  {
    client->send_reply("NOTICE",
                  ":Syntax Error: Too many arguments for the Bot commands.");
    return;
  }

  e_cmd_bot_type cmdType = getCmdTtype(splitedCommand[0]) ;

  switch (cmdType) {
    case BOT_CMD_HELP:
        HelpCmd(client, splitedCommand);
      break;
    case BOT_CMD_TIME:
        // TimeCmd(client, bot_client, splitedCommand);
      break;
    case BOT_CMD_UPTIME:
      // Execute logic to retrieve and send the server's duration.
      break;
    case BOT_CMD_PING:
      // Execute logic to retrieve and send the server's duration.
      break;
    case BOT_CMD_UNKNOWN:
      // Send error reply: "!Unknown command. Use !help."
      break;
    // ...
  }
}
