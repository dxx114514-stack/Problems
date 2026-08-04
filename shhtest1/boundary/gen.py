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
DIR = "boundary_data"
os.makedirs(DIR, exist_ok=True)

def write_file(pid, n, m, h, ops):
    infile = os.path.join(DIR, f"{pid:02d}.in")
    outfile = os.path.join(DIR, f"{pid:02d}.out")
    
    with open(infile, 'w') as f:
        f.write(f"{n} {m}\n")
        f.write(' '.join(map(str, h)) + "\n")
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
    print(f"[Boundary] Point {pid:02d} done | n={n:6d} m={m:6d} | run={t_run:.3f}s mem={mem_str}")

def gen(pid):
    random.seed(20260517 + pid)
    
    if pid <= 2:
        # 点 1~2: 极小数据，卡 O(nm) 暴力
        n = random.randint(50, 100)
        m = random.randint(50, 100)
        h = [random.randint(-1000, 1000) for _ in range(n)]
        ops = []
        for _ in range(m):
            if random.random() < 0.5:
                l = random.randint(1, n)
                r = random.randint(1, n)
                if l > r: l, r = r, l
                x = random.randint(-1000, 1000)
                ops.append(f"1 {l} {r} {x}")
            else:
                ops.append("2")
    
    elif pid <= 4:
        # 点 3~4: 中等数据，卡 O(n) 单次修改暴力
        n = random.randint(3000, 5000)
        m = random.randint(3000, 5000)
        h = [random.randint(-10**6, 10**6) for _ in range(n)]
        ops = []
        for _ in range(m):
            if random.random() < 0.6:
                l = random.randint(1, n)
                r = random.randint(1, n)
                if l > r: l, r = r, l
                x = random.randint(-10**6, 10**6)
                ops.append(f"1 {l} {r} {x}")
            else:
                ops.append("2")
    
    elif pid <= 6:
        # 点 5~6: 1e5 规模，卡 set 常数
        n = 100000
        m = 100000
        h = [random.randint(-10**7, 10**7) for _ in range(n)]
        ops = []
        for _ in range(m):
            if random.random() < 0.7:
                l = random.randint(1, n)
                r = random.randint(1, n)
                if l > r: l, r = r, l
                x = random.randint(-10**7, 10**7)
                ops.append(f"1 {l} {r} {x}")
            else:
                ops.append("2")
    
    elif pid <= 10:
        # 点 7~10: 满规模 2e5，卡所有带 log 做法
        n = 200000
        m = 200000
        h = [random.randint(-10**9, 10**9) for _ in range(n)]
        ops = []
        for _ in range(m):
            if random.random() < 0.7:
                l = random.randint(1, n)
                r = random.randint(1, n)
                if l > r: l, r = r, l
                x = random.randint(-10**9, 10**9)
                ops.append(f"1 {l} {r} {x}")
            else:
                ops.append("2")
    
    elif pid <= 15:
        # 点 11~15: 满规模，全操作2（查询），卡查询不优化
        n = 200000
        m = 200000
        h = [random.randint(-10**9, 10**9) for _ in range(n)]
        ops = ["2"] * m  # 全查询
    
    else:
        # 点 16~20: 满规模，全操作1后接一个2，卡不批量处理
        n = 200000
        m = 200001
        h = [random.randint(-10**9, 10**9) for _ in range(n)]
        ops = []
        for _ in range(m - 1):
            l = random.randint(1, n)
            r = random.randint(1, n)
            if l > r: l, r = r, l
            x = random.randint(-10**9, 10**9)
            ops.append(f"1 {l} {r} {x}")
        ops.append("2")  # 最后查询一次
    
    write_file(pid, n, m, h, ops)

def main():
    for i in range(1, 21):
        gen(i)

if __name__ == "__main__":
    main()