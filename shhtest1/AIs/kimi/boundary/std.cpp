#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<long long> h(n);
    for (int i = 0; i < n; ++i) cin >> h[i];
    
    vector<long long> d(n);
    for (int i = 0; i < n; ++i) {
        long long prev = h[(i - 1 + n) % n];
        d[i] = h[i] - prev;
    }
    
    auto isBorder = [&](int i) -> bool {
        long long dl = d[i];
        long long dr = d[(i + 1) % n];
        if (dl == 0 || dr == 0) return false;
        return (dl > 0) != (dr > 0);
    };
    
    set<int> borders;
    for (int i = 0; i < n; ++i) {
        if (isBorder(i)) borders.insert(i);
    }
    
    while (m--) {
        int type;
        cin >> type;
        if (type == 1) {
            int l, r;
            long long x;
            cin >> l >> r >> x;
            --l; --r; // 0-based
            
            int posL = l;
            int posR = (r + 1) % n;
            
            // 收集所有可能受影响的位置（修改 d[pos] 会影响位置 pos 和 pos-1 的边界态）
            vector<int> check;
            auto add = [&](int p) {
                for (int delta = -1; delta <= 0; ++delta) {
                    int idx = (p + delta + n) % n;
                    check.push_back(idx);
                }
            };
            add(posL);
            add(posR);
            
            // 去重
            sort(check.begin(), check.end());
            check.erase(unique(check.begin(), check.end()), check.end());
            
            // 移除旧的边界态
            for (int p : check) {
                if (borders.count(p)) borders.erase(p);
            }
            
            d[posL] += x;
            d[posR] -= x;
            
            // 添加新的边界态
            for (int p : check) {
                if (isBorder(p)) borders.insert(p);
            }
        } else {
            cout << (int)borders.size() << "\n";
        }
    }
    return 0;
}