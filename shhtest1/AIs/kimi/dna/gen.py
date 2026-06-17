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
DIR = "dna_data"
os.makedirs(DIR, exist_ok=True)

def write_file(pid, n, m, P, s, ops):
    infile = os.path.join(DIR, f"{pid:02d}.in")
    outfile = os.path.join(DIR, f"{pid:02d}.out")
    
    with open(infile, 'w') as f:
        f.write(f"{n} {m} {P}\n")
        f.write(s + "\n")
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
    print(f"[DNA] Point {pid:02d} done | n={n:6d} m={m:6d} P={P:10d} | run={t_run:.3f}s mem={mem_str}")

def gen(pid):
    random.seed(20260517 + pid * 13)
    chars = ['A', 'T', 'C', 'G']
    
    if pid <= 2:
        # 点 1~2: 极小数据
        n = random.randint(50, 100)
        m = random.randint(50, 100)
        P = random.randint(2, 1000)
        s = ''.join(random.choice(chars) for _ in range(n))
        ops = []
        for _ in range(m):
            l = random.randint(1, n)
            r = random.randint(1, n)
            if l > r: l, r = r, l
            if random.random() < 0.5:
                ops.append(f"1 {l} {r}")
            else:
                ops.append(f"2 {l} {r}")
    
    elif pid <= 4:
        # 点 3~4: 5e3，全排序操作，卡不懒标记
        n = 5000
        m = 5000
        P = random.randint(2, 10**6)
        s = ''.join(random.choice(chars) for _ in range(n))
        ops = []
        for _ in range(m):
            l = 1
            r = n
            ops.append(f"1 {l} {r}")
    
    elif pid <= 6:
        # 点 5~6: 1e5，只查询
        n = 100000
        m = 100000
        P = random.randint(2, 10**6)
        s = ''.join(random.choice(chars) for _ in range(n))
        ops = []
        for _ in range(m):
            l = random.randint(1, n)
            r = random.randint(1, n)
            if l > r: l, r = r, l
            ops.append(f"2 {l} {r}")
    
    elif pid <= 8:
        # 点 7~8: 1e5，只排序整个数组
        n = 100000
        m = 100000
        P = random.randint(2, 10**6)
        s = ''.join(random.choice(chars) for _ in range(n))
        ops = []
        for _ in range(m):
            l = 1
            r = n
            ops.append(f"1 {l} {r}")
    
    elif pid <= 10:
        # 点 9~10: 2e5，P=2，卡不取模
        n = 200000
        m = 200000
        P = 2
        s = ''.join(random.choice(chars) for _ in range(n))
        ops = []
        for _ in range(m):
            l = random.randint(1, n)
            r = random.randint(1, n)
            if l > r: l, r = r, l
            if random.random() < 0.5:
                ops.append(f"1 {l} {r}")
            else:
                ops.append(f"2 {l} {r}")
    
    elif pid <= 15:
        # 点 11~15: 2e5，交替排序查询同区间
        n = 200000
        m = 200000
        P = random.randint(10**8, 998244352)
        s = ''.join(random.choice(chars) for _ in range(n))
        ops = []
        fixed_l = random.randint(1, n // 2)
        fixed_r = random.randint(n // 2, n)
        for i in range(m):
            if i % 2 == 0:
                ops.append(f"1 {fixed_l} {fixed_r}")
            else:
                ops.append(f"2 {fixed_l} {fixed_r}")
    
    elif pid <= 18:
        # 点 16~18: 2e5，全排序后全查询
        n = 200000
        m = 400000
        P = random.randint(10**8, 998244352)
        s = ''.join(random.choice(chars) for _ in range(n))
        ops = []
        # 先全排序
        ops.append(f"1 1 {n}")
        # 再全查询
        for _ in range(m - 1):
            l = random.randint(1, n)
            r = random.randint(1, n)
            if l > r: l, r = r, l
            ops.append(f"2 {l} {r}")
    
    else:
        # 点 19~20: 2e5，完全随机，唯一宽松点
        n = 200000
        m = 200000
        P = random.randint(2, 998244352)
        s = ''.join(random.choice(chars) for _ in range(n))
        ops = []
        for _ in range(m):
            l = random.randint(1, n)
            r = random.randint(1, n)
            if l > r: l, r = r, l
            if random.random() < 0.5:
                ops.append(f"1 {l} {r}")
            else:
                ops.append(f"2 {l} {r}")
    
    write_file(pid, n, m, P, s, ops)

def main():
    for i in range(1, 21):
        gen(i)

if __name__ == "__main__":
    main()