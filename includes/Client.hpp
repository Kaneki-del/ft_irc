#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include <map>
#include <string>
#include <vector>
class Client;
#include "Server.hpp"

class Client {
public:
  Client(int fd, Server* serverPtr);
  std::string _nickName;
  std::string _userName;
  std::string _realName;

  void process_and_extract_commands();
  std::string extractAndEraseFromBuffer(size_t pos_found);


  std::string getNickname();
  bool getPassState();
  std::string &getReadBuffer();
  std::string &getOutBuffer();
  bool isRegistered();
  bool getUserState();
  int getFd();

  void setRegistration();
  void setPassState(bool state);
  void setUserState(bool state);
  void setNickState(bool state);
  void setPollOut(bool state);
  // void setAdress(const &std::string A);

  void send_reply(const std::string &numeric, const std::string &content);


private:
  int _fd;
  std::string _read_buffer;
  std::string _out_buffer;
  bool _registered;
  bool _nick_set;
  bool _pass_set;
  bool _user_set;
  std::string _ip_address;
  Server* _server_ptr;
};
#endif
