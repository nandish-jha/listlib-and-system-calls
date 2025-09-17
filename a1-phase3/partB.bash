#!/bin/bash

# AAKASH JANA AAJ284 11297343
# NANDISH JHA NAJ474 11282001

# Give group read permission to this script
chmod g+r "$0"

set -e

# --- Check arguments ---
if [ $# -lt 1 ]; then
    echo "Error, usage: $0 {partA1|partA2|partA3|partA4|partC|clean}" >&1
    exit 1
fi

# Set the target to the first argument
TARGET=$1

# --- Handle 'clean' argument ---
if [ "$TARGET" = "clean" ]; then
    echo "Cleaning up..."
    make clean
    exit 0
fi

# --- Validate target ---
case "$TARGET" in
    partA1|partA2|partA3|partA4|partC)
        ;;
    *)
        echo "Error: Unknown target '$TARGET'" >&2
    echo "Valid options: partA1, partA2, partA3, partA4, partC, clean" \
    >&2
        exit 1
        ;;
esac

# --- Build the target ---
echo "Building $TARGET..."
make "$TARGET" || { echo "Error: Build failed for $TARGET" >&2; exit 1; }

# --- Run the program ---
echo "Running $TARGET..."
echo "-----------------------------------------"
echo ""

# Function to validate arguments
validate_args() {
    local threads=$1
    local deadline=$2
    local size=$3

    if ! [[ "$threads" =~ ^[1-9][0-9]*$ && "$deadline" =~ ^[1-9][0-9]*$ && \
        "$size" =~ ^[1-9][0-9]*$ ]]; then
        echo "Error: All arguments must be positive integers greater than 0."
        echo ""
        return 1
    fi
    if [ "$threads" -gt 1000 ]; then
        echo "Error: threads cannot exceed 1000."
        return 1
    fi
    if [ "$deadline" -gt 120 ]; then
        echo "Error: deadline cannot exceed 120 seconds."
        return 1
    fi
    if [ "$size" -gt 10000 ]; then
        echo "Error: size cannot exceed 10000."
        return 1
    fi
    return 0
}

# Only PartA variants take input arguments
if [[ "$TARGET" =~ ^partA[1-4]$ ]]; then
    while true; do
        echo ""
        echo "Choose input method:"
        echo "1) Enter input manually from keyboard"
        echo "2) Read input from a file"
        echo "3) Exit"
        read -p "Enter choice (1, 2, or 3): " choice
        echo ""

        case "$choice" in
            1)
                echo "Enter input (format: threads deadline size)," \
                "Ctrl+D to finish:"
                while read -r threads deadline size; do
                    # Skip empty lines
                    if [ -z "$threads$deadline$size" ]; then
                        continue
                    fi

                    if ! validate_args "$threads" "$deadline" "$size"; then
                        echo "Invalid input. Returning to input method selection."
                        break
                    fi

                    if [ -x "./$TARGET" ]; then
                        ./"$TARGET" "$threads" "$deadline" "$size" || \
                            echo "Error: $TARGET execution failed for input" \
                            "$threads $deadline $size"
                    else
                        echo "Error: $TARGET executable not found or not executable"
                        break
                    fi
                    echo "-----------------------------------------"
                done
                ;;
            2)
                echo ""
                read -p "Enter input file path: " file
                echo ""
                if [ ! -f "$file" ]; then
                    echo "Error: File '$file' not found. Returning to input method selection."
                    continue
                fi
                if [ ! -r "$file" ]; then
                    echo "Error: File '$file' is not readable. Returning to input method selection."
                    continue
                fi

                while read -r threads deadline size; do
                    # Skip empty lines
                    if [ -z "$threads$deadline$size" ]; then
                        continue
                    fi

                    if ! validate_args "$threads" "$deadline" "$size"; then
                        echo "Invalid input in file: $threads $deadline $size. Skipping..."
                        echo ""
                        continue
                    fi

                    if [ -x "./$TARGET" ]; then
                        ./"$TARGET" "$threads" "$deadline" "$size" || \
                            echo "Error: $TARGET execution failed for input" \
                            "$threads $deadline $size"
                    else
                        echo "Error: $TARGET executable not found or not executable"
                        break
                    fi
                    echo "-----------------------------------------"
                done < "$file"
                ;;
            3)
                echo "Exiting..."
                exit 0
                ;;
            *)
                echo "Invalid choice. Please select 1, 2, or 3."
                ;;
        esac
    done
elif [ "$TARGET" = "partC" ]; then
    echo "Running partC (no input arguments needed)..."
    if [ -x "./$TARGET" ]; then
        ./"$TARGET" || echo "Error: $TARGET execution failed"
    else
        echo "Error: $TARGET executable not found or not executable"
    fi
else
    echo "Error: Unsupported target $TARGET"
    exit 1
fi