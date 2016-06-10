#include <cstdio>
#include <string>
#include <SFML/Network.hpp>

int main() {
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("localhost", 2345);
    if (status != sf::Socket::Done) {
        printf("Cannot connect to the server.\n");
    }

    printf("%d\n", socket.getRemotePort());

    char buffer[100] = "Hello, world!";
    socket.send(buffer, sizeof(buffer));

    size_t received;
    socket.receive(buffer, sizeof(buffer), received);
    printf("%s\n", buffer);

    socket.disconnect();

    return 0;
}  // function main
