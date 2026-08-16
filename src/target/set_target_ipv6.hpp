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

#endif // SET_TARGET_IPV6_HPP
