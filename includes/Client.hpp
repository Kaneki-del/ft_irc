#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>

class Client {
public:
  Client(int fd);
  std::string _nickname;
  std::string _username;
  void check_registration();
  std::string &getReadBuffer();
  void process_and_extract_commands();
  std::string extractAndEraseFromBuffer(size_t pos_found);
  // void setAdress(const &std::string A);

private:
  int _fd;
  std::string _read_buffer;
  std::string _out_buffer;
  bool _registered;
  bool _nick_set;
  bool _pass_set;
  std::string _ip_address;
};
#endif
