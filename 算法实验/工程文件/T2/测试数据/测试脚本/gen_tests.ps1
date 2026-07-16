$outputDir = "test_cases"
New-Item -ItemType Directory -Path $outputDir -Force | Out-Null

# 单字符
@"
a
"@ | Out-File -FilePath "$outputDir\T1_single.txt" -Encoding ascii

# 全相同字符
@"
aaaaa
"@ | Out-File -FilePath "$outputDir\T2_all_same.txt" -Encoding ascii

# 无长回文（除了单字符）
@"
abcde
"@ | Out-File -FilePath "$outputDir\T3_no_pal.txt" -Encoding ascii

# 全回文
@"
abaaba
"@ | Out-File -FilePath "$outputDir\T4_full_pal.txt" -Encoding ascii

# 给定例子
@"
ababbbabbababa
"@ | Out-File -FilePath "$outputDir\T5_given.txt" -Encoding ascii

# 中等随机长度 100
$rng = New-Object System.Random
$len = 100
$s = -join (1..$len | ForEach-Object { [char]($rng.Next(97, 123)) })
$s | Out-File -FilePath "$outputDir\T6_random100.txt" -Encoding ascii
# 随机长度 200
$len = 200
$s = -join (1..$len | ForEach-Object { [char]($rng.Next(97, 123)) })
$s | Out-File -FilePath "$outputDir\T6a_random200.txt" -Encoding ascii

# 随机长度 300
$len = 300
$s = -join (1..$len | ForEach-Object { [char]($rng.Next(97, 123)) })
$s | Out-File -FilePath "$outputDir\T6b_random300.txt" -Encoding ascii

# 随机长度 500
$len = 500
$s = -join (1..$len | ForEach-Object { [char]($rng.Next(97, 123)) })
$s | Out-File -FilePath "$outputDir\T7_random500.txt" -Encoding ascii

# 随机长度 700
$len = 700
$s = -join (1..$len | ForEach-Object { [char]($rng.Next(97, 123)) })
$s | Out-File -FilePath "$outputDir\T7a_random700.txt" -Encoding ascii


# 随机长度 1000
$len = 1000
$s = -join (1..$len | ForEach-Object { [char]($rng.Next(97, 123)) })
$s | Out-File -FilePath "$outputDir\T8_random1000.txt" -Encoding ascii

# 随机长度 1500
$len = 1500
$s = -join (1..$len | ForEach-Object { [char]($rng.Next(97, 123)) })
$s | Out-File -FilePath "$outputDir\T8a_random1500.txt" -Encoding ascii


# 随机长度 2000
$len = 2000
$s = -join (1..$len | ForEach-Object { [char]($rng.Next(97, 123)) })
$s | Out-File -FilePath "$outputDir\T9_random2000.txt" -Encoding ascii

# 随机长度 2500 （挑战大一点）
$len = 2500
$s = -join (1..$len | ForEach-Object { [char]($rng.Next(97, 123)) })
$s | Out-File -FilePath "$outputDir\T9a_random2500.txt" -Encoding ascii

Write-Host "Test inputs generated."