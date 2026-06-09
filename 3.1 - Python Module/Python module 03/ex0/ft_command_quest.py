import sys


def main() -> None:
    arg_nbr = 1
    print("=== Command Quest ===")

    print("Program name: " + sys.argv[0])

    if len(sys.argv) <= 1:
        print("No arguments provided!")
        return

    print(f"Arguments received: {len(sys.argv) - 1}")

    for i in sys.argv[1:]:
        print(f"Argument {arg_nbr}: {i}")
        arg_nbr += 1

    print("Total arguments:", len(sys.argv))


if __name__ == "__main__":
    main()
