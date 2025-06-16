#!/bin/bash

# Check if CMake is installed
if command -v cmake &> /dev/null; then
    echo "CMake is already installed."

    # Check if Python is available
    if command -v python &> /dev/null; then
        PYTHON_EXEC=$(command -v python)

        # Check if glad is installed via pip
        if "$PYTHON_EXEC" -m pip show glad &> /dev/null; then
            echo "Python package 'glad' is already installed."
        else
            echo "'glad' Python module not found."
            read -p "Do you want to install it using pip? (y/n) " pip_choice
            if [[ "$pip_choice" == "y" || "$pip_choice" == "Y" ]]; then
		        echo "installing 'glad'."
                "$PYTHON_EXEC" -m pip install glad
		        "$PYTHON_EXEC" -m pip show glad
            else
                echo "Glad module installation skipped."
            fi
        fi
    else
        echo "Python is not installed. Please install Python to use the glad generator."
    fi

    # Define project directory (current directory or specify your project path)
    PROJECT_DIR="$(pwd)"
    BUILD_DIR="$PROJECT_DIR/build"

    # Create build directory if it doesn't exist
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"

    echo "Running CMake configuration..."
    cmake ..

    echo "Building project..."
    cmake --build .

else
    echo "CMake is not installed."
    read -p "Do you want to download and install CMake? (y/n) " choice
    if [[ "$choice" == "y" || "$choice" == "Y" ]]; then
        echo "Opening CMake download page..."
        cmd.exe /c start https://cmake.org/download/
    else
        echo "Installation canceled."
    fi
fi

read -p "Press Enter to exit..."
