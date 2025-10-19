#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

Client::Client(int fd, Server* serverPtr)
    : _fd(fd), _out_buffer(""), _registered(false), _nick_set(false),
      _pass_set(false), _server_ptr(serverPtr) {}

std::string Client::extractAndEraseFromBuffer(size_t pos_found) {
  std::string toRetrun = _read_buffer.substr(0, pos_found);
  _read_buffer = _read_buffer.substr(pos_found + 2);
  return toRetrun;
}

bool Client::isRegistered() { return _registered; }
std::string Client::getNickname() { return _nickName; }

// void Client::send_reply(const std::string &numeric,
//                         const std::string &content) {
//   // std::string prefix = ":ft_irc.local";
//   // std::string recipient = this->isRegistered() ? this->getNickname() : "*";
//   
//
//   std::string full_message = numeric + " " +  + " " + recipient + " " + content;
//
//   // this->getOutBuffer().append(full_message + "\r\n");
//   // this->setPollOut(true);
// }

bool Client::getPassBool(){
  return _pass_set;
}

bool Client::getUserBool(){
  return _user_set;
}

void Client::setRegistration(){
  _registered = true;
}

void Client::setNickBool(bool state){
  _nick_set = state;
}


void Client::setPassBool(bool state){
  _pass_set = state;
}

void Client::setUserBool(bool state){
  _user_set = state;
}
int Client::getFd(){
  return _fd;
}

std::string &Client::getReadBuffer() { return _read_buffer; }
    
void Client::process_and_extract_commands() {
  size_t pos_found = _read_buffer.find("\r\n");
  while (pos_found != std::string::npos) {
    std::string command_line = extractAndEraseFromBuffer(pos_found);
    _server_ptr->Server::commandDispatcher(this, command_line);
    pos_found = _read_buffer.find("\r\n");
  }
}
