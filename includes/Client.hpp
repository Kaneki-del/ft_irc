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
  std::string _nickname;
  std::string _username;
  void check_registration();
  std::string &getReadBuffer();
  void process_and_extract_commands();
  std::string extractAndEraseFromBuffer(size_t pos_found);
  void send_reply(const std::string &numeric, const std::string &content);
  bool is_registered();
  std::string getNickname();
  void setPassBool(bool state);
  int getFd();
  // void setAdress(const &std::string A);

private:
  int _fd;
  std::string _read_buffer;
  std::string _out_buffer;
  bool _registered;
  bool _nick_set;
  bool _pass_set;
  std::string _ip_address;
  Server* _server_ptr;
};
#endif
