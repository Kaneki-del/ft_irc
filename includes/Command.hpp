#ifndef COMMAND_HPP

#define COMMAND_HPP
enum e_cmd_type {
  CMD_UNKNOWN = 0,
  CMD_PASS    = 1,
  CMD_NICK    = 2,
  CMD_USER    = 3,
  CMD_PRIVMSG  = 4,
};
#endif // !COMMAND_HPP
