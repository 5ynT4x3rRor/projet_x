#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include "scan.hpp"
#include "../target/set_target_ipv4.hpp"
#include "../target/set_range_ipv4.hpp"

void execute_scan() {
    // 1. Récupération de la cible IPv4 active via le getter
    ipv4_target_t *target_ptr = get_target_ipv4();
    if (!target_ptr || target_ptr->ip_str[0] == '\0') {
        std::cout << "\033[1;31m[-] Erreur : Aucune cible définie (utilise 'set target <ip>').\033[0m\n";
        return;
    }
    std::string target(target_ptr->ip_str);

    // 2. Définition des ports (valeurs par défaut si non définis)
    int port_start = 0; 
    int port_end = 0;   

    int start = (port_start != 0) ? port_start : 1;
    int end = (port_end != 0) ? port_end : 1024; // Par défaut les 1024 premiers ports

    std::cout << "\033[1;36m[*] Démarrage du scan TCP sur " << target << " (ports " << start << " à " << end << ")...\033[0m\n";

    // 3. Boucle de scan
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

        // Tentative de connexion (retourne -1 / EINPROGRESS en non-bloquant, ce qui est normal)
        connect(sock, (struct sockaddr*)&target_addr, sizeof(target_addr));

        // 4. Vérification de l'état du socket avec select (timeout de 100ms)
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(sock, &fdset);

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000; 

        if (select(sock + 1, NULL, &fdset, NULL, &tv) == 1) {
            int so_error = 0;
            socklen_t len = sizeof(so_error);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
            if (so_error == 0) {
                std::cout << "\033[1;32m[+] Port " << port << " ouvert\033[0m\n";
            }
        }
        
        close(sock);
    }
    std::cout << "\033[1;32m[+] Scan terminé.\033[0m\n";
}
