import sys
import sysconfig
import os


def get_site_packages() -> str:
    return sysconfig.get_paths()["purelib"]


if __name__ == "__main__":
    in_venv = sys.base_prefix != sys.prefix

    if in_venv:
        message = "Welcome to the construct"
    else:
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
        print("source matrix_env/bin/activate  # On Unix")
        print(r"matrix_env\Scripts\activate  # On Windows")
        print("\nThen run this program again.")
