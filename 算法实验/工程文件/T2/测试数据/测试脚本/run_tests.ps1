$exe = ".\PalindromePartition.exe"
$testDir = "test_cases"
$outCsv = "results.csv"

"Test,n,Output,Time_ms" | Out-File -FilePath $outCsv

Get-ChildItem -Path $testDir -Filter *.txt | Sort-Object Name | ForEach-Object {
    $testName = $_.BaseName
    $filePath = $_.FullName
    $inputStr = Get-Content $filePath -Raw
    # 执行程序，传入字符串
    $output = $inputStr | & $exe 2>&1
    # 解析标准输出第一行是结果
    $lines = $output -split "`r?`n"
    $res = $lines[0]
    $timeMatch = [regex]::Match($output, "Time: ([\d\.]+) ms")
    if ($timeMatch.Success) {
        $time = $timeMatch.Groups[1].Value
        "$testName,$($inputStr.Length),$res,$time" | Add-Content -Path $outCsv
        Write-Host "$testName : n=$($inputStr.Length) cuts=$res time=$time ms"
    } else {
        Write-Warning "Failed for $testName"
    }
}

Write-Host "Done. Output in $outCsv"