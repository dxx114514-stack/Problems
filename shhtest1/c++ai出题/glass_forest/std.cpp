#include <bits/stdc++.h>
using namespace std;

struct Info {
    int lc = 0, rc = 0, cnt = 0;
    bool empty = true;
};

inline Info merge_info(const Info &a, const Info &b) {
    if (a.empty) return b;
    if (b.empty) return a;
    Info res;
    res.empty = false;
    res.lc = a.lc;
    res.rc = b.rc;
    res.cnt = a.cnt + b.cnt - (a.rc == b.lc);
    return res;
}

inline Info reverse_info(Info x) {
    if (!x.empty) swap(x.lc, x.rc);
    return x;
}

struct SegmentTree {
    int n;
    int size;
    vector<Info> tr;

    explicit SegmentTree(const vector<int> &a) {
        n = int(a.size()) - 1;
        size = 1;
        while (size < n) size <<= 1;
        tr.assign(size * 2, {});
        for (int i = 1; i <= n; ++i) {
            tr[size + i - 1] = {a[i], a[i], 1, false};
        }
        for (int i = size - 1; i >= 1; --i) {
            tr[i] = merge_info(tr[i << 1], tr[i << 1 | 1]);
        }
    }

    void update(int x, int c) {
        int p = size + x - 1;
        tr[p] = {c, c, 1, false};
        for (p >>= 1; p; p >>= 1) {
            tr[p] = merge_info(tr[p << 1], tr[p << 1 | 1]);
        }
    }

    Info query(int l, int r) {
        Info left, right;
        l += size - 1;
        r += size - 1;
        while (l <= r) {
            if (l & 1) left = merge_info(left, tr[l++]);
            if (!(r & 1)) right = merge_info(tr[r--], right);
            l >>= 1;
            r >>= 1;
        }
        return merge_info(left, right);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    vector<int> color(n + 1);
    for (int i = 1; i <= n; ++i) cin >> color[i];

    vector<vector<int>> g(n + 1);
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    vector<int> parent(n + 1), depth(n + 1), order;
    order.reserve(n);
    order.push_back(1);
    parent[1] = 0;
    depth[1] = 1;
    for (int i = 0; i < (int)order.size(); ++i) {
        int u = order[i];
        for (int v : g[u]) {
            if (v == parent[u]) continue;
            parent[v] = u;
            depth[v] = depth[u] + 1;
            order.push_back(v);
        }
    }

    vector<int> sz(n + 1, 1), heavy(n + 1, 0);
    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        sz[u] = 1;
        for (int v : g[u]) {
            if (parent[v] != u) continue;
            sz[u] += sz[v];
            if (sz[v] > sz[heavy[u]]) heavy[u] = v;
        }
    }

    vector<int> top(n + 1), pos(n + 1), rev(n + 1);
    int timer = 0;
    vector<pair<int, int>> st;
    st.push_back({1, 1});
    while (!st.empty()) {
        auto [start, tp] = st.back();
        st.pop_back();
        for (int u = start; u; u = heavy[u]) {
            top[u] = tp;
            pos[u] = ++timer;
            rev[timer] = u;
            for (int v : g[u]) {
                if (parent[v] == u && v != heavy[u]) {
                    st.push_back({v, v});
                }
            }
        }
    }

    vector<int> base(n + 1);
    for (int i = 1; i <= n; ++i) base[pos[i]] = color[i];
    SegmentTree seg(base);

    auto query_path = [&](int u, int v) {
        Info left, right;
        while (top[u] != top[v]) {
            if (depth[top[u]] >= depth[top[v]]) {
                Info cur = seg.query(pos[top[u]], pos[u]);
                left = merge_info(left, reverse_info(cur));
                u = parent[top[u]];
            } else {
                Info cur = seg.query(pos[top[v]], pos[v]);
                right = merge_info(cur, right);
                v = parent[top[v]];
            }
        }

        if (depth[u] >= depth[v]) {
            Info cur = seg.query(pos[v], pos[u]);
            left = merge_info(left, reverse_info(cur));
        } else {
            Info cur = seg.query(pos[u], pos[v]);
            left = merge_info(left, cur);
        }

        return merge_info(left, right);
    };

    while (q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int x, c;
            cin >> x >> c;
            seg.update(pos[x], c);
        } else {
            int u, v;
            cin >> u >> v;
            cout << query_path(u, v).cnt << '\n';
        }
    }

    return 0;
}
