#include "../includes/Server.hpp"
#include <string>

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
  std::string message;
  if (arguments.size() > 1)
  {
    message = "Syntax Error: Too many arguments for the Bot command. Use help for command list.";
    Sendclient->SendPrivateMessage(message);
    return;
  }
  std::vector<std::string> HelpMessages;
  HelpMessages.push_back("-- FTBot Command List --");
  HelpMessages.push_back("help :Displays this command list.");
  HelpMessages.push_back("time :Displays the current server time and date.");
  HelpMessages.push_back("uptime :Shows how long the IRC server process has been running.");
  HelpMessages.push_back("ping <nick> : Checks if a specified user is currently connected (Network state lookup).");
  HelpMessages.push_back("------------------------");

  for (size_t i = 0; i < HelpMessages.size(); i++)
    Sendclient->SendPrivateMessage(HelpMessages[i]);
}

void timeCmd(Client *Sendclient, std::vector<std::string> arguments){
   std::string message;
  if (arguments.size() > 1)
  {
     message = "Syntax Error: Too many arguments for the Bot command. Use help for command list.";
    Sendclient->SendPrivateMessage(message);
    return;
  }

  time_t RawSeconds;
  time(&RawSeconds);
  struct tm *TimeInfo = localtime(&RawSeconds);
  char TimeBuffer[100];
  size_t len = strftime(TimeBuffer, sizeof(TimeBuffer), 
                      "Current server time: %H:%M:%S on %Y-%m-%d", 
                      TimeInfo);
  if (len > 0)
    Sendclient->SendPrivateMessage(TimeBuffer);
}

void UpTimeCmd(Client *Sendclient, std::vector<std::string> arguments){
  std::string message;
  if (arguments.size() > 1)
  {
    message = "Syntax Error: Too many arguments for the Bot command. Use help for command list.";
    Sendclient->SendPrivateMessage(message);
    return;
  }
  time_t current_time = time(NULL);
  time_t start_time = Sendclient->GetServerPtr()->getStartTime();
  time_t uptime_seconds = current_time - start_time;
  if (uptime_seconds < 0) {
        return;
  }
  long days = uptime_seconds / (24 * 3600);
  long hours = (uptime_seconds % (24 * 3600)) / 3600;
  long minutes = (uptime_seconds % 3600) / 60;
  long seconds = uptime_seconds % 60;
  std::stringstream ss;
    ss << "Server Uptime: ";
    if (days > 0) {
        ss << days << " day" << (days > 1 ? "s" : "") << ", ";
    }
    ss << hours << " hour" << (hours != 1 ? "s" : "") << ", ";
    ss << minutes << " minute" << (minutes != 1 ? "s" : "") << ", ";
    ss << seconds << " second" << (seconds != 1 ? "s" : "") << ".";
    std::string RespodMessage = ss.str();
    Sendclient->SendPrivateMessage(RespodMessage);
}

void PingCmd(Client *Sendclient, std::vector<std::string> arguments){

  for (size_t i= 0; i < arguments.size(); i++)
       std::cout << "arg: " << i << arguments[i] << std::endl;
  std::string message;
  if (arguments.size() < 2)
  {
    std::cout << "the ping command is found 1" << std::endl;
    message = "Error: !PING command requires a target nickname. Usage: !ping <nickname>.";
    Sendclient->SendPrivateMessage(message);
    return;
  }
  std::cout << "the ping command is found" << std::endl;
  std::string TargetClient = arguments[1];
  std::map<std::string, Client *> nicknames = Sendclient->GetServerPtr()->GetNickNames();
  std::map<std::string, Client*>::iterator it = nicknames.find(TargetClient);
  if (it != nicknames.end()) {
    std::string confirmation_msg = TargetClient + " is currently active on the network.";
    Sendclient->SendPrivateMessage(confirmation_msg);
  }
  else {
    std::string error_msg = TargetClient + " is not connected to this server.";
    Sendclient->SendPrivateMessage(error_msg);
  }
}

void processBotCommand(Client * client, std::string & message){
  std::vector<std::string> splitedCommand =
    split_string_to_vector(message, ' ');

  // for (size_t i; i < splitedCommand.size(); i++)
  //   std::cout << splitedCommand[i] << std::endl;
  if (splitedCommand.empty()) {
    return; 
  }
  else if (splitedCommand.size() > 2)
  {
    message = "Syntax Error: Too many arguments for the Bot command. Use help for command list.";
    client->SendPrivateMessage(message);
    return;
  }
  e_cmd_bot_type cmdType = getCmdTtype(splitedCommand[0]) ;
  switch (cmdType) {
    case BOT_CMD_HELP:
        HelpCmd(client, splitedCommand);
      break;
    case BOT_CMD_TIME:
        timeCmd(client, splitedCommand);
      break;
    case BOT_CMD_UPTIME:
        UpTimeCmd(client, splitedCommand);
      break;
    case BOT_CMD_PING:
        PingCmd(client, splitedCommand);
      break;
    case BOT_CMD_UNKNOWN:
      std::string message = splitedCommand[0] + "is not a recognized command. Use help for a list of available commands.";
      client->SendPrivateMessage(message);
      break;
    // ...
  }
}
