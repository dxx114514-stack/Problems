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

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "usage: gen <case_id> <seed>\n";
        return 1;
    }

    int case_id = atoi(argv[1]);
    unsigned long long seed = strtoull(argv[2], nullptr, 10);
    RNG rnd(seed);

    int n, q, color_cnt;
    string shape = "random";
    bool readonly = false;

    if (case_id <= 3) {
        n = rnd.next_int(1, 2000);
        q = rnd.next_int(1, 2000);
        color_cnt = rnd.next_int(2, 20);
    } else if (case_id <= 6) {
        n = rnd.next_int(50000, 200000);
        q = rnd.next_int(50000, 200000);
        color_cnt = rnd.next_int(2, 1000000);
        shape = "chain";
    } else if (case_id <= 9) {
        n = rnd.next_int(50000, 200000);
        q = rnd.next_int(50000, 200000);
        color_cnt = rnd.next_int(2, 1000000);
        readonly = true;
    } else if (case_id <= 13) {
        n = rnd.next_int(20000, 50000);
        q = rnd.next_int(20000, 50000);
        color_cnt = rnd.next_int(2, 1000);
        shape = (case_id % 2 ? "star" : "binary");
    } else {
        n = 200000;
        q = 200000;
        color_cnt = 1000000000;
        shape = "random";
    }

    vector<int> color(n + 1);
    for (int i = 1; i <= n; ++i) color[i] = rnd.next_int(1, color_cnt);

    vector<pair<int, int>> edges;
    for (int i = 2; i <= n; ++i) {
        int p;
        if (shape == "chain") p = i - 1;
        else if (shape == "star") p = 1;
        else if (shape == "binary") p = i / 2;
        else p = rnd.next_int(1, i - 1);
        edges.push_back({p, i});
    }

    cout << n << ' ' << q << '\n';
    for (int i = 1; i <= n; ++i) {
        if (i > 1) cout << ' ';
        cout << color[i];
    }
    cout << '\n';

    shuffle(edges.begin(), edges.end(), rnd.rng);
    for (auto [u, v] : edges) cout << u << ' ' << v << '\n';

    for (int i = 0; i < q; ++i) {
        bool do_query = readonly || rnd.next_int(1, 100) <= 65;
        if (do_query) {
            int u = rnd.next_int(1, n);
            int v = rnd.next_int(1, n);
            cout << "2 " << u << ' ' << v << '\n';
        } else {
            int x = rnd.next_int(1, n);
            int c = rnd.next_int(1, color_cnt);
            cout << "1 " << x << ' ' << c << '\n';
        }
    }

    return 0;
}

