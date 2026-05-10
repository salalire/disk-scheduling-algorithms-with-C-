# ============================================================
#  test_api.ps1  —  Quick API Test Script
#
#  TEAM NOTES:
#  Run this PowerShell script AFTER starting server.exe
#  to verify all 3 endpoints are working correctly.
#
#  How to run:
#    1. Start the server:  .\server.exe
#    2. Open a new terminal
#    3. Run this script:   .\test_api.ps1
# ============================================================

$base = "http://localhost:8080"

# Sample input used for all tests
$body = @{
    disk_size = 200
    head      = 50
    direction = "right"
    requests  = @(98, 183, 41, 122, 14, 124, 65, 67)
} | ConvertTo-Json

Write-Host "`n===== Testing GET / (Health Check) =====" -ForegroundColor Cyan
Invoke-RestMethod -Uri "$base/" -Method GET | ConvertTo-Json

Write-Host "`n===== Testing POST /scan =====" -ForegroundColor Cyan
Invoke-RestMethod -Uri "$base/scan" -Method POST -Body $body -ContentType "application/json" | ConvertTo-Json -Depth 5

Write-Host "`n===== Testing POST /cscan =====" -ForegroundColor Cyan
Invoke-RestMethod -Uri "$base/cscan" -Method POST -Body $body -ContentType "application/json" | ConvertTo-Json -Depth 5

Write-Host "`n===== Testing POST /compare =====" -ForegroundColor Cyan
Invoke-RestMethod -Uri "$base/compare" -Method POST -Body $body -ContentType "application/json" | ConvertTo-Json -Depth 5

Write-Host "`nAll tests done!" -ForegroundColor Green
