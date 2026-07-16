#include "huffman.h"
#include "bitio.h"
#include <cstring>
#include <cstdio>

struct NodeCmp {
    bool operator()(const HuffNode* a, const HuffNode* b) const {
        return a->freq > b->freq;
    }
};

size_t HuffmanCoder::countFrequencies(FILE* in, uint64_t freq[256]) {
    memset(freq, 0, 256 * sizeof(uint64_t));
    size_t total = 0;
    int ch;
    while ((ch = fgetc(in)) != EOF) {
        freq[ch]++;
        total++;
    }
    rewind(in);
    return total;
}

HuffNode* HuffmanCoder::buildTree(const uint64_t freq[256]) {
    std::priority_queue<HuffNode*, std::vector<HuffNode*>, NodeCmp> heap;
    for (int i = 0; i < 256; ++i)
        if (freq[i] > 0)
            heap.push(new HuffNode(i, freq[i]));
    if (heap.empty())
        return nullptr;
    if (heap.size() == 1) {
        HuffNode* single = heap.top(); heap.pop();
        HuffNode* parent = new HuffNode(-1, single->freq);
        parent->left = single;
        parent->right = new HuffNode(-1, 0);
        heap.push(parent);
    }
    while (heap.size() > 1) {
        HuffNode* left = heap.top(); heap.pop();
        HuffNode* right = heap.top(); heap.pop();
        HuffNode* parent = new HuffNode(-1, left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        heap.push(parent);
    }
    return heap.top();
}

void HuffmanCoder::deleteTree(HuffNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void HuffmanCoder::generateCodes(HuffNode* root, const std::string& code,
    std::string codes[256]) {
    if (!root) return;
    if (root->byte >= 0) {
        codes[root->byte] = code;
        return;
    }
    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

void HuffmanCoder::writeHeader(FILE* out, uint64_t origSize,
    const uint64_t freq[256]) {
    uint32_t magic = 0x46465548;   // "HUFF" 小端
    fwrite(&magic, 4, 1, out);
    fwrite(&origSize, 8, 1, out);
    uint16_t count = 0;
    for (int i = 0; i < 256; ++i)
        if (freq[i] > 0) count++;
    fwrite(&count, 2, 1, out);
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            uint8_t byteVal = static_cast<uint8_t>(i);
            fwrite(&byteVal, 1, 1, out);
            fwrite(&freq[i], 8, 1, out);
        }
    }
}

bool HuffmanCoder::readHeader(FILE* in, uint64_t& origSize,
    uint64_t freq[256]) {
    uint32_t magic;
    if (fread(&magic, 4, 1, in) != 1) return false;
    if (magic != 0x46465548) return false;
    if (fread(&origSize, 8, 1, in) != 1) return false;
    memset(freq, 0, 256 * sizeof(uint64_t));
    uint16_t count;
    if (fread(&count, 2, 1, in) != 1) return false;
    for (uint16_t i = 0; i < count; ++i) {
        uint8_t byteVal;
        if (fread(&byteVal, 1, 1, in) != 1) return false;
        if (fread(&freq[byteVal], 8, 1, in) != 1) return false;
    }
    return true;
}

void HuffmanCoder::writeEncodedData(FILE* in, FILE* out,
    const std::string codes[256]) {
    BitWriter bw(out);
    int ch;
    while ((ch = fgetc(in)) != EOF) {
        const std::string& code = codes[ch];
        for (char c : code)
            bw.write_bit(c == '1' ? 1 : 0);
    }
    bw.flush();
}

void HuffmanCoder::decodeData(FILE* in, FILE* out, HuffNode* root,
    uint64_t origSize) {
    if (origSize == 0) return;
    if (!root) return;
    BitReader br(in);
    uint64_t written = 0;
    HuffNode* node = root;
    while (written < origSize) {
        int bit = br.read_bit();
        if (bit < 0) break;
        node = (bit == 0) ? node->left : node->right;
        if (node->byte >= 0) {
            fputc(node->byte, out);
            written++;
            node = root;
        }
    }
}

bool HuffmanCoder::compress(FILE* in, FILE* out) {
    uint64_t freq[256] = { 0 };
    size_t origSize = countFrequencies(in, freq);
    writeHeader(out, origSize, freq);
    HuffNode* root = buildTree(freq);
    if (origSize > 0 && !root) return false;

    std::string codes[256];
    if (root)
        generateCodes(root, "", codes);

    rewind(in);
    writeEncodedData(in, out, codes);
    deleteTree(root);
    return true;
}

bool HuffmanCoder::decompress(FILE* in, FILE* out) {
    uint64_t origSize = 0;
    uint64_t freq[256] = { 0 };
    if (!readHeader(in, origSize, freq))
        return false;
    HuffNode* root = buildTree(freq);
    if (origSize > 0 && !root) return false;
    decodeData(in, out, root, origSize);
    deleteTree(root);
    return true;
}