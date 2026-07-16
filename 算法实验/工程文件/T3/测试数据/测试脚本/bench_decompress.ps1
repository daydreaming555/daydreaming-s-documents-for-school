param(
    [string]$tool,
    [string]$compressedFile,
    [string]$outputFile,
    [int]$runs = 10,
    [string]$resultCSV = "decompress_results.csv"
)

if ($tool -eq "huff") {
    $cmd = ".\huffman_3.exe"
    $args = @("decompress", "`"$compressedFile`"", "`"$outputFile`"")
} elseif ($tool -eq "7z") {
    $cmd = "D:\tool\class\7zip\7-Zip\7z.exe"
    $args = @("x", "-y", "-o$outputFile", "`"$compressedFile`"")
    # 7z 的解压输出目录需要预先存在，我们在每次循环中创建并清理
} elseif ($tool -eq "gzip") {
    $cmd = "D:\tool\class\7zip\7-Zip\7z.exe"
    $args = @("x", "-y", "-o$outputFile", "`"$compressedFile`"")
} else {
    Write-Error "Unknown tool"
    exit
}

$timings = @()
for ($i = 1; $i -le $runs; $i++) {
    # 清理输出目录
    if (Test-Path $outputFile) { Remove-Item $outputFile -Recurse -Force }
    New-Item -ItemType Directory -Path $outputFile | Out-Null

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
    Start-Sleep -Milliseconds 500
}

$line = "$tool,$compressedFile,$($timings -join ',')"
Add-Content -Path $resultCSV -Value $line
Write-Host "Results appended to $resultCSV"
