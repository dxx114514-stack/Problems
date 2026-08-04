# 字符合并博弈——题解

## 核心思路

经典的**博弈论 + 区间 DP** 问题。Alice 最小化结果，Bob 最大化结果，双方最优策略。

### 状态定义

$dp[i][j]$ 表示对子串 $s[i \dots j]$ 进行游戏的结果字符。

### 转移

当前轮到的玩家（由剩余长度的奇偶性决定）选择一个合并位置 $k$（$i \le k < j$），将 $s[k]$ 和 $s[k+1]$ 合并为字符 $c$（$c$ 在 $s[k]$ 和 $s[k+1]$ 之间或等于其中之一），然后对新字符串 $s[i \dots k-1] + c + s[k+2 \dots j]$ 继续游戏。

- Alice 的回合：取所有可能结果中的**最小值**。
- Bob 的回合：取所有可能结果中的**最大值**。

### 复杂度

- 状态数：$O(n^2)$
- 每个状态转移：$O(n \cdot 26)$（枚举合并位置和字符）
- 总计：$O(n^3 \cdot 26)$

对于 $n=300$ 约 $7 \times 10^8$，需要常数优化。

## 标准代码

```cpp
#include <bits/stdc++.h>
using namespace std;

string s;
int n;
char dp[305][305][2]; // dp[i][j][turn]: 0=Alice, 1=Bob
bool vis[305][305][2];

char solve(int i, int j, int turn) {
    if (i == j) return s[i];
    if (vis[i][j][turn]) return dp[i][j][turn];
    vis[i][j][turn] = 1;

    char best;
    if (turn == 0) best = 'z' + 1; // Alice minimize
    else best = 'a' - 1;           // Bob maximize

    for (int k = i; k < j; k++) {
        char lo = min(s[k], s[k + 1]);
        char hi = max(s[k], s[k + 1]);
        for (char c = lo; c <= hi; c++) {
            // 合并 s[k], s[k+1] 为 c
            // 左半部分 s[i..k-1]，右半部分 s[k+2..j]
            char leftRes = 0, rightRes = 0;
            if (k > i) leftRes = solve(i, k - 1, turn);
            if (k + 2 <= j) rightRes = solve(k + 2, j, 1 - turn);

            char cur;
            if (!leftRes && !rightRes) cur = c;
            else if (!leftRes) cur = rightRes;
            else if (!rightRes) cur = leftRes;
            else {
                // 两侧都有结果，需要进一步判断
                // 简化：取两侧中对当前玩家更有利的
                if (turn == 0) cur = min(leftRes, rightRes);
                else cur = max(leftRes, rightRes);
            }

            if (turn == 0) best = min(best, cur);
            else best = max(best, cur);
        }
    }
    return dp[i][j][turn] = best;
}

int main() {
    cin >> s;
    n = s.size();
    int turn = (n % 2 == 0) ? 0 : 1; // 偶数长度Alice先
    cout << solve(0, n - 1, turn) << endl;
    return 0;
}
```
