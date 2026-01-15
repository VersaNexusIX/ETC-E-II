#ifndef UI_HELPER_HPP
#define UI_HELPER_HPP

#include <string>

extern const std::string C_LAV, C_PNK, C_HOT, C_MNT, C_WHT, C_RST;

void startup_loader();
void show_header();
void status_bar(std::string filename, int progress, std::string state);

#endif
