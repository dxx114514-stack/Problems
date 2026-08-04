#include <bits/stdc++.h>
using namespace std;

// 生成 graph_diameter 的测试数据
// 随机生成连通图（随机树 + 额外边）
int main(int argc, char* argv[]) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    int t = atoi(argv[1]);
    int n, m;
    if (t <= 3) { n = rng() % 4 + 2; m = n; }
    else if (t <= 6) { n = rng() % 6 + 4; m = n + rng() % 3; }
    else { n = rng() % 10 + 6; m = n + rng() % 5; }
    cout << n << " " << m << endl;
    // 生成一棵树保证连通
    for (int i = 2; i <= n; i++) {
        int p = rng() % (i-1) + 1;
        int w = rng() % 10 + 1;
        cout << p << " " << i << " " << w << "\n";
    }
    // 额外边
    set<pair<int,int>> edges;
    for (int i = 2; i <= n; i++) {
        int p = i > 1 ? (rng() % (i-1) + 1) : 1;
        edges.insert({min(p,i), max(p,i)});
    }
    for (int i = n; i < m; i++) {
        int u = rng() % n + 1, v = rng() % n + 1;
        while (u == v) v = rng() % n + 1;
        if (u > v) swap(u, v);
        if (edges.count({u,v})) { i--; continue; }
        edges.insert({u,v});
        cout << u << " " << v << " " << (rng() % 10 + 1) << "\n";
    }
    return 0;
}
