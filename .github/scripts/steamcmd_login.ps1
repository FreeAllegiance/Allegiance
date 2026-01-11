param (
    [Parameter(Mandatory = $true)] [string]$SteamUser,
    [Parameter(Mandatory = $true)] [string]$SteamPass,
    [Parameter(Mandatory = $true)] [string]$Mailbox
)

# Configuration
$steamCmdPath = "steamcmd"
$email = $Mailbox
$mailboxUrl = "https://api.catchmail.io/api/v1/mailbox?address=$email"

Write-Host "Getting SteamCMD installation path..." -ForegroundColor Cyan

# Get SteamCMD install path and log directory
$infoOutput = & $steamCmdPath +info +quit 2>&1 | Out-String
if ($infoOutput -match "InstallPath:\s*(.+)") {
    $installPath = $matches[1].Trim()
    $logFile = Join-Path $installPath "logs\console_log.txt"
    Write-Host "Log file location: $logFile" -ForegroundColor Gray
}
else {
    Write-Host "Could not determine SteamCMD install path. Using default..." -ForegroundColor Yellow
    $logFile = "c:\steamcmd\logs\console_log.txt"
}

# Verify log file exists or can be created
$logDir = Split-Path $logFile -Parent
if (!(Test-Path $logDir)) {
    Write-Host "Log directory does not exist: $logDir" -ForegroundColor Red
    exit 1
}

# Record start time
$startTime = Get-Date
Write-Host "Script started at: $startTime" -ForegroundColor Cyan

# Clear or note the current log file size to only read new content
$initialLogSize = 0
if (Test-Path $logFile) {
    $initialLogSize = (Get-Item $logFile).Length
    Write-Host "Current log file size: $initialLogSize bytes" -ForegroundColor Gray
}

Write-Host "`nStarting SteamCMD login process..." -ForegroundColor Cyan

# Start SteamCMD process with UseShellExecute
$psi = New-Object System.Diagnostics.ProcessStartInfo
$psi.FileName = $steamCmdPath
$psi.Arguments = "+login $SteamUser $SteamPass +quit"
$psi.UseShellExecute = $true
$psi.WindowStyle = [System.Diagnostics.ProcessWindowStyle]::Normal

$process = [System.Diagnostics.Process]::Start($psi)

# Monitor the log file for Steam Guard prompt
$steamGuardRequested = $false
$maxWaitTime = 60
$elapsed = 0

while (!$process.HasExited -and $elapsed -lt $maxWaitTime) {
    Start-Sleep -Seconds 2
    $elapsed += 2
    
    if (Test-Path $logFile) {
        # Read new content from log file
        $currentSize = (Get-Item $logFile).Length
        
        if ($currentSize -gt $initialLogSize) {
            # Read only the new content
            $fileStream = [System.IO.File]::Open($logFile, [System.IO.FileMode]::Open, [System.IO.FileAccess]::Read, [System.IO.FileShare]::ReadWrite)
            $fileStream.Seek($initialLogSize, [System.IO.SeekOrigin]::Begin) | Out-Null
            $reader = New-Object System.IO.StreamReader($fileStream)
            $newContent = $reader.ReadToEnd()
            $reader.Close()
            $fileStream.Close()
            
            # Display new content
            if ($newContent) {
                Write-Host $newContent -NoNewline
            }
            
            # Check for Steam Guard prompt in new content
            if ($newContent -match "Steam Guard code:") {
                Write-Host "`n`nSteam Guard code detected in log. Terminating SteamCMD..." -ForegroundColor Yellow
                $steamGuardRequested = $true
                
                # Kill SteamCMD process
                if (!$process.HasExited) {
                    $process.Kill()
                    $process.WaitForExit(5000)
                }
                break
            }
            
            $initialLogSize = $currentSize
        }
    }
}

$process.Dispose()

if (!$steamGuardRequested) {
    Write-Host "SteamCMD exited without requesting Steam Guard code." -ForegroundColor Red
    exit 1
}

# Wait a moment for the email to arrive
Write-Host "Waiting for Steam Guard email..." -ForegroundColor Cyan
Start-Sleep -Seconds 5

# Fetch mailbox
Write-Host "Checking mailbox at $mailboxUrl" -ForegroundColor Cyan
try {
    $mailboxResponse = Invoke-RestMethod -Uri $mailboxUrl -Method Get
    
    # Find emails received after start time
    $recentEmails = $mailboxResponse | Where-Object {
        if ($_.timestamp) {
            $emailTime = [DateTime]::Parse($_.timestamp)
            $emailTime -gt $startTime
        }
    }
    
    if ($recentEmails.Count -eq 0) {
        Write-Host "No recent emails found. Checking all emails..." -ForegroundColor Yellow
        $recentEmails = $mailboxResponse | Select-Object -First 5
    }
    
    # Try each recent email to find the Steam Guard code
    $steamGuardCode = $null
    foreach ($email in $recentEmails) {
        $messageId = $email.id
        $messageUrl = "https://api.catchmail.io/api/v1/message/${messageId}?mailbox=$Mailbox"
        
        Write-Host "Checking message: $messageId" -ForegroundColor Gray
        
        try {
            $messageResponse = Invoke-RestMethod -Uri $messageUrl -Method Get
            $body = $messageResponse.body_text
            
            # Look for Login Code pattern
            if ($body -match "Login Code[\\n\s]+([A-Z0-9]{5})") {
                $steamGuardCode = $matches[1]
                Write-Host "Found Steam Guard code: $steamGuardCode" -ForegroundColor Green
                break
            }
        }
        catch {
            Write-Host "Error reading message $messageId : $_" -ForegroundColor Red
        }
    }
    
    if (!$steamGuardCode) {
        Write-Host "Could not find Steam Guard code in recent emails." -ForegroundColor Red
        exit 1
    }
    
    # Restart SteamCMD with the Steam Guard code
    Write-Host "`nRestarting SteamCMD with Steam Guard code..." -ForegroundColor Cyan
    $finalCommand = "+set_steam_guard_code $steamGuardCode +login $SteamUser $SteamPass +quit"
    
    Start-Process -FilePath $steamCmdPath -ArgumentList $finalCommand -NoNewWindow -Wait
    
    Write-Host "`nSteamCMD login process completed!" -ForegroundColor Green
}
catch {
    Write-Host "Error accessing mailbox: $_" -ForegroundColor Red
    exit 1
}