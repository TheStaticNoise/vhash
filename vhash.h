#ifndef V_HASH_H
#define V_HASH_H
#include <stdint.h>
#define VHASH_INTESITY 10
static inline uint64_t v_hash(const char* rd, uint64_t seed) {
    const char* s = rd;
    uint8_t pass = 0, n;
    unsigned int c = 0;
    uint64_t hash = 0, temp, i = 0;
    seed &= 63; //modulo
    if ((seed & (seed - 1)) == 0 && seed < 61) seed += 3;

    while (pass != 3) {
        c = *s++;
        if (c == 0) {
            s = rd;
            pass++;
            continue;
        }
        uint32_t temp_c = c;
        uint64_t temp_i = i;
        c++;
        i++;
        hash += ((((((temp_c >> 1) & 0xFB83FFAF) + ((temp_i << 2) / 3)) << temp_i) >> 1) + 4323) >> 1;
        hash |= hash << 2 ^ (hash & 0x000000A6);
        hash ^= i & 63;
        temp = hash;
        hash ^= (temp >> 6) ^ (temp << 4);
        temp = hash;
        hash ^= (temp >> 3) ^ (temp << 6);
        temp = hash;
        hash ^= (temp >> 3) ^ (temp << 4);
        hash ^= c << 5;
        temp = hash;
        hash <<= temp & 0x0000000F;
        temp = hash;
        hash ^= temp << 5;
        hash ^= i << 7;
        hash >>= 5;
        hash ^= (hash >> 2) + (c >> 8);
        n = (i & 4) + 5; // modulo of 4 + 5
        temp = hash;
        hash ^= (temp << n) ^ (temp >> (64 - n)); // I'M FUCKING CRAZY
        temp = hash;
        hash ^= (temp >> (64 - n)) ^ (temp << n);
        temp = hash;
        hash ^= (((i + seed) << 5) & 0xFFFFFFFF); // V-hash: the crazy freak amongst civilized hashes
    }
    hash ^= (temp << 29) ^ (temp >> 35);
    temp = hash;
    hash ^= ++i + ((temp & 0x0001BEE8) >> 7);
    temp = seed;
    seed = temp & 63;
    if (seed <= VHASH_INTESITY) {
        return hash;
    }
    for (int a = 0; a < VHASH_INTESITY; a++) {
        temp = hash;
        hash = (temp >> (seed - a)) ^ (hash << (64 - (seed - a)));
    }
    return hash;
}
#endif
