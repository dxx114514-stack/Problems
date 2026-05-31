import random

random.seed(2025)

class DSU:
    def __init__(self, siz):
        self.p = list(range(siz + 1))
    def find(self, x):
        if self.p[x] != x:
            self.p[x] = self.find(self.p[x])
        return self.p[x]
    def union(self, x, y):
        fx = self.find(x)
        fy = self.find(y)
        if fx != fy:
            self.p[fx] = fy

for case in range(1, 21):
    n = random.randint(100, 100000)
    m = random.randint(0, 100000)
    lim = random.randint(1, 10**9)
    edges = []
    for _ in range(m):
        u = random.randint(1, n)
        v = random.randint(1, n)
        w = random.randint(1, 10**9)
        edges.append((u, v, w))
    
    with open(f"{case}.in", "w", encoding="utf-8") as f:
        f.write(f"{n} {m} {lim}\n")
        for u, v, w in edges:
            f.write(f"{u} {v} {w}\n")
    
    dsu = DSU(n)
    for u, v, w in edges:
        if w <= lim:
            dsu.union(u, v)
    block = 0
    for i in range(1, n+1):
        if dsu.find(i) == i:
            block += 1
    
    with open(f"{case}.ans", "w", encoding="utf-8") as f:
        f.write(str(block) + "\n")