Set-Location $PSScriptRoot

Write-Host "Pass 1: pdflatex..."
pdflatex -interaction=nonstopmode main.tex

Write-Host "Running bibtex..."
bibtex main

Write-Host "Pass 2: pdflatex..."
pdflatex -interaction=nonstopmode main.tex

Write-Host "Pass 3: pdflatex..."
pdflatex -interaction=nonstopmode main.tex

if (Test-Path "main.pdf") {
    Write-Host "Done! main.pdf generated successfully."
} else {
    Write-Host "Compilation failed. Check main.log for details."
}
