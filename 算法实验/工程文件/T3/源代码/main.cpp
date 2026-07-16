#include "huffman.h"
#include <cstdio>
#include <cstring>
#include <chrono>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    system("chcp 65001 > nul");
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <compress|decompress> <input> <output>\n", argv[0]);
        return 1;
    }

    FILE* in = nullptr;
    FILE* out = nullptr;
    errno_t err;

    err = fopen_s(&in, argv[2], "rb");
    if (err != 0 || !in) {
        fprintf(stderr, "Cannot open input file: %s\n", argv[2]);
        return 1;
    }
    err = fopen_s(&out, argv[3], "wb");
    if (err != 0 || !out) {
        fprintf(stderr, "Cannot open output file: %s\n", argv[3]);
        fclose(in);
        return 1;
    }

    HuffmanCoder coder;
    bool ok = false;
    double ms = 0.0;

    if (strcmp(argv[1], "compress") == 0) {
        auto start = std::chrono::high_resolution_clock::now();
        ok = coder.compress(in, out);
        auto end = std::chrono::high_resolution_clock::now();
        ms = std::chrono::duration<double, std::milli>(end - start).count();
        if (ok) printf("Compression time: %.2f ms\n", ms);
    }
    else if (strcmp(argv[1], "decompress") == 0) {
        auto start = std::chrono::high_resolution_clock::now();
        ok = coder.decompress(in, out);
        auto end = std::chrono::high_resolution_clock::now();
        ms = std::chrono::duration<double, std::milli>(end - start).count();
        if (ok) printf("Decompression time: %.2f ms\n", ms);
    }
    else {
        fprintf(stderr, "Unknown command: %s (use 'compress' or 'decompress')\n", argv[1]);
    }

    fclose(in);
    fclose(out);

    if (!ok) {
        fprintf(stderr, "Operation failed.\n");
        return 1;
    }
    return 0;
}