#!/usr/bin/env bash

# Build everything and boot in QEMU (without debug logging).
# Pass extra args to QEMU by appending them: ./run.sh -serial stdio

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

exec ./build.sh run "$@"
