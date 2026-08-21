#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include "scan.hpp"
#include "set_target_ipv4.hpp"

// Fonction utilitaire pour tester un port avec un timeout (ex: 500ms)
bool scan_single_port(const char* ip_str, int port, int timeout_ms) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return false;
    }

    // Mettre le socket en mode non bloquant
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    sockaddr_in target_addr{};
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, ip_str, &target_addr.sin_addr) <= 0) {
        close(sock);
        return false;
    }

    // Tentative de connexion (va retourner -1 immédiatement en mode non bloquant)
    int res = connect(sock, (struct sockaddr*)&target_addr, sizeof(target_addr));
    
    if (res < 0) {
        if (errno == EINPROGRESS) {
            fd_set fdset;
            FD_ZERO(&fdset);
            FD_SET(sock, &fdset);

            timeval tv;
            tv.tv_sec = timeout_ms / 1000;
            tv.tv_usec = (timeout_ms % 1000) * 1000;

            // Attente de la connexion avec select()
            if (select(sock + 1, nullptr, &fdset, nullptr, &tv) > 0) {
                int sockerr;
                socklen_t len = sizeof(sockerr);
                getsockopt(sockerr = sock, SOL_SOCKET, SO_ERROR, &sockerr, &len);
                // Si sockerr est à 0, la connexion a réussi
                if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &sockerr, &len) == 0 && sockerr == 0) {
                    close(sock);
                    return true;
                }
            }
        }
    } else {
        // Connexion immédiate (rare en TCP distant mais possible)
        close(sock);
        return true;
    }

    close(sock);
    return false;
}

// Implémentation de la fonction de scan principale
void execute_scan() {
    ipv4_target_t* target = get_target_ipv4();
    if (!target) {
        std::cout << "[-] Erreur : Aucune cible définie pour le scan. Utilisez 'set target <IP>'.\n";
        return;
    }

    std::cout << "[*] Lancement du scan TCP sur la cible : " << target->ip_str << " ...\n";

    // Exemple de scan sur quelques ports communs (tu pourras lier ça à ton set port plus tard)
    int ports_to_check[] = {21, 22, 23, 25, 53, 80, 443, 445, 3306, 8080};
    int num_ports = sizeof(ports_to_check) / sizeof(ports_to_check[0]);

    for (int i = 0; i < num_ports; i++) {
        int port = ports_to_check[i];
        std::cout << "[*] Test du port " << port << "... \r" << std::flush;
        
        if (scan_single_port(target->ip_str, port, 300)) { // 300ms de timeout
            std::cout << "[+] Port " << port << " est OUVERT          \n";
        }
    }

    std::cout << "[+] Scan terminé.\n";
}
