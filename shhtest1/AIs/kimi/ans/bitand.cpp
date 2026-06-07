#include <bits/stdc++.h>
using namespace std;

struct Node {
    long long sum;
    int orv;
} tr[800005];
int a[200005];

void build(int u, int l, int r) {
    if (l == r) {
        tr[u].sum = a[l];
        tr[u].orv = a[l];
        return;
    }
    int mid = (l + r) >> 1;
    build(u<<1, l, mid);
    build(u<<1|1, mid+1, r);
    tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum;
    tr[u].orv = tr[u<<1].orv | tr[u<<1|1].orv;
}

void update(int u, int l, int r, int L, int R, int x) {
    if (r < L || l > R) return;
    // 剪枝：如果区间所有位的1都是x的位的子集，则 &x 不变
    if ((tr[u].orv & x) == tr[u].orv) return;
    if (l == r) {
        a[l] &= x;
        tr[u].sum = a[l];
        tr[u].orv = a[l];
        return;
    }
    int mid = (l + r) >> 1;
    update(u<<1, l, mid, L, R, x);
    update(u<<1|1, mid+1, r, L, R, x);
    tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum;
    tr[u].orv = tr[u<<1].orv | tr[u<<1|1].orv;
}

long long query(int u, int l, int r, int L, int R) {
    if (L <= l && r <= R) return tr[u].sum;
    int mid = (l + r) >> 1;
    long long res = 0;
    if (L <= mid) res += query(u<<1, l, mid, L, R);
    if (R > mid) res += query(u<<1|1, mid+1, r, L, R);
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; ++i) cin >> a[i];
    build(1, 1, n);
    
    while (m--) {
        int type, l, r, x;
        cin >> type >> l >> r;
        if (type == 1) {
            cin >> x;
            update(1, 1, n, l, r, x);
        } else {
            cout << query(1, 1, n, l, r) << "\n";
        }
    }
    return 0;
}