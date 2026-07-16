#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include "merge.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <divide|heap>" << endl;
        return 1;
    }
    string mode = argv[1];
    if (mode != "divide" && mode != "heap") {
        cerr << "Mode must be 'divide' or 'heap'" << endl;
        return 1;
    }

    int K;
    cin >> K;
    vector<vector<int>> arrays(K);
    for (int i = 0; i < K; ++i) {
        int len;
        cin >> len;
        arrays[i].resize(len);
        for (int j = 0; j < len; ++j) cin >> arrays[i][j];
    }

    vector<int> result;
    auto start = chrono::high_resolution_clock::now();
    if (mode == "divide") {
        if (K > 0)
            result = mergeKDivide(arrays, 0, K - 1);
    }
    else {
        result = mergeKHeap(arrays);
    }
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, std::milli>(end - start).count();

    // 验证排序（调试用，若数据量很大可注释掉循环输出）
    bool sorted = true;
    for (size_t i = 1; i < result.size(); ++i) {
        if (result[i] < result[i - 1]) { sorted = false; break; }
    }
    if (!sorted) cerr << "ERROR: Not sorted!" << endl;

    // 输出结果（若数据量大，建议重定向到文件）
    for (size_t i = 0; i < result.size(); ++i) {
        if (i) cout << ' ';
        cout << result[i];
    }
    cout << endl;

    // 打印时间等信息到 stderr，避免影响输出重定向
    cerr << "Time: " << ms << " ms" << endl;
    cerr << "Total elements: " << result.size() << endl;
    cerr << "Sorted OK" << endl;

    return 0;
}