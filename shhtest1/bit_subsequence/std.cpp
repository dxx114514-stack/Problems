#include <bits/stdc++.h>
using namespace std;
const int MOD = 1e9 + 7;

int main() {
    int n, k;
    string s;
    cin >> n >> k >> s;
    map<pair<int,int>, int> dp[n][n];
    for (int i = 0; i < n; i++) {
        int bit = s[i] - '0';
        dp[i][i][{1, bit % 3}] = 1;
    }
    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;
            dp[l][r] = dp[l+1][r-1 >= l ? r-1 : l];
            { int bit = s[l] - '0'; dp[l][r][{1, bit % 3}]++; }
            if (r > l) { int bit = s[r] - '0'; dp[l][r][{1, bit % 3}]++; }
            if (s[l] == s[r]) {
                int bit = s[l] - '0';
                auto& inner = dp[l+1][r-1 >= l ? r-1 : l];
                for (auto& [key, cnt] : inner) {
                    int newLen = key.first + 2;
                    int newVal = (key.second * 2 + 2 * bit) % 3;
                    dp[l][r][{newLen, newVal}] =
                        (dp[l][r][{newLen, newVal}] + cnt) % MOD;
                }
                dp[l][r][{2, (2 * bit) % 3}]++;
            }
        }
    }
    int ans = 0;
    for (int l = 0; l < n; l++)
        for (int r = l; r < n; r++)
            for (auto& [key, cnt] : dp[l][r])
                if (key.first >= k && key.second == 0)
                    ans = (ans + cnt) % MOD;
    cout << ans << endl;
    return 0;
}
