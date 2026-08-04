# 位串回文子序列——题解

## 核心思路

区间 DP，从两端向中间扩展回文结构，同时维护值对 3 取模的结果。

### 关键性质

二进制数模 3 的递推：设当前值为 $v$，加入新最高位 $b$ 后，新值为 $v' = b \cdot 2^{L} + v$，其中 $L$ 为原串长度。模 3 下：$v' \equiv b \cdot 2^{L} + v \pmod{3}$。

由于回文串从两端对称增长，可以同时追踪长度和值。

### 状态定义

$dp[i][j]$ 为一个哈希表/数组，记录子串 $s[i \dots j]$ 中所有回文子序列的 $(\text{长度},\ \text{值} \bmod 3) \to \text{方案数}$ 的映射。

### 转移

对于区间 $[i, j]$：
1. 继承 $[i+1, j-1]$ 的所有回文子序列。
2. 如果 $s[i] = s[j]$，将 $[i+1, j-1]$ 中每个回文子序列的两端各加一个 $s[i]$，更新长度和模 3 值。
3. 加入单字符 $s[i]$ 和 $s[j]$（如果 $i \ne j$）。

### 复杂度

- 状态数：$O(n^2)$
- 每个状态：$O(n)$（枚举子序列长度）
- 总计：$O(n^3)$

## 标准代码

```cpp
#include <bits/stdc++.h>
using namespace std;
const int MOD = 1e9 + 7;

int main() {
    int n, k;
    string s;
    cin >> n >> k >> s;

    // dp[l][r]: map of (length, val%3) -> count
    map<pair<int,int>, int> dp[n][n];

    for (int i = 0; i < n; i++) {
        int bit = s[i] - '0';
        dp[i][i][{1, bit % 3}] = 1;
    }

    for (int len = 2; len <= n; len++) {
        for (int l = 0; l + len - 1 < n; l++) {
            int r = l + len - 1;
            dp[l][r] = dp[l+1][r-1 >= l ? r-1 : l]; // 继承内部

            // 加入单字符
            { int bit = s[l] - '0'; dp[l][r][{1, bit % 3}]++; }
            if (r > l) { int bit = s[r] - '0'; dp[l][r][{1, bit % 3}]++; }

            // 两端匹配时扩展
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
```
