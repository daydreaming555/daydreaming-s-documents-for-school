param(
    [string]$tool,        # "huff", "7z", "gzip"
    [string]$inputFile,
    [string]$outputFile,
    [int]$runs = 10,
    [string]$resultCSV = "compress_results.csv"
)

# 根据工具选择命令
if ($tool -eq "huff") {
    $cmd = ".\huffman_3.exe"
    $args = @("compress", "`"$inputFile`"", "`"$outputFile`"")
} elseif ($tool -eq "7z") {
    $cmd = "D:\tool\class\7zip\7-Zip\7z.exe"
    $args = @("a", "-mx=1", "`"$outputFile`"", "`"$inputFile`"")
} elseif ($tool -eq "gzip") {
    $cmd = "D:\tool\class\7zip\7-Zip\7z.exe"
    $args = @("a", "-tgzip", "-mx=1", "`"$outputFile`"", "`"$inputFile`"")
} else {
    Write-Error "Unknown tool"
    exit
}

$timings = @()
for ($i = 1; $i -le $runs; $i++) {
    # 删除上次输出文件，确保每次独立
    if (Test-Path $outputFile) { Remove-Item $outputFile -Force }
    # 刷新磁盘缓存
    $sw = [System.Diagnostics.Stopwatch]::StartNew()
    $proc = Start-Process -FilePath $cmd -ArgumentList $args -NoNewWindow -Wait -PassThru
    $sw.Stop()
    if ($proc.ExitCode -ne 0) {
        Write-Warning "Run $i failed with exit code $($proc.ExitCode)"
        continue
    }
    $elapsedMs = $sw.Elapsed.TotalMilliseconds
    Write-Host "Tool=$tool, Run=$i, Time=$elapsedMs ms"
    $timings += $elapsedMs
    # 等待 0.5 秒避免连续 I/O 影响
    Start-Sleep -Milliseconds 500
}

# 保存结果到 CSV
$line = "$tool,$inputFile,$($timings -join ',')"
Add-Content -Path $resultCSV -Value $line
Write-Host "Results appended to $resultCSV"
