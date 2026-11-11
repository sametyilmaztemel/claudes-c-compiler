#!/usr/bin/env bash
# Verify authorship of commits newly introduced in <base>..<head>.
#
# Usage: ci/check-attribution.sh <base-ref> <head-ref> [exempt-ref...]
# Exit:  0 ok, 1 violation(s), 2 usage/ref error.
#
# Every new commit must be authored and committed by the fork owner and must
# not carry AI attribution trailers. Commits reachable from an exempt ref are
# not checked: pass the upstream branch so that upstream history arriving in a
# sync is never flagged, or a contributor's branch to keep third-party
# authorship intact. Commits created on GitHub (committer "GitHub") are
# reported separately; their author still has to be the owner.
set -euo pipefail

OWNER_NAME="${OWNER_NAME:-sametyilmaztemel}"
OWNER_EMAIL="${OWNER_EMAIL:-115784698+sametyilmaztemel@users.noreply.github.com}"
GH_NAME="GitHub"
GH_EMAIL="noreply@github.com"

if [ "$#" -lt 2 ]; then
  echo "usage: $0 <base-ref> <head-ref> [exempt-ref...]" >&2
  exit 2
fi
base="$1"
head="$2"
shift 2
exempt=("$@")

for r in "$base" "$head" ${exempt[@]+"${exempt[@]}"}; do
  if ! git rev-parse --verify --quiet "${r}^{commit}" >/dev/null; then
    echo "error: cannot resolve ref '$r' to a commit" >&2
    exit 2
  fi
done

owner="$OWNER_NAME <$OWNER_EMAIL>"
ghid="$GH_NAME <$GH_EMAIL>"

# Only commits reachable from head and not from base or an exempt ref.
mapfile -t shas < <(git rev-list "$head" --not "$base" ${exempt[@]+"${exempt[@]}"})
if [ "${#shas[@]}" -eq 0 ]; then
  echo "No new commits in ${base}..${head}; nothing to check."
  exit 0
fi

violations=0
for sha in "${shas[@]}"; do
  # Fields separated by US (0x1f): author name, author email, committer name,
  # committer email.
  IFS=$'\x1f' read -r an ae cn ce < <(git log -n1 --format='%an%x1f%ae%x1f%cn%x1f%ce' "$sha")
  author="$an <$ae>"
  committer="$cn <$ce>"
  reasons=()
  class=""

  if [ "$committer" = "$owner" ]; then
    class="owner"
    [ "$author" = "$owner" ] || reasons+=("author is not owner")
  elif [ "$committer" = "$ghid" ]; then
    # Created on GitHub (merge button, squash, web editor). The author must
    # still be the owner identity; this also catches a merge recorded with
    # another e-mail address of the same account.
    class="github-generated"
    [ "$author" = "$owner" ] || reasons+=("author is not owner (commit created on GitHub)")
  else
    reasons+=("committer is not owner or GitHub")
    [ "$author" = "$owner" ] || reasons+=("author is not owner")
  fi

  # Trailer check on the raw message body, case-insensitive.
  message=$(git log -n1 --format='%B' "$sha")
  if grep -qiE '^[[:space:]]*co-authored-by[[:space:]]*:' <<<"$message"; then
    reasons+=("Co-Authored-By trailer present")
  fi
  if grep -qiE '^[[:space:]]*claude-session[[:space:]]*:' <<<"$message"; then
    reasons+=("Claude-Session trailer present")
  fi

  if [ "${#reasons[@]}" -gt 0 ]; then
    joined=$(IFS=';'; echo "${reasons[*]}")
    class="VIOLATION: ${joined//;/; }"
    violations=$((violations + 1))
  fi
  printf '%s | author: %s | committer: %s | %s\n' "$sha" "$author" "$committer" "$class"
done

if [ "$violations" -gt 0 ]; then
  echo "FAILED: $violations violation(s) in ${#shas[@]} commit(s)." >&2
  exit 1
fi
echo "OK: ${#shas[@]} commit(s) checked, no violations."
