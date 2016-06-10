#include <cstdio>
#include <string>
#include <SFML/Network.hpp>

int main() {
    sf::TcpListener listener;

    // bind the listener to a port
    if (listener.listen(2345) != sf::Socket::Done) {
        printf("Cannot listen to port.\n");
    }

    // accept a new connection
    sf::TcpSocket conn;
    puts("Waiting...");
    if (listener.accept(conn) == sf::Socket::Done) {
        printf("%d\n", conn.getRemotePort());

        char buffer[100] = "Hello, world!";
        conn.send(buffer, sizeof(buffer));

        size_t received;
        conn.receive(buffer, sizeof(buffer), received);
        printf("%s\n", buffer);
    }
    puts("233");

    conn.disconnect();
    listener.close();

    return 0;
}  // function main
