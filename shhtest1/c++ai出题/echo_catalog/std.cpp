#include <bits/stdc++.h>
using namespace std;

const long long NEG = -4000000000000000000LL;

struct Node {
    array<int, 10> nxt{};
    int fail = 0;
    long long val = 0;
    Node() {
        nxt.fill(-1);
    }
};

struct Matrix {
    int n = 0;
    vector<vector<long long>> a;

    Matrix() = default;
    explicit Matrix(int n_, bool ident = false) : n(n_), a(n_, vector<long long>(n_, NEG)) {
        if (ident) {
            for (int i = 0; i < n; ++i) a[i][i] = 0;
        }
    }
};

Matrix multiply(const Matrix &A, const Matrix &B) {
    int n = A.n;
    Matrix C(n);
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            if (A.a[i][k] == NEG) continue;
            for (int j = 0; j < n; ++j) {
                if (B.a[k][j] == NEG) continue;
                C.a[i][j] = max(C.a[i][j], A.a[i][k] + B.a[k][j]);
            }
        }
    }
    return C;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long N;
    int m, A;
    if (!(cin >> N >> m >> A)) return 0;

    vector<Node> trie(1);
    for (int i = 0; i < m; ++i) {
        string s;
        long long w;
        cin >> s >> w;
        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (trie[u].nxt[c] == -1) {
                trie[u].nxt[c] = int(trie.size());
                trie.emplace_back();
            }
            u = trie[u].nxt[c];
        }
        trie[u].val += w;
    }

    queue<int> q;
    for (int c = 0; c < A; ++c) {
        int v = trie[0].nxt[c];
        if (v == -1) trie[0].nxt[c] = 0;
        else {
            trie[v].fail = 0;
            q.push(v);
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        trie[u].val += trie[trie[u].fail].val;
        for (int c = 0; c < A; ++c) {
            int v = trie[u].nxt[c];
            if (v == -1) {
                trie[u].nxt[c] = trie[trie[u].fail].nxt[c];
            } else {
                trie[v].fail = trie[trie[u].fail].nxt[c];
                q.push(v);
            }
        }
    }

    int S = int(trie.size());
    Matrix base(S);
    for (int u = 0; u < S; ++u) {
        for (int c = 0; c < A; ++c) {
            int v = trie[u].nxt[c];
            base.a[v][u] = max(base.a[v][u], trie[v].val);
        }
    }

    Matrix res(S, true);
    while (N > 0) {
        if (N & 1) res = multiply(base, res);
        N >>= 1;
        if (N) base = multiply(base, base);
    }

    long long ans = NEG;
    for (int s = 0; s < S; ++s) ans = max(ans, res.a[s][0]);
    cout << ans << '\n';
    return 0;
}
