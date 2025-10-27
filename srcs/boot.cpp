#include "../includes/Server.hpp"
#include <cstddef>

std::vector<std::string> splitVector(const std::string &input_string,
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
    std::stringstream ss(positional_part);
    std::string segment;
    while (std::getline(ss, segment, delimiter)) {
        if (!segment.empty())
            tokens.push_back(segment);
    }
    if (!trailing_part.empty()){
        tokens.push_back(trailing_part);
    }
    return tokens;
}
e_cmd_bot_type getCmdTtype(const std::string & cmdName)
{
    const std::string botCmdsNames[] = {
        "!help",
        "!time",
        "!joke",
    };
    for (size_t i = 0; i < 4; i++) {
        if (cmdName == botCmdsNames[i])
            return e_cmd_bot_type(i);
    }
    return BOT_CMD_UNKNOWN;
}

void SendPrivateMessage(int bot_fd, std::string & SendClient, const std::string &Message){
    std::string FullMessage =  "PRIVMSG " + SendClient + " :" + Message + "\r\n" ;
    std::cout << "FullMessage: " << FullMessage << std::endl;
    if (send(bot_fd, Message.c_str(), Message.length(), 0) == -1){
        throw std::runtime_error("Bot connection failed during send.");
    }

}

void handleBotJoke(int bot_fd, std::string & SendClient, const std::vector<std::string>& args) {
    
    if (args.size() != 1) { 
        // Queue error: "!joke takes no arguments. Usage: !joke"
        return; 
    }
    std::vector<std::string> jokes;
    jokes.push_back("I’d tell them a UDP joke but there’s no guarantee that they would get it.");
    jokes.push_back("Q. How do robots eat pizza? A. One byte at a time.");
    jokes.push_back("Q. How did the first program die? A. It was executed.");
    jokes.push_back("Q. Why do Java programmers wear glasses? A. They can’t C#");
    jokes.push_back("Q. What is the difference between C++ and C? A. Just 1.");
    jokes.push_back("Q. Whats the object-oriented way to become wealthy? A. Inheritance");
    jokes.push_back("Q. What do computers and air conditioners have in common? A. They both become useless when you open windows.");
    size_t joke_count = jokes.size();
    int random_index = rand() % joke_count; 
    std::string selected_joke = jokes[random_index];
    SendPrivateMessage(bot_fd, SendClient, selected_joke);
}

void HelpCmd(int bot_fd, std::string & Sendclient, std::vector<std::string>& arguments){
    std::string message;
    std::vector<std::string> HelpMessages;
    HelpMessages.push_back("-- FTBot Command List --");
    HelpMessages.push_back("help :Displays this command list.");
    HelpMessages.push_back("time :Displays the current server time and date.");
    HelpMessages.push_back("------------------------");
    std::cout << "here" << std::endl;
    for (size_t i = 0; i < HelpMessages.size(); i++)
         SendPrivateMessage(bot_fd, Sendclient, HelpMessages[i]);
}

void timeCmd(int bot_fd, std::string & Sendclient, std::vector<std::string>& arguments){
     std::string message;
     time_t RawSeconds;
     time(&RawSeconds);
     struct tm *TimeInfo = localtime(&RawSeconds);
     char TimeBuffer[100];
     size_t len = strftime(TimeBuffer, sizeof(TimeBuffer), 
                           "Current server time: %H:%M:%S on %Y-%m-%d", 
                           TimeInfo);
     if (len > 0)
         SendPrivateMessage(bot_fd, Sendclient, TimeBuffer);
}

void processBotCommand(int bot_fd, std::string & FullMessage){
    std::vector<std::string> splitedCommand =
        splitVector(FullMessage, ' ');
    for (size_t i = 0; i < splitedCommand.size(); i++){
        std::cout << "splitedCommand: " << i << " = " << splitedCommand[i] << std::endl; 

    }
    if (splitedCommand.empty()) {
        return; 
    }
    else if (splitedCommand.size() > 2)
    {
        std::string message = "Syntax Error: Too many arguments for the Bot command. Use help for command list.";
        SendPrivateMessage(bot_fd, splitedCommand[0], message);
        return;
    }
    //to do: to test if no command is given
    std::string Sendclient = splitedCommand[0];

    e_cmd_bot_type cmdType = getCmdTtype(splitedCommand[1]) ;
    switch (cmdType) {
        case BOT_CMD_HELP:
            HelpCmd(bot_fd, Sendclient, splitedCommand );
            break;
        case BOT_CMD_TIME:
            timeCmd(bot_fd, Sendclient, splitedCommand );
            break;
        case BOT_CMD_JOKE:
            handleBotJoke(bot_fd, Sendclient, splitedCommand );
            break;
        case BOT_CMD_UNKNOWN:
            std::string message = splitedCommand[1] + "is not a recognized command. Use help for a list of available commands.";
            SendPrivateMessage(bot_fd, Sendclient, message);
            break;
        // ...
    }
}

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

void SendMessage(int bot_fd, const std::string &Message){
    if (send(bot_fd, Message.c_str(), Message.length(), 0) == -1){
        throw std::runtime_error("Bot connection failed during send.");
    }
}

void start_bot_loop(int bot_fd, const std::string& password) {
    
    std::string bot_nick = "FTBot";
    std::string bot_user = "BotUser";
     

    SendMessage(bot_fd, "PASS " + password + "\r\n");
    SendMessage(bot_fd, "NICK " + bot_nick + "\r\n");
    SendMessage(bot_fd, "USER " + bot_user + " 0 * :" + bot_user + " Service Bot\r\n");
    std::string bot_in_buffer;
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
        bot_in_buffer.append(buff, readed_count);
        size_t pos;
        while ((pos = bot_in_buffer.find("\r\n")) != std::string::npos) {
            
            // a. Extract complete command
            std::string full_message = bot_in_buffer.substr(0, pos);
            
            // b. Call command processor (The message is now complete!)
            processBotCommand(bot_fd, full_message); 
            
            // c. Erase the processed message plus the delimiter
            bot_in_buffer.erase(0, pos + 2); 
        }
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
