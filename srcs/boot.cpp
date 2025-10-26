#include <iostream>
#include <string>
#include <cstdlib>      // For exit, atoi
#include <unistd.h>     // For close
#include <sys/socket.h> // For socket(), connect()
#include <netinet/in.h> // For sockaddr_in, htons
#include <arpa/inet.h>  //

int setup_connection(const std::string& server_ip, int port) {
    int bot_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (bot_socket_fd < 0) {
        throw std::runtime_error("Failed to create socket.");
    }

    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port); 
    serv_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());

    // Connect to the Server (Mandatory error check must be here)
    if (connect(bot_socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        close(bot_socket_fd);
        throw std::runtime_error("Connection refused or failed.");
    }
    
    return bot_socket_fd;
}
void SendPrivateMessage(int bot_fd, const std::string &Message){
    if (send(bot_fd, Message.c_str(), Message.length(), 0) == -1){
        throw std::runtime_error("Bot connection failed during send.");
    }
}

void start_bot_loop(int bot_fd, const std::string& password) {
    
    std::string bot_nick = "FTBot";
    std::string bot_user = "BotUser";
     

    SendPrivateMessage(bot_fd, "PASS " + password);
    SendPrivateMessage(bot_fd, "NICK " + bot_nick);
    SendPrivateMessage(bot_fd, "USER " + bot_user);
    while (true) {
        int readed_count = 0;
	    char buff[1025];

	    readed_count = read(bot_fd, buff, 1024);
	    if (readed_count == 0)
	    {
		    close(bot_fd);
		    // on_disconnected();
	    }
	    else if (readed_count < 0)
	    {
		    close(bot_fd);
		    // on_disconnected();
		    throw std::runtime_error(strerror(errno));
	    }
	    buff[readed_count] = '\0';
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port> <password>" << std::endl;
        return 1;
    }

    int bot_fd = -1;
    std::string server_ip = argv[1];
    int port = std::atoi(argv[2]); 
    std::string password = argv[3];

    if (port <= 1024 || port > 65535) {
        std::cerr << "Error: Invalid port number." << std::endl;
        return 1;
    }

    
    try {
        // 2. Establish Network Connection
        bot_fd = setup_connection(server_ip, port);
        start_bot_loop(bot_fd, password);

    } catch (const std::exception &e) {
        std::cerr << "Bot Fatal Error: " << e.what() << std::endl;
        if (bot_fd > 0)
            close(bot_fd);
        return 1;
    }
    
    return 0;
}
