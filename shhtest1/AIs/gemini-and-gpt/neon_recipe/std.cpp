#include <bits/stdc++.h>
using namespace std;

const long long INF = 4000000000000000000LL;

struct Line {
    long long m = 0, b = INF;
    bool empty = true;
};

long long eval_line(const Line &ln, long long x) {
    if (ln.empty) return INF;
    __int128 val = (__int128)ln.m * x + ln.b;
    if (val > INF) return INF;
    if (val < -INF) return -INF;
    return (long long)val;
}

struct LiChao {
    vector<long long> xs;
    vector<Line> tr;

    explicit LiChao(vector<long long> points) : xs(move(points)) {
        tr.assign(xs.size() * 4 + 4, {});
    }

    bool better(const Line &a, const Line &b, long long x) {
        return eval_line(a, x) < eval_line(b, x);
    }

    void add_line(Line nw, int p, int l, int r) {
        if (tr[p].empty) {
            tr[p] = nw;
            tr[p].empty = false;
            return;
        }

        int mid = (l + r) >> 1;
        long long xl = xs[l], xm = xs[mid], xr = xs[r];

        if (better(nw, tr[p], xm)) swap(nw, tr[p]);
        if (l == r) return;

        if (better(nw, tr[p], xl)) {
            add_line(nw, p << 1, l, mid);
        } else if (better(nw, tr[p], xr)) {
            add_line(nw, p << 1 | 1, mid + 1, r);
        }
    }

    void add_line(Line ln) {
        add_line(ln, 1, 0, int(xs.size()) - 1);
    }

    long long query(long long x, int p, int l, int r) {
        long long res = eval_line(tr[p], x);
        if (l == r) return res;
        int mid = (l + r) >> 1;
        if (x <= xs[mid]) res = min(res, query(x, p << 1, l, mid));
        else res = min(res, query(x, p << 1 | 1, mid + 1, r));
        return res;
    }

    long long query(long long x) {
        return query(x, 1, 0, int(xs.size()) - 1);
    }
};

int n;
vector<long long> xcoord, enter_cost, use_cost, dp;
vector<int> reach_to;

Line make_line(int i) {
    Line ln;
    ln.empty = false;
    ln.m = -2LL * xcoord[i];
    __int128 val = (__int128)dp[i] + use_cost[i] + (__int128)xcoord[i] * xcoord[i];
    ln.b = val > INF ? INF : (long long)val;
    return ln;
}

void cdq(int l, int r) {
    if (l == r) return;

    int mid = (l + r) >> 1;
    cdq(l, mid);

    int left_bound = mid + 1;
    int len = r - mid;
    vector<vector<Line>> bucket(len * 4 + 4);

    auto add_interval = [&](auto &&self, int p, int L, int R, int ql, int qr, const Line &ln) -> void {
        if (ql <= L && R <= qr) {
            bucket[p].push_back(ln);
            return;
        }
        int M = (L + R) >> 1;
        if (ql <= M) self(self, p << 1, L, M, ql, qr, ln);
        if (M < qr) self(self, p << 1 | 1, M + 1, R, ql, qr, ln);
    };

    for (int i = l; i <= mid; ++i) {
        if (dp[i] >= INF || reach_to[i] < left_bound) continue;
        int ql = left_bound;
        int qr = min(r, reach_to[i]);
        if (ql <= qr) add_interval(add_interval, 1, left_bound, r, ql, qr, make_line(i));
    }

    auto process = [&](auto &&self, int p, int L, int R) -> void {
        if (!bucket[p].empty()) {
            vector<long long> xs;
            xs.reserve(R - L + 1);
            for (int i = L; i <= R; ++i) xs.push_back(xcoord[i]);

            LiChao tree(move(xs));
            for (const Line &ln : bucket[p]) tree.add_line(ln);

            for (int i = L; i <= R; ++i) {
                long long best = tree.query(xcoord[i]);
                if (best >= INF) continue;
                __int128 cand = (__int128)best + (__int128)xcoord[i] * xcoord[i] + enter_cost[i];
                if (cand < dp[i]) dp[i] = cand > INF ? INF : (long long)cand;
            }
        }

        if (L == R) return;
        int M = (L + R) >> 1;
        self(self, p << 1, L, M);
        self(self, p << 1 | 1, M + 1, R);
    };

    process(process, 1, left_bound, r);
    cdq(mid + 1, r);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n)) return 0;
    xcoord.assign(n + 1, 0);
    enter_cost.assign(n + 1, 0);
    use_cost.assign(n + 1, 0);
    reach_to.assign(n + 1, 0);
    dp.assign(n + 1, INF);

    for (int i = 1; i <= n; ++i) {
        cin >> xcoord[i] >> enter_cost[i] >> use_cost[i] >> reach_to[i];
    }

    dp[1] = 0;
    cdq(1, n);

    cout << (dp[n] >= INF ? -1 : dp[n]) << '\n';
    return 0;
}
