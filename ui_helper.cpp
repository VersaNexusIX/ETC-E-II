#include "ui_helper.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>

const std::string C_LAV = "\033[38;5;183m", C_PNK = "\033[38;5;218m", C_HOT = "\033[38;5;205m", C_MNT = "\033[38;5;158m", C_WHT = "\033[38;5;255m", C_RST = "\033[0m";

void startup_loader() {
    system("clear || cls");
    std::cout << "\n\n\n\n" << C_PNK << "          Initializing VOS Environment... ✨\n\n";
    std::cout << "          " << C_LAV << "<" << C_RST;
    for (int i = 0; i <= 30; i++) { 
        std::string color = (i < 15) ? C_LAV : C_MNT;
        std::cout << color << "━" << std::flush; 
        std::this_thread::sleep_for(std::chrono::milliseconds(30)); 
    }
    std::cout << C_LAV << "> " << C_WHT << "100%\n" << C_RST;
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void show_header() {
    system("clear || cls");
    std::vector<std::string> mascot = {
        "  ⠀⢀⠤⠤⢄⡀⠀⠀⠀⠀⠀⠀⢀⠤⠒⠒⢤  ",
        "  ⠀⠏⠀⠀⠀⠈⠳⡄⠀⠀⡠⠚⠁⠀⠀⠀⠘⡄ ",
        "  ⢸⠀⠀⠀⠤⣤⣤⡆⠀⠈⣱⣤⣴⡄⠀⠀⠀⡇ ",
        "  ⠘⡀⠀⠀⠀⠀⢈⣷⠤⠴⢺⣀⠀⠀⠀⠀⢀⡇ ",
        "  ⠀⠡⣀⣀⣤⠶⠻⡏⠀⠀⢸⡟⠙✨⡤⠤⠼ ",
        "  ⠀⠀⢠⡾⠉⠀⢠⡆⠀⠀⢸⠃⠀⠈⢻⣆   ",
        "  ⠀⠀⣿⣠⢶⣄⠀⡇⠀⠀⠘⠃⣀⡤⢌⣈⡀  ",
        "  ⠀⠀⠀⠀⠀⠙⠼⠀⠀⠀⠀⠿⠋      "
    };
    std::vector<std::string> box = {
        "  ╭═══════ SYSTEM INFO ═══════╮",
        "  │  ᴏs      : VOS EXPERIMENTAL  ",
        "  │  ᴠᴇʀsɪᴏɴ : 2.2 HARDENED      ",
        "  │  ʙɪᴛs    : 320-BIT SPONGE    ",
        "  │  ᴅᴇᴠ     : VERSA_NEXUS_IX    ",
        "  ╰═══════════════════════════╯"
    };
    for(int i=0; i<8; i++) {
        std::cout << C_LAV << mascot[i] << C_RST;
        if(i >= 1 && i <= 6) std::cout << C_MNT << box[i-1] << C_RST;
        std::cout << "\n";
    }
    std::cout << "\n" << C_HOT << "                 🌸  ✨  ETC-E-II  ✨  🌸" << C_RST << "\n";
    std::cout << C_PNK << "     ══════════════════════════════════════════════════════════" << C_RST << "\n";
}

void status_bar(std::string filename, int progress, std::string state) {
    int width = 20; if (filename.length() > 18) filename = filename.substr(0, 15) + "...";
    std::cout << "\r     " << C_LAV << "🎀 <";
    int pos = width * progress / 100;
    for (int i = 0; i < width; ++i) {
        if (i < pos) std::cout << C_PNK << "━";
        else std::cout << "\033[38;5;238m-";
    }
    std::cout << C_LAV << "> " << C_WHT << std::setw(18) << std::left << filename << C_MNT << " " << progress << "% " << C_PNK << "[" << state << "]" << C_RST << "\033[K" << std::flush;
}
