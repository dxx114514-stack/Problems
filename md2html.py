#!/usr/bin/env python3
"""
md2html.py - 递归转换 Markdown 到 HTML
- 默认读取当前目录及子目录
- 输出到 ./docs，保持目录结构
- 左上角带返回按钮
- 自动提取标题（按 H1 -> H2 -> H3 优先级降级查找）
"""

import re
import sys
import os
import markdown

# ============================================================
#  1. 代码块保护（防止自定义语法在代码块内被误处理）
# ============================================================
def extract_code_blocks(text):
    blocks = {}
    counter = [0]
    def replace(m):
        key = f'\x00CODEBLOCK{counter[0]}\x00'
        blocks[key] = m.group(0)
        counter[0] += 1
        return key
    # 围栏代码块
    text = re.sub(r'```[\s\S]*?```', replace, text)
    # 行内代码
    text = re.sub(r'(?<!`)`(?!`)[^`\n]+?(?<!`)`(?!`)', replace, text)
    return text, blocks

def restore_code_blocks(text, blocks):
    for key, val in blocks.items():
        text = text.replace(key, val)
    return text

# ============================================================
#  2. 自定义容器/板块  :::type(text){state}
# ============================================================
def process_containers(text):
    pattern = re.compile(
        r'^:::(info|warn)\(([^)]*)\)\{(open|close)\}\s*$(.*?)^:::\s*$',
        re.MULTILINE | re.DOTALL
    )
    def replacer(m):
        ctype, title, state, content = m.group(1), m.group(2), m.group(3), m.group(4)
        content = process_containers(content)  # 递归
        content = process_bilibili(content)
        content = process_url_embed(content)
        content = markdown.markdown(content.strip(), extensions=['extra'])
        
        colors = {
            'info': {'border': '#3b82f6', 'bg': '#eff6ff', 'icon': 'ℹ️'},
            'warn': {'border': '#f59e0b', 'bg': '#fffbeb', 'icon': '⚠️'},
        }
        c = colors.get(ctype, colors['info'])
        
        if state == 'open':
            return (f'<details open class="custom-block block-{ctype}" '
                    f'style="border-left:4px solid {c["border"]};background:{c["bg"]};'
                    f'padding:12px 16px;margin:16px 0;border-radius:4px;">\n'
                    f'<summary style="font-weight:bold;cursor:pointer;">{c["icon"]} {title}</summary>\n'
                    f'{content}\n</details>')
        else:
            return (f'<div class="custom-block block-{ctype}" '
                    f'style="border-left:4px solid {c["border"]};background:{c["bg"]};'
                    f'padding:12px 16px;margin:16px 0;border-radius:4px;">\n'
                    f'<p style="font-weight:bold;margin:0 0 8px 0;">{c["icon"]} {title}</p>\n'
                    f'{content}\n</div>')
    
    prev = None
    while prev != text:
        prev = text
        text = pattern.sub(replacer, text)
    return text

# ============================================================
#  3. @[bilibili](BV号) & @[url](链接)
# ============================================================
def process_bilibili(text):
    def replacer(m):
        bv = m.group(1).strip()
        return (f'<iframe src="//player.bilibili.com/player.html?bvid={bv}'
                f'&high_quality=1&danmaku=0" style="width:100%;aspect-ratio:16/9;'
                f'border:none;" scrolling="no" frameborder="0" allowfullscreen="true" '
                f'sandbox="allow-top-navigation allow-same-origin allow-forms allow-scripts"></iframe>')
    return re.sub(r'@\[bilibili\]\(([^)]+)\)', replacer, text)

def process_url_embed(text):
    def replacer(m):
        url = m.group(1).strip()
        return (f'<iframe src="{url}" style="width:100%;height:500px;border:1px solid #ddd;'
                f'border-radius:4px;" sandbox="allow-scripts allow-same-origin allow-forms allow-popups"></iframe>')
    return re.sub(r'@\[url\]\(([^)]+)\)', replacer, text)

# ============================================================
#  4. 主转换逻辑 (支持 H1 -> H2 -> H3 降级提取标题)
# ============================================================
def extract_title(md_text):
    """按优先级提取标题：H1 -> H2 -> H3，都没有则返回默认名"""
    # 优先找 # 标题
    match = re.search(r'^#\s+(.+)$', md_text, re.MULTILINE)
    if match:
        return match.group(1).strip()
    
    # 没有 H1，顺延找 ## 标题
    match = re.search(r'^##\s+(.+)$', md_text, re.MULTILINE)
    if match:
        return match.group(1).strip()
        
    # 没有 H2，顺延找 ### 标题
    match = re.search(r'^###\s+(.+)$', md_text, re.MULTILINE)
    if match:
        return match.group(1).strip()
        
    return "Document"  # 实在没有就用默认名

def md2html(md_text):
    # 提取标题
    title = extract_title(md_text)
    
    text, code_blocks = extract_code_blocks(md_text)
    text = process_bilibili(text)
    text = process_url_embed(text)
    text = process_containers(text)
    text = restore_code_blocks(text, code_blocks)
    body = markdown.markdown(text, extensions=['extra'])
    
    # 替换标题和内容
    return HTML_TEMPLATE.replace('{{TITLE}}', title).replace('{{BODY}}', body)

# 模板：左上角带返回按钮，标题动态注入
HTML_TEMPLATE = """<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>{{TITLE}}</title>
<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/katex.min.css">
<script defer src="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/katex.min.js"></script>
<script defer src="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/contrib/auto-render.min.js"
    onload="renderMathInElement(document.body,{delimiters:[{left:'$$',right:'$$',display:true},{left:'$',right:'$',display:false}]});"></script>
<style>
body{font-family:-apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,sans-serif;line-height:1.7;max-width:800px;margin:0 auto;padding:20px;color:#333;}
.back-btn{display:inline-block;margin-bottom:20px;padding:6px 12px;background:#f0f0f0;border-radius:4px;text-decoration:none;color:#333;font-size:14px;transition:0.2s;}
.back-btn:hover{background:#e0e0e0;}
h1,h2,h3,h4,h5,h6{margin-top:1.5em;margin-bottom:0.5em;}
pre{background:#f6f8fa;padding:16px;border-radius:6px;overflow-x:auto;}
code{background:#f0f0f0;padding:2px 6px;border-radius:3px;font-size:0.9em;}
pre code{background:none;padding:0;}
table{border-collapse:collapse;width:100%;}
th,td{border:1px solid #ddd;padding:8px 12px;text-align:left;}
th{background:#f6f8fa;}
blockquote{border-left:4px solid #ddd;margin:16px 0;padding:8px 16px;color:#666;}
img{max-width:100%;}
a{color:#0366d6;}
hr{border:none;border-top:1px solid #eee;margin:24px 0;}
details{margin:12px 0;}
details summary{cursor:pointer;user-select:none;}
.katex-display{overflow-x:auto;overflow-y:hidden;padding:4px 0;}
</style>
</head>
<body>
<a href="javascript:history.back()" class="back-btn">← 返回</a>
{{BODY}}
</body>
</html>"""

# ============================================================
#  5. 递归扫描 & 输出到 ./docs
# ============================================================
def process_directory(src_dir, dst_dir):
    count = 0
    for root, _, files in os.walk(src_dir):
        for f in files:
            if f.lower().endswith('.md'):
                src_path = os.path.join(root, f)
                rel_path = os.path.relpath(src_path, src_dir)
                dst_path = os.path.join(dst_dir, os.path.splitext(rel_path)[0] + '.html')
                
                os.makedirs(os.path.dirname(dst_path), exist_ok=True)
                
                with open(src_path, 'r', encoding='utf-8') as fh:
                    html = md2html(fh.read())
                with open(dst_path, 'w', encoding='utf-8') as fh:
                    fh.write(html)
                print(f'  ✓ {rel_path} → docs/{os.path.splitext(rel_path)[0]}.html')
                count += 1
    return count

if __name__ == '__main__':
    src = '.'
    dst = './docs'
    
    print(f'🔍 正在扫描: {os.path.abspath(src)}')
    print(f'📂 输出目录: {os.path.abspath(dst)}\n')
    
    total = process_directory(src, dst)
    print(f'\n🎉 完成！共转换 {total} 个文件到 {dst}')