$outputDir = "test_cases"
New-Item -ItemType Directory -Path $outputDir -Force | Out-Null

# === 基础测试 ===
@"
1
5
10 20 30 40 50
"@ | Out-File -FilePath "$outputDir\T1_K1_normal.txt" -Encoding ascii

@"
1
1
42
"@ | Out-File -FilePath "$outputDir\T2_K1_single.txt" -Encoding ascii

@"
1
0
"@ | Out-File -FilePath "$outputDir\T3_K1_empty.txt" -Encoding ascii

@"
3
3
1 2 3
0
2
8 9
"@ | Out-File -FilePath "$outputDir\T4_K3_with_empty.txt" -Encoding ascii

@"
2
0
4
5 6 7 8
"@ | Out-File -FilePath "$outputDir\T5_K2_one_empty.txt" -Encoding ascii

@"
3
0
0
0
"@ | Out-File -FilePath "$outputDir\T6_all_empty.txt" -Encoding ascii

@"
4
2
1 100
5
2 3 4 5 6
1
0
3
50 60 70
"@ | Out-File -FilePath "$outputDir\T7_K4_various_lengths.txt" -Encoding ascii

@"
2
4
1 2 2 3
3
2 3 4
"@ | Out-File -FilePath "$outputDir\T8_K2_duplicates.txt" -Encoding ascii

# === 中等随机 ===
$rng = New-Object System.Random
$K = 10
$lines = @("$K")
for ($i=0; $i -lt $K; $i++) {
    $len = $rng.Next(1, 101)
    $arr = (1..$len | ForEach-Object { $rng.Next(0, 1000) }) | Sort-Object
    $lines += "$len"
    $lines += ($arr -join ' ')
}
$lines -join "`r`n" | Out-File -FilePath "$outputDir\M1_K10_random.txt" -Encoding ascii

# === 大规模 L1: K=10, N=100,000 ===
$K = 10; $total = 100000; $per = [math]::Round($total / $K)
$lines = @("$K")
for ($i=0; $i -lt $K; $i++) {
    $len = if ($i -eq $K-1) { $total - ($per * ($K-1)) } else { $per }
    $arr = (1..$len | ForEach-Object { $rng.Next(0, 1000000) }) | Sort-Object
    $lines += "$len"
    $lines += ($arr -join ' ')
}
$lines -join "`r`n" | Out-File -FilePath "$outputDir\L1_N100K_K10.txt" -Encoding ascii

# L2: K=100, N=100,000
$K = 100; $total = 100000; $per = [math]::Round($total / $K)
$lines = @("$K")
for ($i=0; $i -lt $K; $i++) {
    $len = if ($i -eq $K-1) { $total - ($per * ($K-1)) } else { $per }
    $arr = (1..$len | ForEach-Object { $rng.Next(0, 1000000) }) | Sort-Object
    $lines += "$len"
    $lines += ($arr -join ' ')
}
$lines -join "`r`n" | Out-File -FilePath "$outputDir\L2_N100K_K100.txt" -Encoding ascii

# L3: K=1000, N=100,000
$K = 1000; $total = 100000; $per = [math]::Round($total / $K)
$lines = @("$K")
for ($i=0; $i -lt $K; $i++) {
    $len = if ($i -eq $K-1) { $total - ($per * ($K-1)) } else { $per }
    $arr = (1..$len | ForEach-Object { $rng.Next(0, 1000000) }) | Sort-Object
    $lines += "$len"
    $lines += ($arr -join ' ')
}
$lines -join "`r`n" | Out-File -FilePath "$outputDir\L3_N100K_K1000.txt" -Encoding ascii

# L4: K=10, N=1,000,000
$K = 10; $total = 1000000; $per = [math]::Round($total / $K)
$lines = @("$K")
for ($i=0; $i -lt $K; $i++) {
    $len = if ($i -eq $K-1) { $total - ($per * ($K-1)) } else { $per }
    $arr = (1..$len | ForEach-Object { $rng.Next(0, 1000000) }) | Sort-Object
    $lines += "$len"
    $lines += ($arr -join ' ')
}
$lines -join "`r`n" | Out-File -FilePath "$outputDir\L4_N1M_K10.txt" -Encoding ascii

# L5: K=100, N=1,000,000
$K = 100; $total = 1000000; $per = [math]::Round($total / $K)
$lines = @("$K")
for ($i=0; $i -lt $K; $i++) {
    $len = if ($i -eq $K-1) { $total - ($per * ($K-1)) } else { $per }
    $arr = (1..$len | ForEach-Object { $rng.Next(0, 1000000) }) | Sort-Object
    $lines += "$len"
    $lines += ($arr -join ' ')
}
$lines -join "`r`n" | Out-File -FilePath "$outputDir\L5_N1M_K100.txt" -Encoding ascii

Write-Host "Test cases generated in $outputDir" -ForegroundColor Green