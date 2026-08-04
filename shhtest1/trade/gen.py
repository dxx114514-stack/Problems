import os
import random
import subprocess
import time

try:
    import psutil
    HAS_PSUTIL = True
except ImportError:
    HAS_PSUTIL = False

ANS = "ans.exe"
DIR = "trade_data"
os.makedirs(DIR, exist_ok=True)

def write_file(pid, n, m, k, a, ops):
    infile = os.path.join(DIR, f"{pid:02d}.in")
    outfile = os.path.join(DIR, f"{pid:02d}.out")
    
    with open(infile, 'w') as f:
        f.write(f"{n} {m} {k}\n")
        f.write(' '.join(map(str, a)) + "\n")
        f.write('\n'.join(ops) + "\n")
    
    t0 = time.perf_counter()
    peak_mem = 0.0
    with open(infile, 'r') as fin, open(outfile, 'w') as fout:
        if HAS_PSUTIL:
            proc = subprocess.Popen([ANS], stdin=fin, stdout=fout)
            p = psutil.Process(proc.pid)
            try:
                while True:
                    ret = proc.poll()
                    if ret is not None:
                        break
                    try:
                        mi = p.memory_info()
                        mem_mb = (mi.peak_wset if os.name == 'nt' else mi.rss) / 1048576.0
                        if mem_mb > peak_mem:
                            peak_mem = mem_mb
                    except (psutil.NoSuchProcess, psutil.AccessDenied):
                        break
                    time.sleep(0.002)
                if proc.returncode != 0:
                    proc.wait()
            except subprocess.TimeoutExpired:
                proc.kill()
                raise
        else:
            subprocess.run([ANS], stdin=fin, stdout=fout, timeout=120, check=True)
    t_run = time.perf_counter() - t0
    
    mem_str = f"{peak_mem:.1f}MB" if HAS_PSUTIL else "N/A"
    print(f"[Trade] Point {pid:02d} done | n={n:6d} m={m:6d} k={k:10d} | run={t_run:.3f}s mem={mem_str}")

def gen(pid):
    random.seed(20260517 + pid * 11)
    
    if pid <= 2:
        # 点 1~2: 极小数据
        n = random.randint(50, 100)
        m = random.randint(50, 100)
        k = random.randint(0, 1000)
        a = [random.randint(0, 1000) for _ in range(n)]
        ops = []
        for _ in range(m):
            if random.random() < 0.5:
                idx = random.randint(1, n)
                x = random.randint(0, 1000)
                ops.append(f"1 {idx} {x}")
            else:
                idx = random.randint(1, n)
                ops.append(f"2 {idx}")
    
    elif pid <= 4:
        # 点 3~4: 中等数据
        n = random.randint(3000, 5000)
        m = random.randint(3000, 5000)
        k = random.randint(0, 10**6)
        a = [random.randint(0, 10**6) for _ in range(n)]
        ops = []
        for _ in range(m):
            if random.random() < 0.5:
                idx = random.randint(1, n)
                x = random.randint(0, 10**6)
                ops.append(f"1 {idx} {x}")
            else:
                idx = random.randint(1, n)
                ops.append(f"2 {idx}")
    
    elif pid <= 6:
        # 点 5~6: 1e5，单调序列，f(i) 很大
        n = 100000
        m = 100000
        k = 10**9
        base = random.randint(0, 10**6)
        a = [base + i for i in range(n)]  # 严格递增
        ops = []
        for _ in range(m):
            if random.random() < 0.3:
                idx = random.randint(1, n)
                # 保持单调性，修改后仍在范围内
                x = base + idx - 1 + random.randint(-100, 100)
                x = max(0, min(10**9, x))
                ops.append(f"1 {idx} {x}")
            else:
                idx = random.randint(1, n)
                ops.append(f"2 {idx}")
    
    elif pid <= 8:
        # 点 7~8: 1e5，随机修改同一点
        n = 100000
        m = 100000
        k = random.randint(10**8, 10**9)
        a = [random.randint(0, 10**9) for _ in range(n)]
        ops = []
        target = random.randint(1, n)
        for i in range(m):
            if i % 2 == 0:
                x = random.randint(0, 10**9)
                ops.append(f"1 {target} {x}")
            else:
                ops.append(f"2 {target}")
    
    elif pid <= 10:
        # 点 9~10: 2e5，只查询不修改
        n = 200000
        m = 200000
        k = random.randint(10**8, 10**9)
        a = [random.randint(0, 10**9) for _ in range(n)]
        ops = []
        for _ in range(m):
            idx = random.randint(1, n)
            ops.append(f"2 {idx}")
    
    elif pid <= 15:
        # 点 11~15: 2e5，修改后必查同一点
        n = 200000
        m = 200000
        k = random.randint(10**8, 10**9)
        a = [random.randint(0, 10**9) for _ in range(n)]
        ops = []
        target = random.randint(1, n)
        for i in range(m):
            if i % 2 == 0:
                x = random.randint(0, 10**9)
                ops.append(f"1 {target} {x}")
            else:
                ops.append(f"2 {target}")
    
    else:
        # 点 16~20: 2e5，完全随机
        n = 200000
        m = 200000
        k = random.randint(0, 10**9)
        a = [random.randint(0, 10**9) for _ in range(n)]
        ops = []
        for _ in range(m):
            if random.random() < 0.5:
                idx = random.randint(1, n)
                x = random.randint(0, 10**9)
                ops.append(f"1 {idx} {x}")
            else:
                idx = random.randint(1, n)
                ops.append(f"2 {idx}")
    
    write_file(pid, n, m, k, a, ops)

def main():
    for i in range(1, 21):
        gen(i)

if __name__ == "__main__":
    main()