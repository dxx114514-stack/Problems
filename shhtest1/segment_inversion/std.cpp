#include <bits/stdc++.h>
using namespace std;
const int MOD = 1e9 + 7;

int main() {
    int n, k;
    cin >> n >> k;
    if (n == 1) { cout << (k == 0 ? 1 : 0) << endl; return 0; }
    // dp[i][b][j] = 长度为i、断点数为b、最少操作数为j的排列数
    vector<vector<vector<int>>> dp(n + 1,
        vector<vector<int>>(n + 2, vector<int>(k + 1, 0)));
    dp[1][0][0] = 1;
    for (int i = 2; i <= n; i++) {
        for (int b = 0; b <= i; b++) {
            for (int j = 0; j <= k; j++) {
                if (dp[i-1][b][j] == 0) continue;
                for (int db = -1; db <= 2; db++) {
                    int nb = b + db;
                    if (nb < 0 || nb > i) continue;
                    int nj = max(j, (nb + 1) / 2);
                    if (nj > k) continue;
                    int ways = 0;
                    if (db == -1) ways = 1;
                    else if (db == 0) ways = max(0, i - 2);
                    else if (db == 1) ways = 2;
                    else ways = 0;
                    if (ways == 0) continue;
                    dp[i][nb][nj] = (dp[i][nb][nj] +
                        1LL * dp[i-1][b][j] * ways) % MOD;
                }
            }
        }
    }
    int ans = 0;
    for (int b = 0; b <= n; b++)
        ans = (ans + dp[n][b][k]) % MOD;
    cout << ans << endl;
    return 0;
}
