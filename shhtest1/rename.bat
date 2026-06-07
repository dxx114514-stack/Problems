@echo off
setlocal enabledelayedexpansion

echo 正在查找并修改 .txt 文件扩展名为 .md ...

:: /r 表示递归遍历当前目录及子目录
for /r %%f in (*.txt) do (
    echo 正在处理: "%%f"
    :: 将扩展名替换为 .md
    ren "%%f" "*.md"
)

echo.
echo 全部修改完成！
pause