#ifndef ETC_ENGINE_HPP
#define ETC_ENGINE_HPP

#include <string>
#include <cstdint>
#include <vector>

#define ROR64(x, n) (((x) >> ((n) & 63)) | ((x) << ((64 - ((n) & 63)) & 63)))

class ETCEngine {
private:
    static constexpr int STATE_B = 40;
    uint64_t x[5], m_pool[1024];
    enum Domain { D_KDF = 0xA1B2C3D4, D_HDR = 0xE1F2A3B4, D_DAT = 0x55667788, D_TAG = 0xDEADBEEF };

    static inline uint64_t load64(const uint8_t* p);
    static inline void store64(uint8_t* p, uint64_t v);
    void p(int r);
    void kdf(const std::string& pw, const uint8_t* salt);

public:
    ETCEngine(const std::string& pw, const uint8_t* salt);
    ~ETCEngine();
    void absorb_nonce(const uint8_t* n);
    void process(uint8_t* d, size_t l, bool enc, bool hdr);
    void finalize(uint8_t* t, const uint8_t* s, const uint8_t* n, uint64_t tl);
};

#endif
