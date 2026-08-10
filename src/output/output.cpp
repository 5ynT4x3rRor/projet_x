#include <iostream>
#include <fstream>
#include <cstdlib>
#include "output.hpp"

// Fonction pour récupérer dynamiquement le dossier HOME de l'utilisateur
std::string get_home_directory() {
    const char* home = std::getenv("HOME");
    if (home != nullptr) {
        return std::string(home);
    }
    return "."; // Fallback sur le dossier courant si HOME n'est pas défini
}

void save_scan_report(const std::string& filename, const std::string& target_ip, int start_port, int end_port, const std::string& format) {
    if (format == "normal") {
        // Construction automatique du chemin complet dans le dossier utilisateur
        std::string full_path = get_home_directory() + "/" + filename;

        std::ofstream outfile(full_path);
        if (!outfile) {
            std::cout << "\033[1;31m[-] Erreur : Impossible de créer ou d'ouvrir le fichier : " << full_path << "\033[0m\n";
            return;
        }

        outfile << "# ProjetX Scan Report\n";
        outfile << "# Cible : " << target_ip << "\n";
        outfile << "# Plage de ports : " << start_port << "-" << end_port << "\n";
        outfile << "--------------------------------------------------\n";
        outfile << "PORT\tSTATE\tSERVICE (Exemple)\n";
        outfile << "--------------------------------------------------\n";
        outfile << "[*] Rapport généré avec succès.\n";
        
        outfile.close();
        std::cout << "\033[1;32m[+] Rapport enregistré dans : " << full_path << "\033[0m\n";
    } else {
        std::cout << "\033[1;31m[-] Erreur : Format d'export inconnu (formats supportés pour l'instant : normal).\033[0m\n";
    }
}
