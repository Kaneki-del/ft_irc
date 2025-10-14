
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
Client::Client(int fd)
    : _fd(fd), _out_buffer(""), _registered(false), _nick_set(false),
      _pass_set(false) {}

std::string Client::extractAndEraseFromBuffer(size_t pos_found) {
  std::string toRetrun = _read_buffer.substr(0, pos_found);
  _read_buffer = _read_buffer.substr(pos_found + 2);
  return toRetrun;
}

void Client::process_and_extract_commands() {
  size_t pos_found = _read_buffer.find("\r\n");
  while (pos_found != std::string::npos) {
    std::string command_line = extractAndEraseFromBuffer(pos_found);
    Server::commandDispatcher(this, command_line);
  }
}
