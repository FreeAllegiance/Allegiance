#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 3 ]]; then
  echo "Usage: $0 <steam_username> <steam_password> <catchmail_email>"
  exit 1
fi

STEAM_USER="$1"
STEAM_PASS="$2"
MAILBOX="$3"

CATCHMAIL_BOX_API="https://api.catchmail.io/api/v1/mailbox?address=${MAILBOX}"
CATCHMAIL_MSG_API="https://api.catchmail.io/api/v1/message"

LOCK_DIR="/tmp/steamcmd-login.lock"

########################################
# 0) Multi-runner lock
########################################

echo "▶ Acquiring SteamCMD login lock..."

until mkdir "$LOCK_DIR" 2>/dev/null; do
  echo "  Another runner is logging into Steam — waiting..."
  sleep 5
done

cleanup() {
  rm -rf "$LOCK_DIR"
}
trap cleanup EXIT

########################################
# 1) Start SteamCMD (expect)
########################################

START_TIME=$(date -u +"%Y%m%dT%H%M%S")
EXPECT_LOG=$(mktemp)

echo "▶ Starting SteamCMD login at $START_TIME"

expect <<EOF | tee "$EXPECT_LOG"
log_user 1
set timeout 300

spawn steamcmd +login $STEAM_USER $STEAM_PASS +exit

expect {
  "Steam Guard code:" {
    puts "Steam Guard prompt detected"
    exit 100
  }
  eof {
    puts "SteamCMD exited normally"
    exit 0
  }
}
EOF

EXPECT_EXIT=$?

if [[ $EXPECT_EXIT -eq 0 ]]; then
  echo "✔ SteamCMD login succeeded (no Steam Guard required)"
  exit 0
elif [[ $EXPECT_EXIT -ne 100 ]]; then
  echo "✖ SteamCMD failed unexpectedly"
  exit 1
fi

########################################
# 2) Poll Catchmail
########################################

echo "▶ Waiting for Steam Guard email..."

STEAM_GUARD_CODE=""
MAIL_ID=""

for i in {1..10}; do
  echo "  Attempt $i..."

  MAIL_ID=$(curl -fsS "$CATCHMAIL_BOX_API" \
    | jq -r --arg START "$START_TIME" '
        .messages[]
        | select(.id > $START)
        | .id
      ' | head -n 1)

  if [[ -n "$MAIL_ID" ]]; then
    echo "  Found message: $MAIL_ID"

    MAIL_BODY=$(curl -fsS "${CATCHMAIL_MSG_API}/${MAIL_ID}?mailbox=${MAILBOX}" \
      | jq -r '.body.text')

    STEAM_GUARD_CODE=$(echo "$MAIL_BODY" \
      | sed -n 's/^Login Code[[:space:]]*\([A-Z0-9]\{5\}\).*$/\1/p')

    [[ -n "$STEAM_GUARD_CODE" ]] && break
  fi

  sleep 6
done

if [[ -z "$STEAM_GUARD_CODE" ]]; then
  echo "✖ Failed to retrieve Steam Guard code"
  exit 1
fi

echo "✔ Steam Guard code received: $STEAM_GUARD_CODE"

########################################
# 3) Delete Catchmail message
########################################

echo "▶ Deleting Steam Guard email..."

curl -fsS -X DELETE \
  "${CATCHMAIL_MSG_API}/${MAIL_ID}?mailbox=${MAILBOX}" \
  >/dev/null || echo "⚠ Failed to delete email (non-fatal)"

########################################
# 4) Retry SteamCMD with Steam Guard code
########################################

echo "▶ Restarting SteamCMD with Steam Guard code"

expect <<EOF
log_user 1
set timeout 300

spawn steamcmd \
  +set_steam_guard_code $STEAM_GUARD_CODE \
  +login $STEAM_USER $STEAM_PASS \
  +exit

expect {
  eof { exit 0 }
}
EOF

echo "✔ SteamCMD authenticated successfully"
