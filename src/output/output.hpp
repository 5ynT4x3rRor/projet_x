#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <string>

void save_scan_report(const std::string& filename, const std::string& target_ip, int start_port, int end_port, const std::string& format);

#endif
