#include <bits/stdc++.h>
using namespace std;

struct Node {
    int mx, mn;
} tr[800005];
int a[200005];
int n, m;
long long K;

void build(int u, int l, int r) {
    if (l == r) {
        tr[u].mx = tr[u].mn = a[l];
        return;
    }
    int mid = (l + r) >> 1;
    build(u<<1, l, mid);
    build(u<<1|1, mid+1, r);
    tr[u].mx = max(tr[u<<1].mx, tr[u<<1|1].mx);
    tr[u].mn = min(tr[u<<1].mn, tr[u<<1|1].mn);
}

void modify(int u, int l, int r, int p, int x) {
    if (l == r) {
        tr[u].mx = tr[u].mn = x;
        return;
    }
    int mid = (l + r) >> 1;
    if (p <= mid) modify(u<<1, l, mid, p, x);
    else modify(u<<1|1, mid+1, r, p, x);
    tr[u].mx = max(tr[u<<1].mx, tr[u<<1|1].mx);
    tr[u].mn = min(tr[u<<1].mn, tr[u<<1|1].mn);
}

// 在线段树上二分：从位置 p 开始，找到最远的 r 使得 [p,r] 的 max-min <= K
// 返回最远的合法下标（0-based 或 1-based，与输入一致）
int queryFar(int p) {
    int curMx = a[p];
    int curMn = a[p];
    int ans = p;
    
    function<void(int,int,int)> dfs = [&](int u, int l, int r) {
        if (r < p) return;
        if (l >= p) {
            int nmx = max(curMx, tr[u].mx);
            int nmn = min(curMn, tr[u].mn);
            if ((long long)nmx - nmn <= K) {
                curMx = nmx;
                curMn = nmn;
                ans = r;
                return;
            }
            if (l == r) return;
        }
        int mid = (l + r) >> 1;
        if (p <= mid) {
            dfs(u<<1, l, mid);
            if (ans >= mid) dfs(u<<1|1, mid+1, r);
        } else {
            dfs(u<<1|1, mid+1, r);
        }
    };
    
    dfs(1, 1, n);
    return ans - p + 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> m >> K;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    
    while (m--) {
        int type;
        cin >> type;
        if (type == 1) {
            int i, x;
            cin >> i >> x;
            a[i] = x;
            modify(1, 1, n, i, x);
        } else {
            int i;
            cin >> i;
            cout << queryFar(i) << "\n";
        }
    }
    return 0;
}