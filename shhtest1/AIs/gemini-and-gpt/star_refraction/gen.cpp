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

    int n, q;
    bool zero_radius = false;
    bool unit_weight = false;
    long long coord_lim;

    if (case_id <= 3) {
        n = rnd.next_int(1, 2000);
        q = rnd.next_int(1, 2000);
        coord_lim = 1000;
    } else if (case_id <= 5) {
        n = rnd.next_int(50000, 200000);
        q = rnd.next_int(50000, 200000);
        coord_lim = 1000000000LL;
        zero_radius = true;
    } else if (case_id <= 8) {
        n = rnd.next_int(50000, 200000);
        q = rnd.next_int(50000, 200000);
        coord_lim = 1000000000LL;
        unit_weight = true;
    } else if (case_id <= 12) {
        n = rnd.next_int(20000, 50000);
        q = rnd.next_int(20000, 50000);
        coord_lim = 1000000;
    } else {
        n = 200000;
        q = 200000;
        coord_lim = 1000000000LL;
    }

    cout << n << ' ' << q << '\n';
    for (int i = 0; i < n; ++i) {
        long long x = rnd.next_ll(-coord_lim, coord_lim);
        long long y = rnd.next_ll(-coord_lim, coord_lim);
        long long w = unit_weight ? 1 : rnd.next_ll(1, 1000000000LL);
        cout << x << ' ' << y << ' ' << w << '\n';
    }

    for (int i = 0; i < q; ++i) {
        long long a = rnd.next_ll(-coord_lim, coord_lim);
        long long b = rnd.next_ll(-coord_lim, coord_lim);
        long long d = zero_radius ? 0 : rnd.next_ll(0, 2 * coord_lim);
        cout << a << ' ' << b << ' ' << d << '\n';
    }

    return 0;
}

