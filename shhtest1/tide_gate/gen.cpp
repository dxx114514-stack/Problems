#include <bits/stdc++.h>
using namespace std;

struct RNG {
    mt19937_64 rng;
    explicit RNG(unsigned long long seed) : rng(seed) {}
    long long next_ll(long long l, long long r) {
        return uniform_int_distribution<long long>(l, r)(rng);
    }
    int next_int(int l, int r) {
        return int(next_ll(l, r));
    }
};

struct Edge {
    int u, v, l, r;
    long long w;
};

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "usage: gen <case_id> <seed>\n";
        return 1;
    }

    int case_id = atoi(argv[1]);
    unsigned long long seed = strtoull(argv[2], nullptr, 10);
    RNG rnd(seed);

    int n, m, P;
    bool dag = false;
    long long max_w = 1000000000LL;

    if (case_id <= 3) {
        n = rnd.next_int(2, 8);
        m = rnd.next_int(n - 1, 20);
        P = rnd.next_int(1, 6);
        max_w = 20;
    } else if (case_id <= 5) {
        n = rnd.next_int(1000, 5000);
        m = rnd.next_int(n - 1, min(20000, n * 8));
        P = 1;
    } else if (case_id <= 8) {
        n = rnd.next_int(20000, 60000);
        m = rnd.next_int(n - 1, 120000);
        P = rnd.next_int(10, 60);
        dag = true;
    } else if (case_id <= 12) {
        n = rnd.next_int(10000, 20000);
        m = rnd.next_int(n - 1, 50000);
        P = rnd.next_int(2, 20);
    } else if (case_id <= 16) {
        n = rnd.next_int(50000, 100000);
        m = rnd.next_int(150000, 200000);
        P = rnd.next_int(30, 60);
    } else {
        n = 100000;
        m = 200000;
        P = 60;
    }

    vector<Edge> edges;
    edges.reserve(m);

    auto add_edge = [&](int u, int v) {
        int l = rnd.next_int(0, P - 1);
        int r = rnd.next_int(l, P - 1);
        long long w = rnd.next_ll(1, max_w);
        edges.push_back({u, v, l, r, w});
    };

    for (int i = 1; i < n && (int)edges.size() < m; ++i) {
        add_edge(i, i + 1);
    }

    while ((int)edges.size() < m) {
        int u = rnd.next_int(1, n);
        int v = rnd.next_int(1, n);
        if (dag) {
            u = rnd.next_int(1, n - 1);
            v = rnd.next_int(u + 1, n);
        } else {
            while (v == u) v = rnd.next_int(1, n);
        }
        add_edge(u, v);
    }

    shuffle(edges.begin(), edges.end(), rnd.rng);

    cout << n << ' ' << m << ' ' << P << '\n';
    for (auto &e : edges) {
        cout << e.u << ' ' << e.v << ' ' << e.w << ' ' << e.l << ' ' << e.r << '\n';
    }
    return 0;
}

