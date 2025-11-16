#!/usr/bin/env bash
set -euo pipefail

# run-clang-tidy.sh
# Usage: ./scripts/run-clang-tidy.sh [build-dir]
# Default build-dir: build

BUILD_DIR=${1:-build}
NPROCS=1
if command -v nproc >/dev/null 2>&1; then
  NPROCS=$(nproc)
elif [[ "$(uname)" == "Darwin" ]] && command -v sysctl >/dev/null 2>&1; then
  NPROCS=$(sysctl -n hw.ncpu)
fi

echo "[run-clang-tidy] build dir: ${BUILD_DIR}, parallel: ${NPROCS}"

check_cmd() {
  if ! command -v "$1" >/dev/null 2>&1; then
    echo "ERROR: required command '$1' not found."
    return 1
  fi
}

missing=0
for cmd in cmake clang clang-tidy clang-tidy-html; do
  if ! check_cmd "$cmd"; then
    missing=1
  fi
done

if [ "$missing" -ne 0 ]; then
  echo
  echo "Please install the missing tools. Examples:" 
  echo "  Ubuntu: sudo apt-get install -y cmake clang clang-tidy libclang-dev python3-pip && pip3 install clang-tidy-html"
  echo "  macOS (Homebrew): brew install llvm cmake && pip3 install clang-tidy-html" 
  exit 2
fi

echo "[run-clang-tidy] Configuring project (export compile commands)..."
mkdir -p "$BUILD_DIR"
cmake -S . -B "$BUILD_DIR" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++

echo "[run-clang-tidy] Building project..."
cmake --build "$BUILD_DIR" -- -j"${NPROCS}"

if [ ! -f "$BUILD_DIR/compile_commands.json" ]; then
  echo "ERROR: compile_commands.json not found in ${BUILD_DIR}. Aborting clang-tidy run."
  ls -la "$BUILD_DIR" || true
  exit 3
fi

echo "[run-clang-tidy] Finding source files..."
FILES=()
while IFS= read -r -d '' file; do
  FILES+=("$file")
done < <(find src include lib -type f \( -name '*.cpp' -o -name '*.c' -o -name '*.hpp' -o -name '*.h' \) -print0)

if [ ${#FILES[@]} -eq 0 ]; then
  echo "No source files found under src/, include/, lib/. Exiting.";
  exit 0
fi

REPORT_TXT="${BUILD_DIR}/clang-tidy-report.txt"
REPORT_HTML="${BUILD_DIR}/clang-tidy-report.html"

echo "[run-clang-tidy] Running clang-tidy over files (this may take a while)..."

# Prepare extra args (helpful on macOS to point to libc++ headers)
EXTRA_ARGS=()
if [[ "$(uname)" == "Darwin" ]]; then
  if [ -d "/Library/Developer/CommandLineTools/usr/include/c++/v1" ]; then
    EXTRA_ARGS+=("-extra-arg=-stdlib=libc++" "-extra-arg=-isystem" "-extra-arg=/Library/Developer/CommandLineTools/usr/include/c++/v1")
  fi
  if [ -d "/opt/homebrew/opt/llvm/include/c++/v1" ]; then
    EXTRA_ARGS+=("-extra-arg=-isystem" "-extra-arg=/opt/homebrew/opt/llvm/include/c++/v1")
  fi
fi

# Run clang-tidy per-file, extracting compile flags from compile_commands.json when available
if [ -n "${FILES}" ]; then
  > "$REPORT_TXT"
  for f in "${FILES[@]}"; do
    echo "--- file: $f" | tee -a "$REPORT_TXT"
    # extract compile flags for this file from compile_commands.json using helper script
    EXTRA_FROM_DB=$(python3 "${PWD}/scripts/clang_tidy_flags.py" "$f" "$BUILD_DIR" || true)

    # combine EXTRA_ARGS detected earlier (e.g., libc++ includes on macOS) with db flags
    CMD=(clang-tidy -p "$BUILD_DIR")
    if [ -n "${EXTRA_FROM_DB}" ]; then
      # read flags into an array safely
      read -r -a DBFLAGS <<< "$EXTRA_FROM_DB"
      for a in "${DBFLAGS[@]}"; do
        CMD+=("$a")
      done
    fi
    for a in "${EXTRA_ARGS[@]}"; do
      CMD+=("$a")
    done

    # run clang-tidy for this file
    "${CMD[@]}" "$f" 2>&1 | tee -a "$REPORT_TXT" || true
  done
else
  echo "No source files found" > "$REPORT_TXT"
fi

echo "[run-clang-tidy] Generating HTML report..."
if command -v clang-tidy-html >/dev/null 2>&1; then
  clang-tidy-html "$REPORT_TXT" -o "$REPORT_HTML" || true
  echo "HTML report: $REPORT_HTML"
else
  echo "clang-tidy-html not found; skipping HTML generation. The text report is at: $REPORT_TXT"
fi

echo "[run-clang-tidy] Done."
