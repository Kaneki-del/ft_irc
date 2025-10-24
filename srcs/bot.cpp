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

void processBotCommand(Client * client,Client * bot_client, std::string & message){
  std::vector<std::string> splitedCommand =
    split_string_to_vector(message, ' ');

  if (splitedCommand.empty()) {
    return; 
  }
  else if (splitedCommand.size() > 2)
    //to much args

  e_cmd_bot_type cmdType = getCmdTtype(splitedCommand[0]) ;

  switch (cmdType) {
    case BOT_CMD_HELP:
      // Execute logic to retrieve and send the current time.
      break;
    case BOT_CMD_TIME:
      // Execute logic to retrieve and send the current time.
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
