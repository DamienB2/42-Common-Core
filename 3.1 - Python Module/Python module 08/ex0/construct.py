import sys
import os


def get_site_packages() -> str:
    venv_root = sys.prefix
    lib_dir = os.path.join(venv_root, "lib")
    python_dirs = [
        d
        for d in os.listdir(lib_dir)
        if os.path.isdir(os.path.join(lib_dir, d)) and d.startswith("python3.")
    ]
    return os.path.join(lib_dir, python_dirs[0], "site-packages")


if __name__ == "__main__":
    if hasattr(sys, "real_prefix") or (
        hasattr(sys, "base_prefix") and sys.base_prefix != sys.prefix
    ):
        in_venv = 1
        message = "Welcome to the construct"
    else:
        in_venv = 0
        message = "You're still plugged in"

    print(f"MATRIX STATUS: {message}\n")
    print(f"Current Python: {sys.executable}")

    if in_venv:
        print(f"Virtual Environment: {os.path.basename(sys.prefix)}")
        print(f"Environment Path: {sys.prefix}\n")
        print("SUCCESS: You're in an isolated environment!")
        print("Safe to install packages without affecting")
        print("the global system.\n")
        print("Package installation path:")

        print(get_site_packages())

    else:
        print("Virtual Environment: None detected\n")
        print("WARNING: You're in the global environment!")
        print("The machines can see everything you install.\n")
        print("To enter the construct, run:")
        print("python -m venv matrix_env")
        print("source matrix_env/bin/activate # On Unix")
        print(r"matrix_env\Scripts\activate # On Windows\n")
        print("Then run this program again.")
