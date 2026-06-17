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

    long long N;
    int m, A, total_limit;
    if (case_id <= 3) {
        N = rnd.next_int(1, 12);
        m = rnd.next_int(1, 8);
        A = rnd.next_int(1, 3);
        total_limit = 12;
    } else if (case_id <= 5) {
        N = rnd.next_ll(100000, 1000000000000LL);
        m = rnd.next_int(1, 20);
        A = 1;
        total_limit = 80;
    } else if (case_id <= 8) {
        N = rnd.next_int(1000, 100000);
        m = rnd.next_int(20, 50);
        A = rnd.next_int(2, 10);
        total_limit = 80;
    } else if (case_id <= 12) {
        N = rnd.next_ll(1000000000LL, 1000000000000LL);
        m = rnd.next_int(10, 35);
        A = rnd.next_int(2, 8);
        total_limit = 40;
    } else {
        N = rnd.next_ll(100000000000LL, 1000000000000LL);
        m = 50;
        A = 10;
        total_limit = 80;
    }

    vector<string> patterns;
    int used = 0;
    for (int i = 0; i < m; ++i) {
        int left_patterns = m - i;
        int max_len = max(1, total_limit - used - (left_patterns - 1));
        int len = rnd.next_int(1, max_len);
        used += len;
        string s;
        for (int j = 0; j < len; ++j) {
            s.push_back(char('a' + rnd.next_int(0, A - 1)));
        }
        patterns.push_back(s);
    }

    cout << N << ' ' << m << ' ' << A << '\n';
    for (const string &s : patterns) {
        int w = rnd.next_int(-1000, 1000);
        cout << s << ' ' << w << '\n';
    }

    return 0;
}

