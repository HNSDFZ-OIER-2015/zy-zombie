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

    sf::Packet pack;
    pack << "Hello, world!";
    socket.send(pack);

    socket.receive(pack);
    std::string data;
    if (pack >> data) {
        printf("%s\n", data.c_str());
    } else {
        puts("(error) No data");
    }

    socket.disconnect();

    return 0;
}  // function main
