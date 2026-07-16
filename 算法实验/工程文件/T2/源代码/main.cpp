#include <iostream>
#include <string>
#include <vector>
#include <chrono>
using namespace std;

int minCut(string s) {
    int n = s.size();
    if (n <= 1) return 0;

    // 预处理回文表 isPal[i][j] : s[i..j] 是否回文
    vector<vector<bool>> isPal(n, vector<bool>(n, false));
    // 所有长度为 1 的都是回文
    for (int i = 0; i < n; ++i) isPal[i][i] = true;
    // 长度为 2
    for (int i = 0; i < n - 1; ++i)
        if (s[i] == s[i + 1]) isPal[i][i + 1] = true;
    // 长度 >= 3
    for (int len = 3; len <= n; ++len) {
        for (int i = 0; i + len - 1 < n; ++i) {
            int j = i + len - 1;
            if (s[i] == s[j] && isPal[i + 1][j - 1])
                isPal[i][j] = true;
        }
    }

    // dp[i] 表示前 i 个字符 (s[0..i-1]) 的最少切割次数
    vector<int> dp(n + 1, n);
    dp[0] = -1; // 空串定义为 -1 ，方便转移
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (isPal[j][i - 1]) {
                dp[i] = min(dp[i], dp[j] + 1);
            }
        }
    }
    return dp[n];
}

int main() {
    string s;
    // 读取一整行字符串
    getline(cin, s);
    // 去除可能的结尾换行符
    if (!s.empty() && s.back() == '\r') s.pop_back();

    auto start = chrono::high_resolution_clock::now();
    int cuts = minCut(s);
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, std::milli>(end - start).count();

    cout << cuts << endl;               
    cerr << "Time: " << ms << " ms" << endl;
    return 0;
}