#include <bits/stdc++.h>
using namespace std;

// 生成 tree_coloring 的测试数据
// 随机生成一棵树
int main(int argc, char* argv[]) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    int t = atoi(argv[1]);
    int n, c, d;
    if (t <= 3) { n = rng() % 5 + 2; c = rng() % 3 + 1; d = rng() % 5; }
    else if (t <= 6) { n = rng() % 10 + 5; c = rng() % 4 + 1; d = rng() % 10; }
    else { n = rng() % 20 + 10; c = rng() % 5 + 1; d = rng() % 20; }
    cout << n << " " << c << " " << d << endl;
    for (int i = 1; i <= n; i++)
        cout << (rng() % 100 + 1) << (i < n ? " " : "\n");
    for (int i = 2; i <= n; i++)
        cout << (rng() % (i-1) + 1) << " " << i << "\n";
    return 0;
}
