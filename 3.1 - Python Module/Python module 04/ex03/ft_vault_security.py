def secure_archive(file_name: str, action: str = "r", content: str = "") -> tuple[bool, str]:
    try:
        with open(file_name, action) as file:
            if action == "r":
                return True, file.read()
            else:
                file.write(content)
                return True, "Content successfully written to file"
            
    except (PermissionError, FileNotFoundError) as e:
        return (False, str(e))

       

def main() -> None:
    print("=== Cyber Archives Security ===")
    print()

    print("Using 'secure_archive' to read from a nonexistent file:")
    print(secure_archive("not/existing/file"))
    print()

    print("Using 'secure_archive' to read from an inaccessible file:")
    print(secure_archive("/etc/master.passwd"))
    print()

    print("Using 'secure_archive' to read from a regular file:")
    print(secure_archive("ancient_fragment.txt"))
    print()
    
    print("Using 'secure_archive' to write previous content to a new file:")
    print(secure_archive("ancient_fragment.txt", "w", "[FRAGMENT 004] I'm the test fragment\n"))

if __name__ == "__main__":
    main()
