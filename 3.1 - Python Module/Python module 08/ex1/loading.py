from __future__ import annotations

import importlib
from importlib import metadata
from typing import TYPE_CHECKING, TypedDict

if TYPE_CHECKING:
    import pandas as pd

REQUIRED_PACKAGES = {
    "pandas": "Data manipulation",
    "numpy": "Numerical computation",
    "requests": "Network access",
    "matplotlib": "Visualization",
}


class PackageInfo(TypedDict):
    name: str
    available: bool
    version: str | None


def check_dependency(pkg_name: str) -> PackageInfo:
    try:
        importlib.import_module(pkg_name)
    except ImportError:
        return {"name": pkg_name, "available": False, "version": None}
    try:
        version = metadata.version(pkg_name)
    except metadata.PackageNotFoundError:
        version = "unknown"
    return {"name": pkg_name, "available": True, "version": version}


def generate_matrix(nb: int = 1000) -> pd.DataFrame:
    import numpy as np
    import pandas as pd

    x = np.random.normal(size=nb)
    y = np.random.normal(size=nb)
    return pd.DataFrame({"x": x, "y": y})


def visualize(
    df: pd.DataFrame, output_path: str = "matrix_analysis.png"
) -> None:
    import matplotlib.pyplot as plt

    print("Generating visualization...")
    fig, ax = plt.subplots()
    ax.scatter(df["x"], df["y"], s=10)
    ax.set_title("Matrix Data Analysis")
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    fig.savefig(output_path)
    plt.close(fig)
    print(f"Analysis complete! Results saved to: {output_path}")


def main() -> None:
    all_imported = True
    for package, description in REQUIRED_PACKAGES.items():
        package_infos = check_dependency(package)
        if package_infos["available"]:
            print(
                f"[OK] {package_infos['name']}",
                f"({package_infos['version']})",
                f"- {description} ready",
            )
        else:
            all_imported = False
            print(f"[KO] {package_infos['name']} - not installed")
            print(
                f"Use 'pip install {package_infos['name']}' or",
                f"'poetry add {package_infos['name']}'",
                "to install the missing package",
            )

    if all_imported:
        df = generate_matrix()
        visualize(df)


if __name__ == "__main__":
    main()
