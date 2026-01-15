## 🌸 ETC-E-II: Hardened Sponge Encryption Engine ✨

 ETC-E-II is an experimental encryption tool based on a 320-bit sponge construction, designed specifically for the VOS Experimental CLI environment. This project represents an evolution in personal cryptographic tools, focusing on modularity and hardened key derivation while maintaining a distinct "kawaii" aesthetic.

# ❈ Key Features
  * 320-Bit Sponge Engine: Utilizes an internal state of 320 bits (5 \times 64-bit words) for its permutation rounds.

 * Hardened KDF: Features a Key Derivation Function with 1024 permutation iterations and a memory pool to increase resistance against basic brute-force attempts.

 * Salt & Nonce Binding: Every file is bound to a unique 16-byte salt and a 12-byte nonce, ensuring unique ciphertexts even for identical keys and plaintexts.

 * Atomic Temp-Swap: Implements a safety mechanism using temporary files (.tmp) to ensure data integrity and prevent corruption during the encryption/decryption process.

 * Kawaii UI Architecture: A specialized interface built for Termux and VOS, featuring a pastel color palette (lavender, mint, pink) and custom mascot banners.
❈ Technical Specifications

| Component | Specification |
|---|---|
| Algorithm | Custom Sponge-based Permutation |
| State Size | 320-bit (5 \times 64-bit words) |
| Authentication | 32-byte Message Authentication Tag |
| Build Version | Versa-Hardened v2.2 |
| Language | C++17 |
| Hardware Target | Optimized for MTK G35 / 4GB RAM |
❈ Project Structure
The codebase is organized into modular components for easier maintenance and integration:
 * main.cpp: Orchestrates file I/O, recursive directory processing, and the command loop.
 * etc_engine.hpp / .cpp: The cryptographic core containing the permutation, KDF, and sponge logic.
 * ui_helper.hpp / .cpp: Handles the visual identity, including ANSI color processing, banners, and status bars.

#Install
```bash
git clone https://github.com/VersaNexusIX/ETC-E-II
cd ETC-E-II
g++ -std=c++17 main.cpp etc_engine.cpp ui_helper.cpp -o etc_tool
```

❈ Security Disclaimer
> [!IMPORTANT]
> ETC-E-II is a personal hobby project and an experimental implementation of cryptographic principles by VersaNexusIX. While it follows established sponge construction patterns, it has not undergone professional third-party auditing or rigorous cryptanalysis. It is intended for educational purposes or for securing non-critical data. Use at your own risk.
> 
