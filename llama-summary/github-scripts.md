# .github/scripts

[Back to the index](../LLAMA_SUMMARY.md) - 1 files, 83 lines. Notes by a local model, to orient by: not verified facts.

This folder contains scripts used by the Allegiance project, specifically focusing on automating interactions with external services to enhance game functionality.

- get_steam_guard_code.ps1: Monitors a specified email address for Steam Guard codes sent by Catchmail.io and extracts the first valid code found, aiding in automated login processes.

## `.github/scripts/get_steam_guard_code.ps1`

PURPOSE: This PowerShell script monitors a specified email address for Steam Guard codes sent by Catchmail.io and extracts the first valid code found.

KEY PARTS:
- `$CatchmailBoxApi`: URL to check for new emails in the mailbox.
- `$CatchmailMsgApi`: URL to fetch details of an individual email message.
- `$StartTimeStr`: Timestamp used to filter emails received within the last minute.
- `$MaxWaitSeconds` and `$PollInterval`: Controls how long the script waits for a Steam Guard code and how often it checks for new emails.
- `Invoke-RestMethod`: Used to make HTTP requests to Catchmail.io APIs.
- Regex pattern: Matches the Steam Guard code format (5 characters, alphanumeric).

CONNECTS TO:
- Catchmail.io email service for retrieving Steam Guard codes.

NOTES:
- The script outputs only the found Steam Guard code to standard output.
- It deletes the message containing the code after extracting it.
- Error handling is in place for transient issues and timeouts.
