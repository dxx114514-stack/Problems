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
DIR = "bitand_data"
os.makedirs(DIR, exist_ok=True)

def write_file(pid, n, m, a, ops):
    infile = os.path.join(DIR, f"{pid:02d}.in")
    outfile = os.path.join(DIR, f"{pid:02d}.out")
    
    with open(infile, 'w') as f:
        f.write(f"{n} {m}\n")
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
    print(f"[BitAnd] Point {pid:02d} done | n={n:6d} m={m:6d} | run={t_run:.3f}s mem={mem_str}")

def gen(pid):
    random.seed(20260517 + pid * 17)
    
    if pid <= 2:
        # 点 1~2: 极小数据
        n = random.randint(50, 100)
        m = random.randint(50, 100)
        V = 1000
        a = [random.randint(0, V) for _ in range(n)]
        ops = []
        for _ in range(m):
            l = random.randint(1, n)
            r = random.randint(1, n)
            if l > r: l, r = r, l
            if random.random() < 0.5:
                x = random.randint(0, V)
                ops.append(f"1 {l} {r} {x}")
            else:
                ops.append(f"2 {l} {r}")
    
    elif pid <= 4:
        # 点 3~4: 5e3，x=0，全变0
        n = 5000
        m = 5000
        V = 10**6
        a = [random.randint(0, V) for _ in range(n)]
        ops = []
        for _ in range(m):
            l = random.randint(1, n)
            r = random.randint(1, n)
            if l > r: l, r = r, l
            if random.random() < 0.7:
                ops.append(f"1 {l} {r} 0")
            else:
                ops.append(f"2 {l} {r}")
    
    elif pid <= 6:
        # 点 5~6: 1e5，x=2^30-1，无修改
        n = 100000
        m = 100000
        V = 10**6
        a = [random.randint(0, V) for _ in range(n)]
        ops = []
        for _ in range(m):
            l = random.randint(1, n)
            r = random.randint(1, n)
            if l > r: l, r = r, l
            if random.random() < 0.7:
                ops.append(f"1 {l} {r} {(1 << 30) - 1}")
            else:
                ops.append(f"2 {l} {r}")
    
    elif pid <= 8:
        # 点 7~8: 1e5，全相同数组，剪枝失效
        n = 100000
        m = 100000
        V = (1 << 30) - 1
        same_val = random.randint(0, V)
        a = [same_val] * n
        ops = []
        for _ in range(m):
            l = random.randint(1, n)
            r = random.randint(1, n)
            if l > r: l, r = r, l
            if random.random() < 0.6:
                x = random.randint(0, V)
                ops.append(f"1 {l} {r} {x}")
            else:
                ops.append(f"2 {l} {r}")
    
    elif pid <= 10:
        # 点 9~10: 2e5，只查询
        n = 200000
        m = 200000
        V = 10**9
        a = [random.randint(0, V) for _ in range(n)]
        ops = []
        for _ in range(m):
            l = random.randint(1, n)
            r = random.randint(1, n)
            if l > r: l, r = r, l
            ops.append(f"2 {l} {r}")
    
    elif pid <= 15:
        # 点 11~15: 2e5，x 随机，剪枝率低
        n = 200000
        m = 200000
        V = (1 << 30) - 1
        a = [random.randint(0, V) for _ in range(n)]
        ops = []
        for _ in range(m):
            l = random.randint(1, n)
            r = random.randint(1, n)
            if l > r: l, r = r, l
            if random.random() < 0.6:
                x = random.randint(0, V)
                ops.append(f"1 {l} {r} {x}")
            else:
                ops.append(f"2 {l} {r}")
    
    elif pid <= 18:
        # 点 16~18: 2e5，x 逐位清0
        n = 200000
        m = 200000
        V = (1 << 30) - 1
        a = [V] * n  # 全1
        ops = []
        bits = list(range(30))
        random.shuffle(bits)
        bit_idx = 0
        for i in range(m):
            if i % 10 == 0 and bit_idx < 30:
                # 每10个操作清一位
                mask = V ^ (1 << bits[bit_idx])
                bit_idx += 1
                l = 1
                r = n
                ops.append(f"1 {l} {r} {mask}")
            else:
                l = random.randint(1, n)
                r = random.randint(1, n)
                if l > r: l, r = r, l
                if random.random() < 0.3:
                    x = random.randint(0, V)
                    ops.append(f"1 {l} {r} {x}")
                else:
                    ops.append(f"2 {l} {r}")
    
    else:
        # 点 19~20: 2e5，完全随机
        n = 200000
        m = 200000
        V = (1 << 30) - 1
        a = [random.randint(0, V) for _ in range(n)]
        ops = []
        for _ in range(m):
            l = random.randint(1, n)
            r = random.randint(1, n)
            if l > r: l, r = r, l
            if random.random() < 0.6:
                x = random.randint(0, V)
                ops.append(f"1 {l} {r} {x}")
            else:
                ops.append(f"2 {l} {r}")
    
    write_file(pid, n, m, a, ops)

def main():
    for i in range(1, 21):
        gen(i)

if __name__ == "__main__":
    main()