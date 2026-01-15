#include <iostream>
#include <filesystem>
#include <fstream>
#include <random>
#include <thread>
#include <chrono>

#include "etc_engine.hpp"
#include "ui_helper.hpp"

namespace fs = std::filesystem;

bool work_file(const fs::path& target, const std::string& pass, bool enc, std::mt19937_64& gen) {
    if (enc) {
        std::ifstream fin(target, std::ios::binary); if(!fin) return false;
        std::vector<uint8_t> buf((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>()); fin.close();
        status_bar(target.filename().string(), 30, "INIT");
        uint8_t salt[16], nce[12], tag[32];
        for (int i=0; i<16; i+=8) { uint64_t r=gen(); memcpy(salt+i,&r,8); }
        for (int i=0; i<12; i+=4) { uint32_t r=(uint32_t)gen(); memcpy(nce+i,&r,4); }
        status_bar(target.filename().string(), 70, "CORE");
        ETCEngine eng(pass, salt); eng.absorb_nonce(nce);
        std::string fn = target.filename().string(); uint32_t fl = (uint32_t)fn.size();
        std::vector<uint8_t> md(4+fl); memcpy(md.data(),&fl,4); memcpy(md.data()+4,fn.data(),fl);
        eng.process(md.data(), md.size(), true, true); eng.process(buf.data(), buf.size(), true, false);
        eng.finalize(tag, salt, nce, (uint64_t)buf.size());
        fs::path outP = target.parent_path() / (target.stem().string() + ".etc");
        fs::path tmpP = outP.string() + ".tmp";
        std::ofstream fout(tmpP, std::ios::binary);
        fout.write("ETCE", 4); uint8_t v=0x95; fout.write((char*)&v,1);
        fout.write((char*)salt, 16); fout.write((char*)nce, 12); fout.write((char*)tag, 32);
        fout.write((char*)md.data(), md.size()); fout.write((char*)buf.data(), buf.size());
        fout.close(); if(fs::exists(tmpP)) { fs::rename(tmpP, outP); fs::remove(target); status_bar(target.filename().string(), 100, "DONE"); return true; }
        return false;
    } else {
        std::ifstream fin(target, std::ios::binary | std::ios::ate); size_t fsz = fin.tellg(); fin.seekg(0, std::ios::beg);
        if (fsz < 65) return false;
        status_bar(target.filename().string(), 30, "AUTH");
        uint8_t mg[4], vr, slt[16], nce[12], tg[32]; fin.read((char*)mg, 4); fin.read((char*)&vr, 1);
        if (memcmp(mg, "ETCE", 4) != 0) return false;
        fin.read((char*)slt, 16); fin.read((char*)nce, 12); fin.read((char*)tg, 32);
        uint32_t e_fl; fin.read((char*)&e_fl, 4);
        ETCEngine tmp(pass, slt); tmp.absorb_nonce(nce); tmp.process((uint8_t*)&e_fl, 4, false, true);
        if(e_fl > 1024) return false; 
        status_bar(target.filename().string(), 70, "OPEN");
        std::vector<uint8_t> mb(4 + e_fl); fin.seekg(65, std::ios::beg); fin.read((char*)mb.data(), 4 + e_fl);
        ETCEngine real(pass, slt); real.absorb_nonce(nce); real.process(mb.data(), mb.size(), false, true);
        std::string r_nm((char*)mb.data() + 4, e_fl); size_t p_sz = (size_t)fsz - 65 - mb.size();
        std::vector<uint8_t> py(p_sz); fin.read((char*)py.data(), p_sz); fin.close();
        real.process(py.data(), py.size(), false, false); uint8_t vt[32]; real.finalize(vt, slt, nce, (uint64_t)py.size());
        if (memcmp(tg, vt, 32) == 0) {
            fs::path out = target.parent_path() / r_nm; fs::path tmpD = out.string() + ".tmp";
            std::ofstream fo(tmpD, std::ios::binary); fo.write((char*)py.data(), py.size()); fo.close();
            if(fs::exists(tmpD)) { fs::rename(tmpD, out); fs::remove(target); status_bar(target.filename().string(), 100, "DONE"); return true; }
        }
        return false;
    }
}

int main() {
    uint64_t seed = std::random_device{}();
    seed ^= std::chrono::high_resolution_clock::now().time_since_epoch().count();
    seed ^= (uintptr_t)&seed; std::mt19937_64 gen(seed);
    startup_loader();
    while (true) {
        show_header();
        std::cout << "     " << C_HOT << "✦ " << C_LAV << "[1] Seal File           " << C_HOT << "✦ " << C_LAV << "[2] Seal Folder\n"
                  << "     " << C_HOT << "✦ " << C_LAV << "[3] Open Target         " << C_HOT << "✦ " << C_LAV << "[4] Technical Specs\n"
                  << "     " << C_HOT << "✦ " << C_LAV << "[5] Exit Environment\n\n"
                  << C_MNT << "     Your Command: " << C_RST;
        int opt; if (!(std::cin >> opt)) break;
        if (opt == 5) break;
        if (opt == 4) {
            show_header(); std::cout << "\n     " << C_HOT << "❈  CORE ARCHITECTURE  ❈\n" << C_PNK << "     ══════════════════════════════════════════════════════════\n" << C_WHT << "      • Engine   : 320-Bit Hardened Sponge\n      • Security : Atomic Temp-Swap + Salt/Nonce Binding\n      • Build    : Versa-Hardened v2.2\n" << C_PNK << "     ══════════════════════════════════════════════════════════\n\n     " << C_HOT << "Return to Menu? (Enter)" << C_RST;
            std::cin.ignore(); std::cin.get(); continue;
        }
        std::string pass, path; std::cout << C_LAV << "     Secret Key : " << C_RST; std::cin >> pass;
        std::cout << C_LAV << "     Location   : " << C_RST; std::cin >> path; fs::path target(path);
        if (!fs::exists(target)) { std::this_thread::sleep_for(std::chrono::seconds(1)); continue; }
        std::cout << "\n";
        if (opt == 1) work_file(target, pass, true, gen);
        else if (opt == 2) {
            int c = 0; for (auto& e : fs::recursive_directory_iterator(target)) if (fs::is_regular_file(e) && e.path().extension() != ".etc") if(work_file(e.path(), pass, true, gen)) c++;
            std::cout << "\n     " << C_MNT << "❈ Success! Processed " << c << " items." << C_RST << "\n";
        } else if (opt == 3) {
            if (fs::is_directory(target)) {
                int c = 0; for (auto& e : fs::recursive_directory_iterator(target)) if (fs::is_regular_file(e) && e.path().extension() == ".etc") if(work_file(e.path(), pass, false, gen)) c++;
                std::cout << "\n     " << C_MNT << "❈ Success! Restored " << c << " items." << C_RST << "\n";
            } else work_file(target, pass, false, gen);
        }
        std::cout << C_PNK << "     ══════════════════════════════════════════════════════════\n     " << C_HOT << "Press Enter to Refresh... ฅ^•ﻌ•^ฅ" << C_RST;
        std::cin.ignore(); std::cin.get();
    }
    return 0;
}
