#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
int P;
long long pw[200005];
long long spw[200005]; // spw[i] = sum_{j=0}^{i-1} P^j % MOD

int val(char c) {
    if (c == 'A') return 1;
    if (c == 'T') return 2;
    if (c == 'C') return 3;
    return 4; // G
}

struct Node {
    int cnt[4];
    long long hsh;
    int len;
    bool hasTag;
    int tag[4];
} tr[800005];

// O(4) 计算排序状态的哈希值，假设从位置0开始
long long calcHash(int c[4], int len) {
    long long res = 0;
    int pos = 0;
    int vals[4] = {1, 2, 3, 4};
    for (int t = 0; t < 4; ++t) {
        if (c[t] == 0) continue;
        int l = pos;
        int r = pos + c[t];
        long long seg = (spw[r] - spw[l] + MOD) % MOD;
        res = (res + vals[t] * seg) % MOD;
        pos = r;
    }
    return res;
}

void apply(int u, int c[4], int len) {
    for (int i = 0; i < 4; ++i) tr[u].cnt[i] = c[i];
    tr[u].len = len;
    tr[u].hsh = calcHash(c, len);
    tr[u].hasTag = true;
    for (int i = 0; i < 4; ++i) tr[u].tag[i] = c[i];
}

void pushup(int u) {
    for (int i = 0; i < 4; ++i) 
        tr[u].cnt[i] = tr[u<<1].cnt[i] + tr[u<<1|1].cnt[i];
    tr[u].len = tr[u<<1].len + tr[u<<1|1].len;
    tr[u].hsh = (tr[u<<1].hsh * pw[tr[u<<1|1].len] + tr[u<<1|1].hsh) % MOD;
    tr[u].hasTag = false;
}

// 下传懒标记：将父节点的排序状态分配给左右子树（左子树先拿前 lenL 个）
void pushdown(int u) {
    if (!tr[u].hasTag) return;
    int lenL = tr[u<<1].len;
    int lenR = tr[u<<1|1].len;
    
    int leftTag[4] = {0,0,0,0};
    int rem[4];
    for (int i = 0; i < 4; ++i) rem[i] = tr[u].tag[i];
    
    int need = lenL;
    for (int t = 0; t < 4 && need > 0; ++t) {
        int give = min(rem[t], need);
        leftTag[t] = give;
        rem[t] -= give;
        need -= give;
    }
    
    apply(u<<1, leftTag, lenL);
    apply(u<<1|1, rem, lenR);
    tr[u].hasTag = false;
}

void build(int u, int l, int r, string &s) {
    tr[u].hasTag = false;
    if (l == r) {
        tr[u].len = 1;
        for (int i = 0; i < 4; ++i) tr[u].cnt[i] = 0;
        tr[u].cnt[val(s[l-1]) - 1] = 1;
        tr[u].hsh = val(s[l-1]) % MOD;
        return;
    }
    int mid = (l + r) >> 1;
    build(u<<1, l, mid, s);
    build(u<<1|1, mid+1, r, s);
    pushup(u);
}

// 查询区间 [L,R] 的 cnt[4]
void queryCnt(int u, int l, int r, int L, int R, int res[4]) {
    if (L <= l && r <= R) {
        for (int i = 0; i < 4; ++i) res[i] += tr[u].cnt[i];
        return;
    }
    if (tr[u].hasTag) pushdown(u);
    int mid = (l + r) >> 1;
    if (L <= mid) queryCnt(u<<1, l, mid, L, R, res);
    if (R > mid) queryCnt(u<<1|1, mid+1, r, L, R, res);
}

// 区间排序转录：将 [L,R] 赋值为 totalC 描述的排序状态
void rangeAssign(int u, int l, int r, int L, int R, int totalC[4]) {
    if (R < l || r < L) return;
    if (L <= l && r <= R) {
        int myC[4] = {0,0,0,0};
        int skip = l - L;      // 当前节点在 [L,R] 中的起始偏移
        int need = r - l + 1;  // 当前节点需要的碱基数量
        for (int t = 0; t < 4; ++t) {
            if (skip >= totalC[t]) {
                skip -= totalC[t];
            } else {
                int have = totalC[t] - skip;
                int give = min(have, need);
                myC[t] = give;
                need -= give;
                skip = 0;
                if (need == 0) break;
            }
        }
        apply(u, myC, r - l + 1);
        return;
    }
    if (tr[u].hasTag) pushdown(u);
    int mid = (l + r) >> 1;
    rangeAssign(u<<1, l, mid, L, R, totalC);
    rangeAssign(u<<1|1, mid+1, r, L, R, totalC);
    pushup(u);
}

// 查询区间 [L,R] 的哈希值（返回排序后的哈希）
Node queryNode(int u, int l, int r, int L, int R) {
    if (L <= l && r <= R) return tr[u];
    if (tr[u].hasTag) pushdown(u);
    int mid = (l + r) >> 1;
    if (R <= mid) return queryNode(u<<1, l, mid, L, R);
    if (L > mid) return queryNode(u<<1|1, mid+1, r, L, R);
    Node left = queryNode(u<<1, l, mid, L, R);
    Node right = queryNode(u<<1|1, mid+1, r, L, R);
    Node res;
    for (int i = 0; i < 4; ++i) res.cnt[i] = left.cnt[i] + right.cnt[i];
    res.len = left.len + right.len;
    res.hsh = (left.hsh * pw[right.len] + right.hsh) % MOD;
    res.hasTag = false;
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m >> P;
    string s;
    cin >> s;
    
    pw[0] = 1;
    for (int i = 1; i <= n; ++i) pw[i] = pw[i-1] * P % MOD;
    spw[0] = 0;
    for (int i = 1; i <= n; ++i) {
        spw[i] = (spw[i-1] + pw[i-1]) % MOD;
    }
    
    build(1, 1, n, s);
    
    while (m--) {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 1) {
            int c[4] = {0,0,0,0};
            queryCnt(1, 1, n, l, r, c);
            rangeAssign(1, 1, n, l, r, c);
        } else {
            Node res = queryNode(1, 1, n, l, r);
            cout << res.hsh << "\n";
        }
    }
    return 0;
}