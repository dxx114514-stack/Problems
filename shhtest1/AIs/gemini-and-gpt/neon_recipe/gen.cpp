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

    int n;
    string mode = "random";
    if (case_id <= 3) {
        n = rnd.next_int(2, 500);
        mode = "small";
    } else if (case_id <= 6) {
        n = rnd.next_int(10000, 50000);
        mode = "full_reach";
    } else if (case_id <= 10) {
        n = rnd.next_int(5000, 10000);
        mode = "medium";
    } else if (case_id <= 13) {
        n = rnd.next_int(20000, 50000);
        mode = "short";
    } else {
        n = 50000;
        mode = "random";
    }

    vector<int> x(n + 1), c(n + 1), b(n + 1), r(n + 1);
    int cur_x = rnd.next_int(1, 10);
    for (int i = 1; i <= n; ++i) {
        if (i > 1) cur_x += rnd.next_int(1, max(1, 1000000 / n));
        x[i] = cur_x;
        c[i] = rnd.next_int(0, 1000000000);
        b[i] = rnd.next_int(0, 1000000000);
    }
    c[1] = 0;

    for (int i = 1; i <= n; ++i) {
        if (i == n) {
            r[i] = n;
        } else if (mode == "full_reach") {
            r[i] = n;
        } else if (mode == "short") {
            r[i] = min(n, i + rnd.next_int(1, 50));
        } else if (mode == "small") {
            r[i] = rnd.next_int(i + 1, n);
        } else {
            int low = i + 1;
            int high = min(n, i + rnd.next_int(1, max(2, n / 5)));
            r[i] = rnd.next_int(low, high);
        }
    }

    cout << n << '\n';
    for (int i = 1; i <= n; ++i) {
        cout << x[i] << ' ' << c[i] << ' ' << b[i] << ' ' << r[i] << '\n';
    }

    return 0;
}

