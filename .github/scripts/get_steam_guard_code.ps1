param (
    [Parameter(Mandatory = $true)] [string]$Mailbox
)

$ErrorActionPreference = "Stop"

# Configuration
$CatchmailBoxApi = "https://api.catchmail.io/api/v1/mailbox?address=${Mailbox}"
$CatchmailMsgApi = "https://api.catchmail.io/api/v1/message"

# Start looking for emails that arrived very recently (start time - 1 minute)
# We format as a string (yyyyMMddTHHmmss) to match Catchmail IDs for simple lexicographical comparison
$StartTimeStr = (Get-Date ([DateTime]::UtcNow).AddMinutes(-1) -Format "yyyyMMddTHHmmss")
$MaxWaitSeconds = 60
$PollInterval = 5
$TimeoutDate = (Get-Date).AddSeconds($MaxWaitSeconds)

Write-Host "Monitoring mailbox $Mailbox for Steam Guard code..." -ForegroundColor Cyan
Write-Host "Looking for emails with ID > $StartTimeStr" -ForegroundColor Gray

while ((Get-Date) -lt $TimeoutDate) {
    try {
        $response = Invoke-RestMethod -Uri $CatchmailBoxApi -Method Get
        
        # Filter for messages newer than StartTime using string comparison
        $messages = if ($response.messages) { $response.messages } else { $response }
        
        $targetMsgs = $messages | Where-Object { 
            $id = $_.id
            if ([string]::IsNullOrWhiteSpace($id)) { return $false }
            
            # Simple string verify it looks like a timestamp (digits and T)
            # Note: IDs often have suffixes like -1234, so we allow trailing chars.
            if ($id -notmatch '^\d{8}T\d{6}') { 
                Write-Host "Skipping non-conforming ID: $id" -ForegroundColor DarkGray
                return $false 
            }
            
            return $id -gt $StartTimeStr
        }

        foreach ($msg in $targetMsgs) {
            $msgId = $msg.id
            $msgUrl = "${CatchmailMsgApi}/${msgId}?mailbox=${Mailbox}"
            
            Write-Host "Checking message $msgId..." -ForegroundColor DarkGray
            
            $msgDetails = Invoke-RestMethod -Uri $msgUrl -Method Get
            
            # extract body, handling potential API structure variations
            $textBody = if ($msgDetails.body_text) { $msgDetails.body_text } 
            elseif ($msgDetails.body.text) { $msgDetails.body.text }
            else { $msgDetails | Out-String }

            # Regex for Steam Guard Code (5 chars, alphanumeric)
            if ($textBody -match 'Login Code\s+([A-Z0-9]{5})') {
                $SteamGuardCode = $Matches[1]
                Write-Host "Found Steam Guard code!" -ForegroundColor Green
                
                # Delete the message
                Write-Host "Deleting message $msgId..." -ForegroundColor Gray
                try {
                    Invoke-RestMethod -Uri $msgUrl -Method Delete
                }
                catch {
                    Write-Warning "Failed to delete message: $_"
                }
                
                # Output ONLY the code to standard output for capture
                Write-Output $SteamGuardCode
                exit 0
            }
        }
    }
    catch {
        Write-Warning "Transient error checking mailbox: $_"
    }

    Start-Sleep -Seconds $PollInterval
}

Write-Error "Timeout reached. Steam Guard code not found within $MaxWaitSeconds seconds."
exit 1
