#include "../includes/Client.hpp"
#include <vector>

Client::Client(int fd, Server* serverPtr)
    : _fd(fd), _out_buffer(""), _registered(false), _nick_set(false),
      _pass_set(false), _server_ptr(serverPtr) {}

//Get the commnad untile the /r/n return it and remove it from the buffer
std::string Client::extractAndEraseFromBuffer(size_t pos_found) {
  std::string toRetrun = _read_buffer.substr(0, pos_found);
  _read_buffer = _read_buffer.substr(pos_found + 2);
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
  return _read_buffer; 
}

int Client::getFd(){
  return _fd;
}

bool Client::isRegistered() {
  return _registered; 
}

bool Client::getPassState(){
  return _pass_set;
}

bool Client::getUserState(){
  return _user_set;
}

std::string Client::getNickname() {
  return _nickName; 
}

std::string & Client::getOutBuffer(){
    return _out_buffer;
}

//Setters
void Client::setRegistration(){
  _registered = true;
}

void Client::setNickState(bool state){
  _nick_set = state;
}

void Client::setPassState(bool state){
  _pass_set = state;
}

void Client::setUserState(bool state){
  _user_set = state;
}

void Client::setPollOut(bool state){
    std::vector<struct pollfd>& poll_fds = _server_ptr->getPollfds();
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
void Client::process_and_extract_commands() {
  size_t pos_found = _read_buffer.find("\r\n");
  while (pos_found != std::string::npos) {
    std::string command_line = extractAndEraseFromBuffer(pos_found);
    _server_ptr->Server::commandDispatcher(this, command_line);
    pos_found = _read_buffer.find("\r\n");
  }
}
