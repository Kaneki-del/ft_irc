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

  void processAndExtractCommands();
  std::string extractAndEraseFromBuffer(size_t posFound, int delimiterLen);


  std::string getNickname();
  bool getPassState();
  std::string &getReadBuffer();
  std::string &getOutBuffer();
  bool isRegistered();
  bool getUserState();
  bool getNicknameState();
  int getFd();
  const std::string & getIpAddress() const ;

  void setRegistration();
  void setPassState(bool state);
  void setUserState(bool state);
  void setNickState(bool state);
  void setPollOut(bool state);
  void setIpAddress(const std::string &addrr);

  void send_reply(const std::string &numeric, const std::string &content);


private:
  int _fd;
  std::string _readBuffer;
  std::string _outBuffer;
  bool _registered;
  bool _nickSet;
  bool _passSet;
  bool _userSet;
  std::string _ipAddrres;
  Server* _serverPtr;
};
#endif
