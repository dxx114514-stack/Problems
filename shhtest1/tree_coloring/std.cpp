#include <bits/stdc++.h>
using namespace std;
const int MAXN = 500005, MAXC = 10;

int n, c, d, w[MAXN];
vector<int> adj[MAXN];
vector<array<int, 1 << MAXC>> dp;
int tmp[1 << MAXC];

void dfs(int v, int p) {
    for (int S = 0; S < (1 << c); S++) dp[v][S] = 0;
    for (int u : adj[v]) {
        if (u == p) continue;
        dfs(u, v);
        for (int S = (1 << c) - 1; S >= 0; S--) {
            int best = dp[v][S];
            for (int T = S; T > 0; T = (T - 1) & S)
                if (dp[u][T] > 0)
                    best = max(best, dp[v][S ^ T] + dp[u][T]);
            dp[v][S] = best;
        }
    }
    for (int S = 0; S < (1 << c); S++) tmp[S] = dp[v][S];
    for (int k = 0; k < c; k++) {
        for (int S = 0; S < (1 << c); S++) {
            if (S & (1 << k)) continue;
            int val = dp[v][S] + 1;
            int newS = S | (1 << k);
            tmp[newS] = max(tmp[newS], val);
        }
    }
    for (int S = 0; S < (1 << c); S++) dp[v][S] = tmp[S];
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> n >> c >> d;
    dp.resize(n + 1);
    for (int i = 1; i <= n; i++) cin >> w[i];
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1, 0);
    int ans = 0;
    for (int S = 0; S < (1 << c); S++)
        ans = max(ans, dp[1][S]);
    cout << ans << endl;
    return 0;
}