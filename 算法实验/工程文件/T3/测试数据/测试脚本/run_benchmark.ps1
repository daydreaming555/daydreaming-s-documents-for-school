# run_benchmark.ps1
$files = @("novel.txt", "binary.exe", "zero.bin")
$tools = @("huff", "7z", "gzip")
$compressLog = "compress_results.csv"
$decompressLog = "decompress_results.csv"

# 清理旧日志
if (Test-Path $compressLog) { Remove-Item $compressLog }
if (Test-Path $decompressLog) { Remove-Item $decompressLog }

foreach ($file in $files) {
    foreach ($tool in $tools) {
        # 确定输出文件名
        if ($tool -eq "huff") { $output = "$file.huff" }
        elseif ($tool -eq "7z") { $output = "$file.7z" }
        else { $output = "$file.gz" }

        Write-Host "=== Compress: $tool $file -> $output ===" -ForegroundColor Cyan
        .\bench_compress.ps1 -tool $tool -inputFile $file -outputFile $output -runs 10 -resultCSV $compressLog
    }
}

foreach ($file in $files) {
    foreach ($tool in $tools) {
        # 确定压缩文件和还原目录
        if ($tool -eq "huff") { $compressed = "$file.huff"; $restoreDir = "huff_restore" }
        elseif ($tool -eq "7z") { $compressed = "$file.7z"; $restoreDir = "7z_restore" }
        else { $compressed = "$file.gz"; $restoreDir = "gz_restore" }

        Write-Host "=== Decompress: $tool $compressed -> $restoreDir ===" -ForegroundColor Cyan
        .\bench_decompress.ps1 -tool $tool -compressedFile $compressed -outputFile $restoreDir -runs 10 -resultCSV $decompressLog
    }
}

Write-Host "All benchmarks completed!" -ForegroundColor Green