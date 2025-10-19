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
  std::string &getReadBuffer();
  void process_and_extract_commands();
  std::string extractAndEraseFromBuffer(size_t pos_found);
  void send_reply(const std::string &numeric, const std::string &content);
  bool isRegistered();
  std::string getNickname();
  void setPassBool(bool state);
  void setUserBool(bool state);
  void setNickBool(bool state);
  bool getPassBool();
  bool getUserBool();
  int getFd();
  void setRegistration();
  // void setAdress(const &std::string A);

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
