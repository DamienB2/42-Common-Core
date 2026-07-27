import importlib
from importlib import metadata
import numpy as np
import matplotlib.pyplot as plt
import pandas

REQUIRED_PACKAGES = {
    "pandas": "Data manipulation",
    "numpy": "Numerical computation",
    "requests": "Network access",
    "matplotlib": "Visualization",
}


def check_dependency(pkg_name):
    try:
        importlib.import_module(pkg_name)
    except ImportError:
        return {
            "name": pkg_name,
            "available": False,
            "version": None,
        }
    try:
        version = metadata.version(pkg_name)
    except metadata.PackageNotFoundError:
        version = "unknown"
    return {
        "name": pkg_name,
        "available": True,
        "version": version,
    }


def generate_matrix(nb: int = 1000):
    x = np.random.normal(loc=0.0, scale=1.0, size=nb)
    y = np.random.normal(loc=1.0, scale=2.0, size=n_points)


if __name__ == "__main__":
    for package, description in REQUIRED_PACKAGES.items():
        package_infos = check_dependency(package)
        if package_infos["available"]:
            print(
                f"[OK] {package_infos['name']}",
                f"({package_infos['version']})",
                f"- {description} ready",
            )
        else:
            print(f"[KO] {package_infos['name']} - not installed")
            print(
                f"Use 'pip install {package_infos['name']}' or",
                f"'poetry add {package_infos['name']}'",
                "to install the missing package",
            )

    generate_matrix()
