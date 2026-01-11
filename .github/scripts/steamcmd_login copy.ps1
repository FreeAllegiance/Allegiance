param (
    [Parameter(Mandatory = $true)] [string]$SteamUser,
    [Parameter(Mandatory = $true)] [string]$SteamPass,
    [Parameter(Mandatory = $true)] [string]$Mailbox
)

$ErrorActionPreference = "Stop"

$CatchmailBoxApi = "https://api.catchmail.io/api/v1/mailbox?address=${Mailbox}"
$CatchmailMsgApi = "https://api.catchmail.io/api/v1/message"



# --- 1) Start SteamCMD (Initial Login) ---
# Steam uses UTC for its email timestamps, matching Catchmail
$StartTime = Get-Date ([DateTime]::UtcNow) -Format "yyyyMMddTHHmmss"
Write-Host ">>> Starting SteamCMD login attempt at $StartTime (UTC)"

# Execute SteamCMD with output monitoring
# UseShellExecute MUST be false to redirect streams.
# We usage async event handling to read output in real-time to avoid buffer deadlocks
# and to let the user see what is happening.

$pInfo = New-Object System.Diagnostics.ProcessStartInfo
$pInfo.FileName = "steamcmd.exe"
$pInfo.Arguments = "+login $SteamUser $SteamPass +quit"
$pInfo.RedirectStandardOutput = $true
$pInfo.RedirectStandardError = $true
$pInfo.RedirectStandardInput = $true
$pInfo.UseShellExecute = $false
$pInfo.CreateNoWindow = $true

$p = New-Object System.Diagnostics.Process
$p.StartInfo = $pInfo

$outputBuilder = New-Object System.Text.StringBuilder

# Handle Output Async
$outEvent = Register-ObjectEvent -InputObject $p -EventName OutputDataReceived -MessageData $outputBuilder -Action {
    if ($Event.SourceEventArgs.Data) {
        Write-Host $Event.SourceEventArgs.Data
        $Event.MessageData.AppendLine($Event.SourceEventArgs.Data) | Out-Null
    }
}

# Handle Error Async (merge into same log for checking)
$errEvent = Register-ObjectEvent -InputObject $p -EventName ErrorDataReceived -MessageData $outputBuilder -Action {
    if ($Event.SourceEventArgs.Data) {
        Write-Host $Event.SourceEventArgs.Data
        $Event.MessageData.AppendLine($Event.SourceEventArgs.Data) | Out-Null
    }
}

$p.Start() | Out-Null
$p.BeginOutputReadLine()
$p.BeginErrorReadLine()

# Wait 30 seconds to allow SteamCMD to update and trigger the email.
Write-Host ">>> Waiting 30s for SteamCMD to initialize/update/login..."
Start-Sleep -Seconds 30

# Close Stdin to force SteamCMD to exit (if it's waiting at the prompt)
$p.StandardInput.Close()

Write-Host ">>> Closing input and waiting for exit..."
# Wait for it to finish (90 seconds timeout for updates etc)
if (-not $p.WaitForExit(90000)) {
    try { $p.Kill() } catch {}
    throw "Timed out waiting for SteamCMD."
}

# Unregister events
Unregister-Event -SourceIdentifier $outEvent.Name
Unregister-Event -SourceIdentifier $errEvent.Name

$combinedOutput = $outputBuilder.ToString()

$guardDetected = $false
if ($combinedOutput -match "Steam Guard code:") {
    Write-Host "`n>>> Steam Guard prompt detected (via output log)."
    $guardDetected = $true
} 
elseif ($p.ExitCode -eq 0) {
    Write-Host "`n[OK] SteamCMD login succeeded (no Steam Guard required)."
    return
} 

if (-not $guardDetected) {
    throw "SteamCMD failed unexpectedly with exit code $($p.ExitCode)"
}

# --- 2) Poll Catchmail ---
Write-Host ">>> Waiting for Steam Guard email..."
$SteamGuardCode = $null
$MailId = $null

for ($i = 1; $i -le 10; $i++) {
    Write-Host "    Attempt $i..."
    
    $response = Invoke-RestMethod -Uri $CatchmailBoxApi -Method Get
    # Filter for messages with an ID (timestamp) greater than our start time
    $targetMsg = $response.messages | Where-Object { $_.id -gt $StartTime } | Select-Object -First 1

    if ($null -ne $targetMsg) {
        $MailId = $targetMsg.id
        Write-Host "    Found message: $MailId"

        $msgDetails = Invoke-RestMethod -Uri "${CatchmailMsgApi}/${MailId}?mailbox=${Mailbox}" -Method Get
        $body = $msgDetails.body.text

        # Using single quotes for regex pattern to avoid PowerShell parsing issues
        if ($body -match 'Login Code\s+([A-Z0-9]{5})') {
            $SteamGuardCode = $Matches[1]
            break
        }
    }
    Start-Sleep -Seconds 30
}

if ($null -eq $SteamGuardCode) {
    throw "Failed to retrieve Steam Guard code after 10 attempts."
}

Write-Host "[OK] Steam Guard code received: $SteamGuardCode"

# --- 3) Delete Catchmail message ---
Write-Host ">>> Deleting Steam Guard email..."
try {
    Invoke-RestMethod -Uri "${CatchmailMsgApi}/${MailId}?mailbox=${Mailbox}" -Method Delete
}
catch {
    Write-Host "[WARN] Failed to delete email (non-fatal)."
}

# --- 4) Retry SteamCMD with code ---
Write-Host ">>> Restarting SteamCMD with Steam Guard code..."

# Run steamcmd again with the code
# We can use direct execution here or the same process wrapper if needed.
# Since this one usually just works or fails, simple execution is okay,
# but using Process ensures we don't have quoting issues.
# But for simplicity, let's stick to the previous simple invocation for step 4
# OR upgrade it to process as well for consistency. 
# The original script used `& steamcmd ...`. Let's stick to that for now to minimize changes in step 4.

& steamcmd +set_steam_guard_code $SteamGuardCode +login $SteamUser $SteamPass +exit

if ($LASTEXITCODE -ne 0) { throw "SteamCMD failed during final authentication." }
Write-Host "[OK] SteamCMD authenticated successfully."
