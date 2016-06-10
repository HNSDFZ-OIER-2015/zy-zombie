#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

int main() {
    if (SDL_Init(0) < 0) {
        printf("SDL_Init: %s\n", SDL_GetError());
        exit(-1);
    }

    if (SDLNet_Init() < 0) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(-1);
    }

    atexit(SDLNet_Quit);
    atexit(SDL_Quit);

    IPaddress ip;
    TCPsocket sock;

    if (SDLNet_ResolveHost(&ip, "localhost", 2444) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(-1);
    }

    sock = SDLNet_TCP_Open(&ip);
    if (!sock) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(-1);
    }

    IPaddress *remote_ip;

    remote_ip = SDLNet_TCP_GetPeerAddress(sock);
    if (!remote_ip) {
        printf("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
    } else {
        printf("Remote: %d.%d.%d.%d:%d\n", remote_ip->host & 0xFF000000,
               remote_ip->host & 0x00FF0000, remote_ip->host & 0x0000FF00,
               remote_ip->host & 0x000000FF, remote_ip->port);
    }

    SDLNet_TCP_Close(sock);

    return 0;
}  // function main
