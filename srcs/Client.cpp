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

bool Client::is_registered() { return _registered; }
std::string Client::getNickname() { return _nickname; }

void Client::send_reply(const std::string &numeric,
                        const std::string &content) {
  std::string prefix = ":ft_irc.local";
  std::string recipient = this->is_registered() ? this->getNickname() : "*";

  std::string full_message =
      prefix + " " + numeric + " " + recipient + " " + content;

  // this->getOutBuffer().append(full_message + "\r\n");
  // this->setPollOut(true);
}

std::string &Client::getReadBuffer() { return _read_buffer; }

void Client::process_and_extract_commands() {
  size_t pos_found = _read_buffer.find("\r\n");
  while (pos_found != std::string::npos) {
    std::string command_line = extractAndEraseFromBuffer(pos_found);
    Server::commandDispatcher(this, command_line);
    pos_found = _read_buffer.find("\r\n");
  }
}
