#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n;
    vector<long long> bit;

    explicit Fenwick(int n_) : n(n_), bit(n_ + 1, 0) {}

    void add(int idx, long long val) {
        for (; idx <= n; idx += idx & -idx) bit[idx] += val;
    }

    long long sum_prefix(int idx) const {
        long long res = 0;
        for (; idx > 0; idx -= idx & -idx) res += bit[idx];
        return res;
    }

    long long sum_range(int l, int r) const {
        if (l > r) return 0;
        return sum_prefix(r) - sum_prefix(l - 1);
    }
};

struct Point {
    long long u, v, w;
};

struct Event {
    long long limit_u, low_v, high_v;
    int id, coef;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    vector<Point> points(n);
    vector<long long> all_v;
    all_v.reserve(n);

    for (auto &p : points) {
        long long x, y, w;
        cin >> x >> y >> w;
        p.u = x + y;
        p.v = x - y;
        p.w = w;
        all_v.push_back(p.v);
    }

    vector<Event> events;
    events.reserve(q * 2);
    for (int i = 0; i < q; ++i) {
        long long a, b, d;
        cin >> a >> b >> d;
        long long U = a + b;
        long long V = a - b;
        events.push_back({U + d, V - d, V + d, i, 1});
        events.push_back({U - d - 1, V - d, V + d, i, -1});
    }

    sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
        return a.u < b.u;
    });
    sort(events.begin(), events.end(), [](const Event &a, const Event &b) {
        return a.limit_u < b.limit_u;
    });
    sort(all_v.begin(), all_v.end());
    all_v.erase(unique(all_v.begin(), all_v.end()), all_v.end());

    Fenwick fw(int(all_v.size()));
    vector<long long> ans(q, 0);
    int ptr = 0;

    for (const auto &e : events) {
        while (ptr < n && points[ptr].u <= e.limit_u) {
            int pos = int(lower_bound(all_v.begin(), all_v.end(), points[ptr].v) - all_v.begin()) + 1;
            fw.add(pos, points[ptr].w);
            ++ptr;
        }

        int l = int(lower_bound(all_v.begin(), all_v.end(), e.low_v) - all_v.begin()) + 1;
        int r = int(upper_bound(all_v.begin(), all_v.end(), e.high_v) - all_v.begin());
        ans[e.id] += 1LL * e.coef * fw.sum_range(l, r);
    }

    for (long long x : ans) cout << x << '\n';
    return 0;
}
