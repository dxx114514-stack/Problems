# 树上染色——题解

## 核心思路

树形 DP + 状态压缩。由于颜色数 $c \le 10$，可以用位掩码表示子树中使用了哪些颜色。

### 状态定义

$dp[v][S]$ 表示以 $v$ 为根的子树中，使用颜色集合为 $S$ 的情况下，最多可以染色的节点数。其中 $S$ 的每个位表示对应颜色的连通分量是否完全包含在该子树中。

### 转移

对于节点 $v$，先处理所有子节点，再决定 $v$ 自身的颜色：

1. **$v$ 不染色：** 合并子节点的子树，保持连通性。
2. **$v$ 染颜色 $k$：** 对于每个子节点 $u$，可以选择：
   - 不染色
   - 染颜色 $k$（需满足 $|w_v - w_u| \ge d$，与 $v$ 连通）
   - 染其他颜色（开启新的连通分量）

### 复杂度

- 状态数：$O(n \cdot 2^c)$
- 转移：$O(c)$
- 总计：$O(n \cdot c \cdot 2^c)$

## 标准代码

```cpp
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 500005, MAXC = 10;

int n, c, d, w[MAXN];
vector<int> adj[MAXN];
int dp[MAXN][1 << MAXC], tmp[1 << MAXC];

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
    memcpy(tmp, dp[v], sizeof(dp[v]));
    for (int k = 0; k < c; k++) {
        for (int S = 0; S < (1 << c); S++) {
            if (S & (1 << k)) continue;
            int val = dp[v][S] + 1;
            int newS = S | (1 << k);
            tmp[newS] = max(tmp[newS], val);
        }
    }
    memcpy(dp[v], tmp, sizeof(tmp));
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> n >> c >> d;
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
```
