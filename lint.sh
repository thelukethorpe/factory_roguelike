#!/bin/bash

set -e

echo "Running C++ lint checks..."

if ! command -v clang-format >/dev/null 2>&1; then
    echo "ERROR: clang-format not installed"
    exit 1
fi

if ! command -v clang-tidy >/dev/null 2>&1; then
    echo "ERROR: clang-tidy not installed"
    exit 1
fi

./build.sh

FILES=$(find src tests \
    \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \))

if [ -z "$FILES" ]; then
    echo "No source files found"
    exit 0
fi

echo ""
echo "Formatting source files..."

for file in $FILES; do
    clang-format -i "$file"
done

#
# Re-stage formatted files
#

git add $FILES

echo ""
echo "Running clang-tidy..."

for file in $FILES; do
    case "$file" in
        *.cpp)
            clang-tidy \
                "$file" \
                -p build
            ;;
    esac
done

echo ""
echo "Lint successful"
