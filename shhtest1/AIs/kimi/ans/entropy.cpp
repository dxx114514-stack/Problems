#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    long long k;
    cin >> n >> m >> k;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    
    // 断裂边：存储左端点 i，表示 i 与 i+1 之间断裂 (0 <= i < n-1)
    set<int> breaks;
    breaks.insert(-1);  // 哨兵：左侧边界
    breaks.insert(n-1); // 哨兵：右侧边界（表示 n-1 与 n 之间断裂，即结尾）
    
    auto isBreak = [&](int i) -> bool {
        if (i < 0 || i >= n - 1) return true; // 哨兵视为断裂
        return (a[i] ^ a[i+1]) > k;
    };
    
    long long S = 0; // 纠缠熵
    
    // 初始化断裂边
    for (int i = 0; i < n - 1; ++i) {
        if (isBreak(i)) breaks.insert(i);
    }
    
    // 计算初始 S：簇为 [prev+1, curr] 的闭区间，长度 = curr - prev
    int prev = -1;
    for (int b : breaks) {
        if (b == -1) continue;
        long long len = b - prev;
        S += len * len;
        prev = b;
    }
    
    while (m--) {
        int type;
        cin >> type;
        if (type == 1) {
            int p;
            long long x;
            cin >> p >> x;
            --p; // 0-based
            
            // 找到包含 p 的簇的左右断裂边
            auto itR = breaks.upper_bound(p);
            int rightBreak = *itR; // p 右侧的第一个断裂边
            --itR;
            int leftBreak = *itR;  // p 左侧的第一个断裂边
            
            long long oldLen = rightBreak - leftBreak;
            S -= oldLen * oldLen;
            
            // 移除旧的与 p 相关的断裂边
            if (p > 0) {
                auto it = breaks.find(p - 1);
                if (it != breaks.end()) breaks.erase(it);
            }
            if (p < n - 1) {
                auto it = breaks.find(p);
                if (it != breaks.end()) breaks.erase(it);
            }
            
            a[p] = x;
            
            // 添加新的断裂边
            if (p > 0 && isBreak(p - 1)) breaks.insert(p - 1);
            if (p < n - 1 && isBreak(p)) breaks.insert(p);
            
            // 重新计算包含 p 的簇（可能分裂或合并）
            itR = breaks.upper_bound(p);
            rightBreak = *itR;
            --itR;
            leftBreak = *itR;
            long long newLen = rightBreak - leftBreak;
            S += newLen * newLen;
        } else {
            cout << S << "\n";
        }
    }
    return 0;
}