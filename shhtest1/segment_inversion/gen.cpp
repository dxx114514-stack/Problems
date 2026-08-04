#include <bits/stdc++.h>
using namespace std;

// 生成 segment_inversion 的测试数据
// 随机生成 n 和 k，输出 n k
int main(int argc, char* argv[]) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    int t = atoi(argv[1]); // 测试点编号
    int n, k;
    if (t <= 3) { n = rng() % 5 + 1; k = rng() % min(n/2+1, 4); }
    else if (t <= 6) { n = rng() % 10 + 5; k = rng() % 5; }
    else { n = rng() % 20 + 10; k = rng() % 8; }
    cout << n << " " << k << endl;
    return 0;
}
