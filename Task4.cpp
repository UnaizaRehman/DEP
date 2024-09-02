#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <sstream>
#include <fstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

const int PORT = 8080;
const std::string HTML_DIR = "./html";

std::mutex cout_mutex;

void handle_client(int client_socket)
{
    char buffer[1024] = {0};
    int valread = read(client_socket, buffer, 1024);

    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Received request:\n"
              << buffer << std::endl;

    if (valread < 0)
    {
        std::cerr << "Failed to read from socket: " << strerror(errno) << std::endl;
        close(client_socket);
        return;
    }

    std::istringstream request(buffer);
    std::string method, path, version;
    request >> method >> path >> version;

    if (path == "/")
    {
        path = "/index.html";
    }

    std::string file_path = HTML_DIR + path;
    std::ifstream file(file_path);
    std::string response;

    if (file.is_open())
    {
        std::stringstream file_stream;
        file_stream << file.rdbuf();
        std::string content = file_stream.str();

        response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: text/html\r\n";
        response += "Content-Length: " + std::to_string(content.size()) + "\r\n";
        response += "\r\n";
        response += content;
    }
    else
    {
        std::string not_found = "<html><body><h1>404 Not Found</h1></body></html>";

        response = "HTTP/1.1 404 Not Found\r\n";
        response += "Content-Type: text/html\r\n";
        response += "Content-Length: " + std::to_string(not_found.size()) + "\r\n";
        response += "\r\n";
        response += not_found;
    }

    send(client_socket, response.c_str(), response.size(), 0);
    close(client_socket);
}

int main()
{
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::vector<std::thread> threads;

    while (true)
    {
        client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (client_socket < 0)
        {
            perror("accept failed");
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Accepted new connection" << std::endl;
        }

        threads.emplace_back(handle_client, client_socket);
    }
    for (auto &thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
    close(server_fd);
    return 0;
}
