$env:PATH += ";C:\Users\pprav\AppData\Local\Programs\MiKTeX\miktex\bin\x64"
Set-Location $PSScriptRoot

Write-Host "Pass 1: pdflatex..." -ForegroundColor Cyan
pdflatex -interaction=nonstopmode main.tex | Select-String "^!" | ForEach-Object { Write-Host $_ -ForegroundColor Red }

Write-Host "Running bibtex..." -ForegroundColor Cyan
bibtex main

Write-Host "Pass 2: pdflatex..." -ForegroundColor Cyan
pdflatex -interaction=nonstopmode main.tex | Select-String "^!" | ForEach-Object { Write-Host $_ -ForegroundColor Red }

Write-Host "Pass 3: pdflatex..." -ForegroundColor Cyan
pdflatex -interaction=nonstopmode main.tex | Select-String "^!" | ForEach-Object { Write-Host $_ -ForegroundColor Red }

if (Test-Path "main.pdf") { Write-Host "Done! main.pdf generated successfully." -ForegroundColor Green }
else { Write-Host "Build failed - main.pdf not found." -ForegroundColor Red }
