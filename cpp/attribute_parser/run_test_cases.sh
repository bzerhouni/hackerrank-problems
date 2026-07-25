#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/../.." && pwd)"
build_dir="$script_dir/build"
test_cases_dir="$script_dir/test_cases"
target="attribute_parser_exec"

cmake -S "$repo_root" -B "$build_dir" >/dev/null
cmake --build "$build_dir" --target "$target"

exec_path=$(find "$build_dir" -type f \( -name "$target" -o -name "$target.exe" \) | head -n 1)
if [[ -z "$exec_path" ]]; then
    echo "error: could not find built executable '$target' under $build_dir" >&2
    exit 1
fi

for test_case in "$test_cases_dir"/*; do
    [[ -f "$test_case" ]] || continue
    echo "=== $(basename "$test_case") ==="
    "$exec_path" < "$test_case"
    echo
done
