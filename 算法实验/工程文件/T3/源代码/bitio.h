#pragma once
#include <cstdint>
#include <cstdio>

class BitWriter {
public:
    explicit BitWriter(FILE* f);
    void write_bit(int bit);
    void write_bits(uint64_t value, int nbits);
    void flush();

private:
    FILE* fp;
    uint8_t buffer;
    int bits_filled;
};

class BitReader {
public:
    explicit BitReader(FILE* f);
    int read_bit();
    uint64_t read_bits(int nbits);

private:
    FILE* fp;
    uint8_t buffer;
    int bits_left;
};