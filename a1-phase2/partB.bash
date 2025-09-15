#!/bin/bash

# AAKASH JANA AAJ284 11297343
# NANDISH JHA NAJ474 11282001

# Give group read permission to this script
chmod g+r "$0"

set -e

RED='\033[0;31m'    # ${RED}
CYAN='\033[0;36m'   # ${CYAN}
GREEN='\033[0;32m'  # ${GREEN}
WHITE='\033[0;37m'  # ${WHITE}

# --- Check arguments ---
if [ $# -lt 1 ]; then
    echo -e "${RED}Error, usage: $0 {partA1|partA2|partA3|partA4|partC|clean}${WHITE}" >&1
    exit 1
fi

# Set the target to the first argument
TARGET=$1

# --- Handle 'clean' argument ---
if [ "$TARGET" = "clean" ]; then
    echo -e "${GREEN}Cleaning up...${WHITE}"
    make clean
    exit 0
fi

# --- Validate target ---
case "$TARGET" in
    partA1|partA2|partA3|partA4|partC)
        ;;
    *)
        echo -e "${RED}Error: Unknown target '$TARGET'" >&2
        echo -e "Valid options: partA1, partA2, partA3, partA4, partC, clean${WHITE}" >&2
        exit 1
        ;;
esac

# --- Build the target ---
echo -e "${GREEN}Building $TARGET...${WHITE}"
make "$TARGET" || { echo -e "${RED}Error: Build failed for $TARGET${WHITE}" >&2; exit 1; }

# --- Run the program ---
echo -e "${GREEN}Running $TARGET..."
echo -e "-----------------------------------------"
echo -e "${WHITE}"

# Function to validate arguments
validate_args() {
    local threads=$1
    local deadline=$2
    local size=$3

    if ! [[ "$threads" =~ ^[1-9][0-9]*$ && "$deadline" =~ ^[1-9][0-9]*$ && "$size" =~ ^[1-9][0-9]*$ ]]; then
        echo -e "${RED}Error: All arguments must be positive integers greater than 0.${WHITE}"
        echo -e ""
        return 1
    fi
    if [ "$threads" -gt 1000 ]; then
        echo -e "${RED}Error: threads cannot exceed 1000.${WHITE}"
        return 1
    fi
    if [ "$deadline" -gt 120 ]; then
        echo -e "${RED}Error: deadline cannot exceed 120 seconds.${WHITE}"
        return 1
    fi
    if [ "$size" -gt 10000 ]; then
        echo -e "${RED}Error: size cannot exceed 10000.${WHITE}"
        return 1
    fi
    return 0
}

# Only PartA variants take input arguments
if [[ "$TARGET" =~ ^partA[1-4]$ ]]; then
    while true; do
        echo -e "${CYAN}"
        echo -e "Choose input method:"
        echo -e "1) Enter input manually from keyboard"
        echo -e "2) Read input from a file"
        echo -e "3) Exit"
        read -p "Enter choice (1, 2, or 3): " choice
        echo -e "${WHITE}"

        case "$choice" in
            1)
                echo -e "${CYAN}Enter input (format: threads deadline size), Ctrl+D to finish:${WHITE}"
                while read -r threads deadline size; do
                    # Skip empty lines
                    if [ -z "$threads$deadline$size" ]; then
                        continue
                    fi

                    if ! validate_args "$threads" "$deadline" "$size"; then
                        echo -e "${RED}Invalid input. Returning to input method selection.${WHITE}"
                        break
                    fi

                    if [ -x "./$TARGET" ]; then
                        ./"$TARGET" "$threads" "$deadline" "$size" || \
                            echo -e "${RED}Error: $TARGET execution failed for input $threads $deadline $size${WHITE}"
                    else
                        echo -e "${RED}Error: $TARGET executable not found or not executable${WHITE}"
                        break
                    fi
                    echo -e "${CYAN}-----------------------------------------${WHITE}"
                done
                ;;
            2)
                echo -e "${CYAN}"
                read -p "Enter input file path: " file
                echo -e "${WHITE}"
                if [ ! -f "$file" ]; then
                    echo -e "${RED}Error: File '$file' not found. Returning to input method selection.${WHITE}"
                    continue
                fi
                if [ ! -r "$file" ]; then
                    echo -e "${RED}Error: File '$file' is not readable. Returning to input method selection.${WHITE}"
                    continue
                fi

                while read -r threads deadline size; do
                    # Skip empty lines
                    if [ -z "$threads$deadline$size" ]; then
                        continue
                    fi

                    if ! validate_args "$threads" "$deadline" "$size"; then
                        echo -e "${RED}Invalid input in file: $threads $deadline $size. Skipping...${WHITE}"
                        echo -e ""
                        continue
                    fi

                    if [ -x "./$TARGET" ]; then
                        ./"$TARGET" "$threads" "$deadline" "$size" || \
                            echo -e "${RED}Error: $TARGET execution failed for input $threads $deadline $size${WHITE}"
                    else
                        echo -e "${RED}Error: $TARGET executable not found or not executable${WHITE}"
                        break
                    fi
                    echo -e "${CYAN}-----------------------------------------${WHITE}"
                done < "$file"
                ;;
            3)
                echo -e "${GREEN}Exiting...${WHITE}"
                exit 0
                ;;
            *)
                echo -e "${RED}Invalid choice. Please select 1, 2, or 3.${WHITE}"
                ;;
        esac
    done
elif [ "$TARGET" = "partC" ]; then
    echo -e "${RED}Running partC (no input arguments needed)...${WHITE}"
    if [ -x "./$TARGET" ]; then
        ./"$TARGET" || echo -e "Error: $TARGET execution failed${WHITE}"
    else
        echo -e "${RED}Error: $TARGET executable not found or not executable${WHITE}"
    fi
else
    echo -e "${RED}Error: Unsupported target $TARGET${WHITE}"
    exit 1
fi