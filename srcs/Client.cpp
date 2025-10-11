
#include "../includes/Client.hpp"

Client::Client(int fd)
    : _fd(fd), _out_buffer(""), _registered(false), _nick_set(false),
      _pass_set(false) {}
