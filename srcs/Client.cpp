#include "../includes/Client.hpp"
#include <vector>

Client::Client(int fd, Server* serverPtr)
    : _fd(fd), _outBuffer(""), _registered(false), _nickSet(false),
      _passSet(false), _serverPtr(serverPtr) {}

//Get the commnad untile the /r/n return it and remove it from the buffer
std::string Client::extractAndEraseFromBuffer(size_t pos_found) {
  std::string toRetrun = _readBuffer.substr(0, pos_found);
  _readBuffer = _readBuffer.substr(pos_found + 2);
  return toRetrun;
}

void Client::send_reply(const std::string &numeric,
                        const std::string &content) {
  std::string prefix = ":ft_irc.local";
  std::string recipient;
  
  if (this->isRegistered() || !this->getNickname().empty()) {
    recipient = this->getNickname();
  } else {
    recipient = "*"; 
  }

  std::string full_message = prefix + " " + numeric + " " + recipient + " " + content;

  this->getOutBuffer().append(full_message + "\r\n");
  this->setPollOut(true);
}

//Getters
std::string &Client::getReadBuffer() {
  return _readBuffer; 
}

int Client::getFd(){
  return _fd;
}

bool Client::isRegistered() {
  return _registered; 
}

bool Client::getPassState(){
  return _passSet;
}

bool Client::getUserState(){
  return _userSet;
}

std::string Client::getNickname() {
  return _nickName; 
}

std::string & Client::getOutBuffer(){
    return _outBuffer;
}

bool Client::getNicknameState(){
  return _nickSet;
}

//Setters
void Client::setRegistration(){
  _registered = true;
}

void Client::setNickState(bool state){
  _nickSet = state;
}

void Client::setPassState(bool state){
  _passSet = state;
}

void Client::setUserState(bool state){
  _userSet = state;
}

void Client::setPollOut(bool state){
    std::vector<struct pollfd>& poll_fds = _serverPtr->getPollfds();
    for (size_t i = 0; i < poll_fds.size(); ++i) {

        if (poll_fds[i].fd == _fd) {
            if (state == true) {
                poll_fds[i].events |= POLLOUT; 
            } else {
                poll_fds[i].events &= ~POLLOUT;
            }
            return; 
        }
    }
    //TODO If the loop finishes without finding the FD, the client may have disconnected.
}
    
// retreave the commmand and its argument then run it 
void Client::processAndExtractCommands() {
  size_t pos_found = _readBuffer.find("\r\n");
  while (pos_found != std::string::npos) {
    std::string command_line = extractAndEraseFromBuffer(pos_found);
    _serverPtr->Server::commandDispatcher(this, command_line);
    pos_found = _readBuffer.find("\r\n");
  }
}
