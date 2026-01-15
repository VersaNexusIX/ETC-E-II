# 🎨 DESIGN.md: ETC-E-II System Architecture

This document outlines the internal design specifications, cryptographic mechanisms, and interface philosophy of the **ETC-E-II (Enhanced Triple-C Encryption II)**.

## 1. Design Philosophy
ETC-E-II is built on the principle of **"Utility meets Aesthetic."** The goal is to create a functional file encryption tool for personal use in CLI environments (specifically VOS/Termux) with a unique visual identity that departs from traditional "matrix-style" security tools.

## 2. Cryptographic Architecture (Sponge Construction)
The system utilizes a custom **Sponge Construction** with a $320$-bit internal state.

### A. State Configuration
The state consists of five $64$-bit unsigned integers ($x_0$ to $x_4$):
* **Total State ($b$):** $320$ bits.
* **Rate ($r$):** $64$ bits (used for data absorption/squeezing).
* **Capacity ($c$):** $256$ bits (providing a theoretical security margin).



### B. Permutation Function ($p$)
The core permutation function operates through a series of rounds ($10$ or $12$ iterations depending on the phase):
1.  **Round Constants:** Adds a unique variation to each iteration to prevent slide attacks.
2.  **Linear Layer:** Employs XOR operations and Circular Rotation (`ROR64`) for bit diffusion.
3.  **Non-linear Layer:** Uses bitwise AND, NOT, and XOR operations (reminiscent of the Chi layer in Keccak) to create cryptographic confusion.

## 3. Hardened Key Derivation (KDF)
Instead of using the raw password, ETC-E-II processes the key through a **Hardened KDF**:
* **Memory Pool:** Initializes a $1024$-element array of `uint64_t` scrambled based on the password and a $16$-byte salt.
* **Sponge Iteration:** Performs heavy permutation cycles to ensure the initial state is strictly dependent on every bit of the input password.
* **Objective:** To increase the computational cost for offline brute-force attacks during the initialization phase.

## 4. Binary File Structure (`.etc`)
Each encrypted file follows a specific binary layout:

| Offset | Size | Description |
| :--- | :--- | :--- |
| 0x00 | 4 Bytes | Magic Header (`ETCE`) |
| 0x04 | 1 Byte | Metadata Version (0x95) |
| 0x05 | 16 Bytes | Salt (Unique random per file) |
| 0x15 | 12 Bytes | Nonce (Unique random per file) |
| 0x21 | 32 Bytes | Authentication Tag (MAC) |
| 0x41 | Variable | Encrypted Metadata (Original Filename) |
| Variable | Variable | Encrypted Payload (File Data) |

## 5. Metadata Privacy
ETC-E-II encrypts not only the file content but also the **original filename**. This is stored in a separate header block using a dedicated domain separator (`D_HDR`), preventing sensitive information leakage about the file type before a correct key is provided.

## 6. Integrity and Authentication (AEAD)
The system mimics **Authenticated Encryption with Associated Data (AEAD)** behavior:
* During decryption, the engine calculates a **Verification Tag**.
* If the key is incorrect or even a single bit of the data has been tampered with, the tags will not match.
* The system will immediately abort and refuse to output the file, protecting against chosen-ciphertext attacks.

## 7. Interface Design (UI/UX)
The interface uses 8-bit ANSI color codes to maintain a "kawaii" pastel aesthetic:
* **Lavender (#af87ff):** Navigation elements and frames.
* **Mint (#afffd7):** Success indicators and system status.
* **Hot Pink (#ff5faf):** Emphasis, headers, and mascot art.

## 8. Security Disclaimer
* **Experimental Status:** This design is a result of self-study in sponge-based structures and is not intended to replace industry standards like AES-GCM or ChaCha20-Poly1305 for critical infrastructure.
* **Cryptanalysis:** The custom permutation algorithm has not undergone formal third-party differential or linear cryptanalysis.
