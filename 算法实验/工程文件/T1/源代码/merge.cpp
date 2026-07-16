#include "merge.h"
#include <queue>

std::vector<int> mergeTwo(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> res;
    res.reserve(a.size() + b.size());
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        if (a[i] <= b[j]) res.push_back(a[i++]);
        else res.push_back(b[j++]);
    }
    while (i < a.size()) res.push_back(a[i++]);
    while (j < b.size()) res.push_back(b[j++]);
    return res;
}

std::vector<int> mergeKDivide(const std::vector<std::vector<int>>& arrays, int left, int right) {
    if (left == right) return arrays[left];
    int mid = left + (right - left) / 2;
    auto leftPart = mergeKDivide(arrays, left, mid);
    auto rightPart = mergeKDivide(arrays, mid + 1, right);
    return mergeTwo(leftPart, rightPart);
}

std::vector<int> mergeKHeap(const std::vector<std::vector<int>>& arrays) {
    using Node = std::pair<int, int>; // value, array index
    auto comp = [](const Node& a, const Node& b) { return a.first > b.first; };
    std::priority_queue<Node, std::vector<Node>, decltype(comp)> pq(comp);

    std::vector<size_t> indices(arrays.size(), 0);
    for (size_t i = 0; i < arrays.size(); ++i) {
        if (!arrays[i].empty()) {
            pq.push({ arrays[i][0], (int)i });
            indices[i] = 1;
        }
    }

    size_t total = 0;
    for (const auto& arr : arrays) total += arr.size();
    std::vector<int> result;
    result.reserve(total);

    while (!pq.empty()) {
        Node top = pq.top();
        int val = top.first;
        int idx = top.second;
        pq.pop();
        result.push_back(val);
        if (indices[idx] < arrays[idx].size()) {
            pq.push({ arrays[idx][indices[idx]], idx });
            indices[idx]++;
        }
    }
    return result;
}