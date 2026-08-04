# 区间翻转排序——题解

## 核心思路

本题要求统计最少需要恰好 $k$ 次区间翻转才能排好序的排列个数。这是一个经典的**排列排序**问题。

### 断点（Breakpoint）分析

在排列前后加上哨兵 $0$ 和 $n+1$，得到 $[0, p_1, p_2, \dots, p_n, n+1]$。如果相邻两个数之差不为 $1$，则称这两个位置之间有一个**断点**。

- 排好序的排列有 $0$ 个断点。
- 每次翻转操作最多减少 $2$ 个断点。
- 因此 $f(p) \ge \lceil B(p)/2 \rceil$，其中 $B(p)$ 是断点数。

### 动态规划

设 $dp[i][b][k]$ 表示前 $i$ 个元素、断点数为 $b$、最少操作数为 $k$ 的方案数。

转移时考虑将元素 $i$ 插入到长度为 $i-1$ 的排列的不同位置，根据相邻元素的变化更新断点数。

**复杂度：** $O(n^2 \cdot k)$

## 标准代码

```cpp
#include <bits/stdc++.h>
using namespace std;
const int MOD = 1e9 + 7;

int main() {
    int n, k;
    cin >> n >> k;
    if (n == 1) {
        cout << (k == 0 ? 1 : 0) << endl;
        return 0;
    }
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
```
