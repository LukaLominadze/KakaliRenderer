import subprocess
import os

# Path to your MSI file (use raw string or double backslashes)
msi_path = os.getcwd() + "\\vendor\\installations\\cmake-4.0.3-windows-x86_64.msi"

# Run the installer with msiexec
# Example: silent install (you can adjust parameters)
subprocess.run([
    "msiexec.exe",
    "/i",
    msi_path
], check=True)