#!/bin/bash

while IFS= read -r -d '' file; do
    echo "Formatting $file"
    clang-format -i "$file"
done < <(find . -name '.pio' -prune -o \( -name "*.cpp" -o -name "*.h" \) -print0)

echo "Done"
