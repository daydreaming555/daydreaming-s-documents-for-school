#include "bitio.h"

BitWriter::BitWriter(FILE* f) : fp(f), buffer(0), bits_filled(0) {}

void BitWriter::write_bit(int bit) {
    if (bit)
        buffer |= (1 << bits_filled);
    bits_filled++;
    if (bits_filled == 8) {
        fwrite(&buffer, 1, 1, fp);
        buffer = 0;
        bits_filled = 0;
    }
}

void BitWriter::write_bits(uint64_t value, int nbits) {
    while (nbits--) {
        write_bit(value & 1);
        value >>= 1;
    }
}

void BitWriter::flush() {
    if (bits_filled > 0) {
        fwrite(&buffer, 1, 1, fp);
        buffer = 0;
        bits_filled = 0;
    }
}

BitReader::BitReader(FILE* f) : fp(f), buffer(0), bits_left(0) {}

int BitReader::read_bit() {
    if (bits_left == 0) {
        if (fread(&buffer, 1, 1, fp) != 1)
            return -1;
        bits_left = 8;
    }
    int bit = buffer & 1;
    buffer >>= 1;
    bits_left--;
    return bit;
}

uint64_t BitReader::read_bits(int nbits) {
    uint64_t value = 0;
    for (int i = 0; i < nbits; ++i) {
        int b = read_bit();
        if (b < 0) break;
        if (b)
            value |= (static_cast<uint64_t>(1) << i);
    }
    return value;
}