import sys


def main() -> None:
    if len(sys.argv) > 1:
        file = sys.argv[1]
        print("=== Cyber Archives Recovery ===")
        print(f"Accessing file '{file}'")
        try:
            f = open(file)
            print(f"---\n\n{f.read()}\n\n---")
            f.close()
            print(f"File '{file}' closed.")

        except FileNotFoundError as e:
            print(f"Error opening file '{file}': {e}")
        except PermissionError as e:
            print(f"Error opening file '{file}': {e}")

    else:
        print("Usage: ft_ancient_text.py <file>")


if __name__ == "__main__":
    main()
