#include "etc_engine.hpp"
#include <cstring>

inline uint64_t ETCEngine::load64(const uint8_t* p) {
    return (uint64_t)p[0] | (uint64_t)p[1] << 8 | (uint64_t)p[2] << 16 | (uint64_t)p[3] << 24 |
           (uint64_t)p[4] << 32 | (uint64_t)p[5] << 40 | (uint64_t)p[6] << 48 | (uint64_t)p[7] << 56;
}

inline void ETCEngine::store64(uint8_t* p, uint64_t v) {
    p[0] = v & 0xff; p[1] = (v >> 8) & 0xff; p[2] = (v >> 16) & 0xff; p[3] = (v >> 24) & 0xff;
    p[4] = (v >> 32) & 0xff; p[5] = (v >> 40) & 0xff; p[6] = (v >> 48) & 0xff; p[7] = (v >> 56) & 0xff;
}

void ETCEngine::p(int r) {
    for (int i = 0; i < r; i++) {
        x[2] ^= (uint64_t)(0xf0 - i);
        x[0] ^= x[4]; x[4] ^= x[3]; x[2] ^= x[1];
        uint64_t t[5];
        for (int j = 0; j < 5; j++) t[j] = (~x[j]) & x[(j + 1) % 5];
        for (int j = 0; j < 5; j++) x[j] ^= t[(j + 1) % 5];
        x[1] ^= x[0]; x[0] ^= x[4]; x[3] ^= x[2]; x[2] = ~x[2];
        x[0] ^= ROR64(x[0], 19) ^ ROR64(x[0], 28); x[1] ^= ROR64(x[1], 61) ^ ROR64(x[1], 39);
        x[2] ^= ROR64(x[2], 1)  ^ ROR64(x[2], 6); x[3] ^= ROR64(x[3], 10) ^ ROR64(x[3], 17);
        x[4] ^= ROR64(x[4], 7)  ^ ROR64(x[4], 41);
    }
}

void ETCEngine::kdf(const std::string& pw, const uint8_t* salt) {
    memset(x, 0, sizeof(x));
    for (size_t i = 0; i < pw.size(); i++) ((uint8_t*)x)[i % STATE_B] ^= pw[i];
    for (int i = 0; i < 16; i++) ((uint8_t*)x)[(i + 24) % STATE_B] ^= salt[i];
    x[4] ^= D_KDF;
    for (int i = 0; i < 1024; i++) { p(2); m_pool[i] = x[0] ^ x[2] ^ x[4]; }
    for (int i = 0; i < 1024; i++) { x[1] ^= m_pool[(x[0] ^ i) & 1023]; p(2); }
}

ETCEngine::ETCEngine(const std::string& pw, const uint8_t* salt) { kdf(pw, salt); }

ETCEngine::~ETCEngine() { memset(x, 0, sizeof(x)); memset(m_pool, 0, sizeof(m_pool)); }

void ETCEngine::absorb_nonce(const uint8_t* n) { for (int i = 0; i < 12; i++) ((uint8_t*)x)[i] ^= n[i]; p(12); }

void ETCEngine::process(uint8_t* d, size_t l, bool enc, bool hdr) {
    x[4] ^= hdr ? (uint64_t)D_HDR : (uint64_t)D_DAT;
    int r = hdr ? 12 : 10; p(r);
    size_t blks = l / 8;
    for (size_t i = 0; i < blks; i++) {
        uint64_t m = load64(d + (i * 8));
        if (enc) { x[0] ^= m; m = x[0]; } else { uint64_t c = m; m ^= x[0]; x[0] = c; }
        store64(d + (i * 8), m); p(r);
    }
    size_t rem = l % 8;
    if (rem) {
        size_t off = blks * 8;
        for (size_t j = 0; j < rem; j++) {
            if (enc) { ((uint8_t*)x)[j] ^= d[off + j]; d[off + j] = ((uint8_t*)x)[j]; }
            else { uint8_t c = d[off + j]; d[off + j] ^= ((uint8_t*)x)[j]; ((uint8_t*)x)[j] = c; }
        }
        p(r);
    }
}

void ETCEngine::finalize(uint8_t* t, const uint8_t* s, const uint8_t* n, uint64_t tl) {
    x[4] ^= D_TAG; x[3] ^= tl;
    x[0] ^= load64(s); x[1] ^= load64(s + 8); x[2] ^= load64(n);
    x[2] ^= (uint64_t)n[8] << 32 | (uint64_t)n[9] << 40 | (uint64_t)n[10] << 48 | (uint64_t)n[11] << 56;
    p(12); store64(t, x[0]); store64(t+8, x[1]); store64(t+16, x[2]); store64(t+24, x[3]);
}
