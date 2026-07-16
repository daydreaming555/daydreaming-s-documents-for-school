#pragma once
#include <cstdint>
#include <string>
#include <queue>
#include <vector>

struct HuffNode {
    int byte;
    uint64_t freq;
    HuffNode* left, * right;
    HuffNode(int b, uint64_t f) : byte(b), freq(f), left(nullptr), right(nullptr) {}
};

class HuffmanCoder {
public:
    bool compress(FILE* in, FILE* out);
    bool decompress(FILE* in, FILE* out);

private:
    size_t countFrequencies(FILE* in, uint64_t freq[256]);
    HuffNode* buildTree(const uint64_t freq[256]);
    void deleteTree(HuffNode* root);
    void generateCodes(HuffNode* root, const std::string& code, std::string codes[256]);
    void writeHeader(FILE* out, uint64_t origSize, const uint64_t freq[256]);
    bool readHeader(FILE* in, uint64_t& origSize, uint64_t freq[256]);
    void writeEncodedData(FILE* in, FILE* out, const std::string codes[256]);
    void decodeData(FILE* in, FILE* out, HuffNode* root, uint64_t origSize);
};