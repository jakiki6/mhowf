#include <stdint.h>
#include <string.h>
#include <endian.h>

#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d) (             \
    a += b, d ^= a, d = ROTL(d, 16), \
    c += d, b ^= c, b = ROTL(b, 12), \
    a += b, d ^= a, d = ROTL(d,  8), \
    c += d, b ^= c, b = ROTL(b,  7))

static void chacha_block(uint32_t out[16], uint32_t const in[16], uint64_t rounds)
{
    int i;
    uint32_t x[16];

    for (i = 0; i < 16; ++i)
        x[i] = le32toh(in[i]);
    for (i = 0; i < rounds; i += 2) {
        // Odd round
        QR(x[0], x[4], x[ 8], x[12]); // column 1
        QR(x[1], x[5], x[ 9], x[13]); // column 2
        QR(x[2], x[6], x[10], x[14]); // column 3
        QR(x[3], x[7], x[11], x[15]); // column 4
        // Even round
        QR(x[0], x[5], x[10], x[15]); // diagonal 1 (main diagonal)
        QR(x[1], x[6], x[11], x[12]); // diagonal 2
        QR(x[2], x[7], x[ 8], x[13]); // diagonal 3
        QR(x[3], x[4], x[ 9], x[14]); // diagonal 4
    }

    x[0] ^= 1;

    for (i = 0; i < 16; ++i)
        out[i] = htole32(x[i] + in[i]);
}

void mhowf(uint8_t output[64], uint8_t input[64], uint8_t *buffer, uint64_t memory, uint64_t hardness) {
    memset(buffer, 0, memory);
    memcpy(buffer, input, 64);

    for (uint64_t i = 0; i < memory - 64; i += 32) {
        chacha_block((uint32_t *) (buffer + i), (uint32_t *) (buffer + i), hardness);
    }

    for (uint64_t i = memory - 64; i < 0x7fffffffffffffff; i -= 32) {
        chacha_block((uint32_t *) (buffer + i), (uint32_t *) (buffer + i), hardness);
    }

    memcpy(output, buffer, 64);
}
