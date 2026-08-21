#ifndef SET_TARGET_IPV6_HPP
#define SET_TARGET_IPV6_HPP

#include <string>

class SetTargetIPv6 {
private:
     std::string targetIPv6;

public:
     SetTargetIPv6();
     explicit SetTargetIPv6(const std::string& initialTarget);

     bool setTarget(const std::string& input);
     std::string getTarget() const;
     bool validateIPv6(const std::string& ip) const;
 };

// Déclaration de la fonction globale pour le main.cpp
void handle_target_ipv6_command(const std::string& command);

#endif // SET_TARGET_IPV6_HPP
