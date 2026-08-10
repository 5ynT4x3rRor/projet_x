#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "scan.hpp"
#include "target/target.hpp"
#include "target/range.hpp"

void execute_scan() {
    // Vérification de base
    if (target.empty()) {
        std::cout << "\033[1;31m[-] Erreur : Aucune cible définie (utilise 'set target : <ip>').\033[0m\n";
        return;
    }
    
    int start = (port_start != 0) ? port_start : 1;
    int end = (port_end != 0) ? port_end : 1024; // Par défaut les 1024 premiers si non définis

    std::cout << "\033[1;36m[*] Démarrage du scan TCP sur " << target << " (ports " << start << " à " << end << ")...\033[0m\n";

    // Boucle de scan basique
    for (int port = start; port <= end; ++port) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) continue;

        // Rendre le socket non-bloquant pour gérer un timeout rapide
        long arg = fcntl(sock, F_GETFL, NULL);
        fcntl(sock, F_SETFL, arg | O_NONBLOCK);

        sockaddr_in target_addr{};
        target_addr.sin_family = AF_INET;
        target_addr.sin_port = htons(port);
        inet_pton(AF_INET, target.c_str(), &target_addr.sin_addr);

        // Tentative de connexion
        connect(sock, (struct sockaddr*)&target_addr, sizeof(target_addr));

        // Utilisation de select pour un timeout court (ex: 50 millisecondes par port)
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);

        timeval tv{};
        tv.tv_sec = 0;
        tv.tv_usec = 50000; // 50 ms

        if (select(sock + 1, NULL, &fdset, NULL, &tv) == 1) {
            int so_error;
            socklen_t len = sizeof(so_error);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
            if (so_error == 0) {
                std::cout << "\033[1;32m[+] Port " << port << " : OUVERT\033[0m\n";
            }
        }

        close(sock);
    }
    std::cout << "\033[1;36m[*] Scan terminé.\033[0m\n";
}
