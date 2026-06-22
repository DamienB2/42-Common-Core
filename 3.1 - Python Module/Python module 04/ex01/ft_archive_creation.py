import sys

def save() -> None:
    name = input('Enter new file name (or empty):')
    if name


def main() -> None:
    if len(sys.argv) > 1:
        file = sys.argv[1]
        print("=== Cyber Archives Recovery ===")
        print(f"Accessing file '{file}'")
        try:
            f = open(file)

            print(f"---\n\n{f.read()}\n\n---")

            f.seek(0)
            lignes = [ligne.strip() + '#' for ligne in f.readlines()] 

            f.close()
            print(f"File '{file}' closed.")
            print()

            print("Transform data:")
            print("---\n")

            for ligne in lignes:
                print(ligne)

            print("\n---")


        except FileNotFoundError as e:
            print(f"Error opening file '{file}': {e}")
        except PermissionError as e:
            print(f"Error opening file '{file}': {e}")

    else:
        print("Usage: ft_ancient_text.py <file>")


if __name__ == "__main__":
    main()
