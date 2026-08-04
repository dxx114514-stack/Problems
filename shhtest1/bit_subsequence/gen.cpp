#include <bits/stdc++.h>
using namespace std;

// 生成 bit_subsequence 的测试数据
int main(int argc, char* argv[]) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    int t = atoi(argv[1]);
    int n, k;
    if (t <= 3) { n = rng() % 4 + 1; k = rng() % n + 1; }
    else if (t <= 6) { n = rng() % 6 + 3; k = rng() % n + 1; }
    else { n = rng() % 10 + 5; k = rng() % n + 1; }
    cout << n << " " << k << "\n";
    for (int i = 0; i < n; i++)
        cout << (rng() % 2);
    cout << "\n";
    return 0;
}
