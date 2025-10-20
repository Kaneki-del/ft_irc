#include "../includes/Server.hpp"
#include <vector>
Server::Server(const int port, const std::string password)
    : _password(password), _port(port), _listener_fd(-1),
    _server_name("ft_irc.local") {
    this->_command_map["PASS"] = CMD_PASS;
    this->_command_map["NICK"] = CMD_NICK;
    this->_command_map["USER"] = CMD_USER;
    _listener_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (_listener_fd < 0) {
        throw std::runtime_error("Socket creation failed.");
    }
    fcntl(_listener_fd, F_SETFL, O_NONBLOCK);

    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(_port);
    if (bind(_listener_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ==
        -1) {
        std::cerr << "Error: Failed to bind socket to port " << _port << "."
            << std::endl;

        close(_listener_fd);

        throw std::runtime_error(
            "Fatal server initialization error (bind failed).");
    }

    listen(_listener_fd, SOMAXCONN);
    if (listen(_listener_fd, SOMAXCONN) == -1) {
        close(_listener_fd);
        throw std::runtime_error("Listen failed.");
    }

    struct pollfd listener_poll_fd = {};

    listener_poll_fd.fd = _listener_fd;

    listener_poll_fd.events = POLLIN;
    listener_poll_fd.revents = 0;
    _poll_fds.push_back(listener_poll_fd);
}

std::vector<struct pollfd> & Server::getPollfds(){
    return _poll_fds;
}

void Server::handle_outgoing_data(int clientFd){

    Client* client = _clients[clientFd];
    std::string& buffer = client->getOutBuffer();
    const char* data_ptr = buffer.c_str(); 
    size_t length = buffer.length();

    ssize_t bytes_sent = send(client->getFd(), data_ptr, length, 0);
    if (bytes_sent > 0) {
        buffer.erase(0, bytes_sent);
        if (buffer.empty()) {
            client->setPollOut(false); 
        }
    } else if (bytes_sent < 0) {
        //TODO
        if (errno != EWOULDBLOCK && errno != EAGAIN) {
            // Fatal error (e.g., connection reset). Treat as disconnect.
            // T
            // this->disconnect_client(client->getFd());
        }
    }

}

void Server::checkRegistration(Client * client){
  if (client->getPassState() && client->getUserState())
        client->setRegistration();
}
e_cmd_type Server::getCommandType(std::string command) {
    
    std::map<std::string, e_cmd_type>::iterator it = _command_map.find(command);

    if (it != _command_map.end()) {
        return it->second;
    } else {
        return CMD_UNKNOWN; 
    }
}
// void Server::setAdress(const &std::string A) { _ip_adress = A; }

void Server::handle_new_connection() {
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    std::memset(&client_addr, 0, addr_size);

    int new_client_fd =
        accept(_listener_fd, (struct sockaddr *)&client_addr, &addr_size);
    // TODO check later for fealure -1
    //
    //
    _clients[new_client_fd] = new Client(new_client_fd, this);
    // TODO check if we need to stor the adress of the client
    //  setAdress(client_addr);

    if (fcntl(new_client_fd, F_SETFL, O_NONBLOCK) == -1) {
        // TODO true the apropriate error
        close(new_client_fd);
        return;
    }
    // TODO create a function return this struct
    struct pollfd client_poll_fd = {};
    client_poll_fd.fd = new_client_fd;
    client_poll_fd.events = POLLIN;
    client_poll_fd.revents = 0;
    _poll_fds.push_back(client_poll_fd);
}

void Server::handle_client_command(const int current_fd) {

    Client *client = _clients[current_fd];

    if (!client) {
        // Handle case where client object is somehow missing (fatal error)
        return;
    }

    char temp_buffer[1024];

    ssize_t bytes_read =
        recv(current_fd, temp_buffer, sizeof(temp_buffer) - 1, 0);

    if (bytes_read == 0) {
        // --- MANDATORY: Client Disconnect (Graceful Close) ---
        std::cerr << "[LOG] Client FD " << current_fd << " disconnected gracefully."
            << std::endl;
        close(current_fd);
        Client* client_to_delete = _clients[current_fd]; 
        _nicknames.erase(client_to_delete->getNickname());
        delete client_to_delete;
        _clients.erase(current_fd);

    } else if (bytes_read < 0) {
        std::cerr << "Error in read" << std::endl;
    }
    // EWOULDBLOCK or EAGAIN means no more data to read, which is NORMAL.
    if (errno == EWOULDBLOCK || errno == EAGAIN) {
        return;
        // } else {
        // A genuine connection error (e.g., connection reset). Treat as
        // disconnect.
        // perror("Recv error");
        // disconnect_client(current_fd);
        // }
    } else {
        temp_buffer[bytes_read] = '\0';
        std::string temp = client->getReadBuffer().append(temp_buffer, bytes_read);
        std::cout << "tmp: " << temp << std::endl;
        client->process_and_extract_commands();
    }
}

std::vector<std::string> split_string_to_vector(const std::string &input_string,
                                                char delimiter) {
    size_t colon_pos = input_string.find(':');
    std::string positional_part;
    std::string trailing_part;
    if (colon_pos == std::string::npos) {
        positional_part = input_string;
        trailing_part = "";
    } else {
        positional_part = input_string.substr(0, colon_pos);

        if (colon_pos + 1 < input_string.length()) {
            trailing_part = input_string.substr(colon_pos + 1);
        }
    }
    std::cout << "positional_part: " << positional_part << std::endl;
    std::cout << "trailing_part: " << trailing_part << std::endl;
    std::vector<std::string> tokens;
    std::stringstream ss(positional_part);
    std::string segment;
    while (std::getline(ss, segment, delimiter)) {
        if (!segment.empty())
            tokens.push_back(segment);
    }
    if (!trailing_part.empty()){
        std::cout << "trailing_part: " << trailing_part << std::endl;
        tokens.push_back(trailing_part);
    }
    std::cout << "tokens[1] " << tokens[1] << std::endl;
    return tokens;
}

void Server::commandDispatcher(Client *client, std::string commandLine) {
    std::vector<std::string> splitedCommand =
        split_string_to_vector(commandLine, ' ');
    if (splitedCommand.empty()) {
        return; 
    }
    std::cout << "args[0]: " << splitedCommand[0] << std::endl;
    std::cout << "args[1]: " << splitedCommand[1] << std::endl;

    std::string command = splitedCommand[0];
    e_cmd_type cmd = this->getCommandType(command);

    switch (cmd) {
        case CMD_PASS:
            handle_pass_command(client, splitedCommand); 
            break;
        case CMD_NICK:
            handleNickCommand(client, splitedCommand);
            break;
        case CMD_USER:
            handleUserCommand(client, splitedCommand);
            break;
        case CMD_UNKNOWN:
            // ... implementation
            break;

            client->send_reply("421", /* command verb */ + " :Unknown command"); 
            break;
    }
}
void Server::run() {

    std::cerr << "[DEBUG] 4. Runing the server." << std::endl;

    while (true) {
        std::cerr <<  GREEN <<"Polling on " << _poll_fds.size() << " FDs."
            << std::endl;
        int ret = poll(&_poll_fds[0], _poll_fds.size(), -1);
        std::cerr << "Poll returned: " << ret << std::endl;
        if (ret < 0) {
            throw std::runtime_error("Poll fatal error");
        }
        for (long unsigned int i = 0; i < _poll_fds.size(); ++i) {
            int current_fd = _poll_fds[i].fd;

            //TODO
            // if (_poll_fds[i].revents & (POLLHUP | POLLERR)) {
            //     // This MUST be the first check (or handle it inside the POLLIN block if recv returns 0)
            //     disconnect_client(current_fd, i); // <-- Must pass index 'i' for vector erase
            //     i--; // CRITICAL: Decrement the loop counter after erasing from vector
            //     continue;
            // }
            
            if (_poll_fds[i].revents & POLLIN) {
                if (current_fd == _listener_fd)
                    handle_new_connection();
                else
                    handle_client_command(current_fd);
            }

            else if (_poll_fds[i].revents & POLLOUT)
                handle_outgoing_data(current_fd);
        }
    }
}




