#include "../includes/Server.hpp"
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
    std::cerr << "[DEBUG] 5. Listening on port " << port << "." << std::endl;

    struct pollfd listener_poll_fd = {};

    listener_poll_fd.fd = _listener_fd;

    listener_poll_fd.events = POLLIN;
    listener_poll_fd.revents = 0;
    _poll_fds.push_back(listener_poll_fd);
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
    // 1. Retrieve the Client object pointer
    Client *client = _clients[current_fd];
    std::cerr << "[DEBUG] handle_client_command." << std::endl;

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
        // disconnect_client(current_fd);
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
        // --- SUCCESS: Data was Read ---
        temp_buffer[bytes_read] = '\0';
        // This is the first half of the parsing solution: APPEND
        std::string temp = client->getReadBuffer().append(temp_buffer, bytes_read);
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
    std::vector<std::string> tokens;
    std::stringstream ss(input_string);
    std::string segment;
    while (std::getline(ss, segment, delimiter)) {
        if (!segment.empty())
            tokens.push_back(segment);
    }
    if (!trailing_part.empty()) 
        tokens.push_back(trailing_part);
    return tokens;
}

void Server::commandDispatcher(Client *client, std::string commandLine) {
    (void)client;
    std::vector<std::string> splitedCommand =
        split_string_to_vector(commandLine, ' ');
    if (splitedCommand.empty()) {
        return; 
    }
    std::string command = splitedCommand[0];
    e_cmd_type cmd = this->getCommandType(command);

    switch (cmd) {
        case CMD_PASS:
            handle_pass_command(client, splitedCommand); 
            break;
        case CMD_NICK:
            // ... implementation
            break;
        case CMD_USER:
            // ... implementation
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
        std::cerr << "[DEBUG] Polling on " << _poll_fds.size() << " FDs."
            << std::endl;
        std::cerr << "[DEBUG] Listener FD (should be 3): " << _poll_fds[0].fd
            << std::endl;
        int ret = poll(&_poll_fds[0], _poll_fds.size(), -1);
        std::cerr << "[DEBUG] Poll returned: " << ret << std::endl;
        if (ret < 0) {
            throw std::runtime_error("Poll fatal error");
        }
        for (long unsigned int i = 0; i < _poll_fds.size(); ++i) {
            int current_fd = _poll_fds[i].fd;

            if (_poll_fds[i].revents & POLLIN) {
                if (current_fd == _listener_fd)
                    handle_new_connection();
                else
                    handle_client_command(current_fd);
            }
        }
    }
}




