$exe = ".\ConsoleApplication1.exe"  
$testDir = "test_cases"
$outCsv = "results.csv"

"Test,Mode,K,N,Time_ms" | Out-File -FilePath $outCsv

Get-ChildItem -Path $testDir -Filter *.txt | Sort-Object Name | ForEach-Object {
    $testName = $_.BaseName
    $filePath = $_.FullName

    foreach ($mode in @("divide", "heap")) {
        Write-Host "Testing $testName $mode" -ForegroundColor Cyan
        $output = Get-Content $filePath | & $exe $mode 2>&1

        $timeMatch = [regex]::Match($output, "Time: ([\d\.]+) ms")
        $totalMatch = [regex]::Match($output, "Total elements: (\d+)")
        $firstLine = Get-Content $filePath -First 1
        $K = [int]$firstLine

        if ($timeMatch.Success -and $totalMatch.Success) {
            $time = $timeMatch.Groups[1].Value
            $N = $totalMatch.Groups[1].Value
            "$testName,$mode,$K,$N,$time" | Add-Content -Path $outCsv
            Write-Host "  OK: time=$time ms, elements=$N"
        } else {
            Write-Warning "  FAILED to parse output for $testName $mode"
            Write-Host "  Output was: $output"
        }
    }
}

Write-Host "All tests completed! Results saved to $outCsv" -ForegroundColor Green