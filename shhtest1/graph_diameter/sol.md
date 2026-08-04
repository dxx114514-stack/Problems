# 图的直径——题解

## 核心思路

**关键观察：** 加倍边权只会增大距离，不会减小。因此最优策略是找到一条边，加倍它对直径的影响最小。

### 分析

1. 找到直径路径（图中最长的最短路径）。
2. 直径路径上的边加倍后，直径会增加。增加量等于该边的原始权值。
3. 直径路径外的边加倍后，不影响直径。

**结论：** 倍加直径路径上**权值最小**的边，直径增加最小。

### 算法

1. 从任意节点出发 Dijkstra，找到最远节点 $a$。
2. 从 $a$ 出发 Dijkstra，找到最远节点 $b$，$a \to b$ 即为直径路径。
3. 沿直径路径找到最小权值的边 $w_{\min}$。
4. 答案为 $\min(d(a,b),\ d(a,b) + w_{\min})$（选择不操作或加倍最小边）。

### 复杂度

$O(m \log n)$

## 标准代码

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll, int> pli;
const int MAXN = 100005;
const ll INF = 1e18;

int n, m;
vector<pair<int, ll>> adj[MAXN];
ll dist[MAXN];
int parent[MAXN];

void dijkstra(int src) {
    for (int i = 1; i <= n; i++) { dist[i] = INF; parent[i] = -1; }
    dist[src] = 0;
    priority_queue<pli, vector<pli>, greater<pli>> pq;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int u, v; ll w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    dijkstra(1);
    int a = 1;
    for (int i = 2; i <= n; i++)
        if (dist[i] > dist[a]) a = i;
    dijkstra(a);
    int b = 1;
    for (int i = 2; i <= n; i++)
        if (dist[i] > dist[b]) b = i;
    ll diameter = dist[b];

    // 回溯直径路径，找最小边权
    vector<int> path;
    for (int cur = b; cur != -1; cur = parent[cur])
        path.push_back(cur);
    reverse(path.begin(), path.end());

    ll minEdge = INF;
    for (int i = 1; i < (int)path.size(); i++) {
        int u = path[i-1], v = path[i];
        for (auto [to, w] : adj[u])
            if (to == v) { minEdge = min(minEdge, w); break; }
    }

    ll ans = diameter;
    if (minEdge < INF)
        ans = min(ans, diameter + minEdge);
    cout << ans << endl;
    return 0;
}
```
