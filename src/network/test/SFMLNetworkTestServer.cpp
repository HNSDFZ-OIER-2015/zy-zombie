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

        sf::Packet pack;
        pack << "Hello, world!";
        conn.send(pack);

        conn.receive(pack);
        std::string data;
        if (pack >> data) {
            printf("%s\n", data.c_str());
        } else {
            puts("(error) No data");
        }
    }
    puts("233");

    conn.disconnect();
    listener.close();

    return 0;
}  // function main
