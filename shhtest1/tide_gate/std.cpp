#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    long long w;
    int l, r;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, P;
    if (!(cin >> n >> m >> P)) return 0;

    vector<vector<Edge>> g(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v, l, r;
        long long w;
        cin >> u >> v >> w >> l >> r;
        g[u].push_back({v, w, l, r});
    }

    const long long INF = (1LL << 62);
    vector<long long> dist(1LL * (n + 1) * P, INF);

    auto id = [P](int u, int rem) {
        return 1LL * u * P + rem;
    };

    auto wait_time = [P](int rem, int l, int r) {
        if (l <= rem && rem <= r) return 0;
        if (rem < l) return l - rem;
        return P - rem + l;
    };

    using State = tuple<long long, int, int>;
    priority_queue<State, vector<State>, greater<State>> pq;
    dist[id(1, 0)] = 0;
    pq.push({0, 1, 0});

    while (!pq.empty()) {
        auto [d, u, rem] = pq.top();
        pq.pop();
        if (d != dist[id(u, rem)]) continue;

        for (const auto &e : g[u]) {
            long long nd = d + wait_time(rem, e.l, e.r) + e.w;
            int nr = int(nd % P);
            long long idx = id(e.to, nr);
            if (nd < dist[idx]) {
                dist[idx] = nd;
                pq.push({nd, e.to, nr});
            }
        }
    }

    long long ans = INF;
    for (int rem = 0; rem < P; ++rem) {
        ans = min(ans, dist[id(n, rem)]);
    }

    cout << (ans == INF ? -1 : ans) << '\n';
    return 0;
}
