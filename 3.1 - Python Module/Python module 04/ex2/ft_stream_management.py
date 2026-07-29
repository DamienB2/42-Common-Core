import sys


def save(lignes: list) -> None:
    sys.stdout.write("Enter new file name (or empty): ")
    sys.stdout.flush()
    name = sys.stdin.readline().strip()
    if name == "":
        print("Not saving data.")
    else:
        try:
            print(f"Saving data to '{name}'")
            f = open(name, "w")
            for ligne in lignes:
                f.write(ligne + "\n")
            f.close()
            print(f"Data saved in file '{name}'.")
        except PermissionError as e:
            print(
                f"[STDERR] Error opening file '{name}': {e}", file=sys.stderr
            )
            print("Data not saved.")


def main() -> None:
    if len(sys.argv) > 1:
        file = sys.argv[1]
        print("=== Cyber Archives Recovery & Preservation ===")
        print(f"Accessing file '{file}'")
        try:
            f = open(file)

            content = f.read()
            print(f"---\n\n{content}\n\n---")
            lignes = [
                ligne.strip() + "#"
                for ligne in content.split("\n")
                if ligne.strip() != ""
            ]
            f.close()

            print(f"File '{file}' closed.")
            print()

            print("Transform data:")
            print("---\n")

            for ligne in lignes:
                print(ligne)

            print("\n---")

            save(lignes)

        except FileNotFoundError as e:
            print(
                f"[STDERR] Error opening file '{file}': {e}", file=sys.stderr
            )
        except PermissionError as e:
            print(
                f"[STDERR] Error opening file '{file}': {e}", file=sys.stderr
            )

    else:
        print("Usage: ft_stream_management.py <file>")


if __name__ == "__main__":
    main()
