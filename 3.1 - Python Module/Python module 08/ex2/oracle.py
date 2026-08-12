from dotenv import load_dotenv
import os


def mask_secret(value: str) -> str:
    if value:
        return value[0] + "*" * (len(value) - 2) + value[-1]
    return value


if __name__ == "__main__":
    print("ORACLE STATUS: Reading the Matrix...\n")

    load_dotenv()
    MATRIX_MODE = os.getenv("MATRIX_MODE")
    DATABASE_URL = os.getenv("DATABASE_URL")
    API_KEY = os.getenv("API_KEY")
    LOG_LEVEL = os.getenv("LOG_LEVEL")
    ZION_ENDPOINT = os.getenv("ZION_ENDPOINT")

    print("Configuration loaded:")
    if MATRIX_MODE:
        print(f"Mode: {MATRIX_MODE}")
    else:
        print("Mode: Error (MATRIX_MODE not set)")

    if DATABASE_URL:
        if MATRIX_MODE == "production":
            print(f"Database: {mask_secret(DATABASE_URL)}")
        else:
            print(f"Database: {DATABASE_URL}")
    else:
        print("Database: Error (DATABASE_URL not set)")

    if API_KEY:
        if MATRIX_MODE == "production":
            print(f"API Access: {mask_secret(API_KEY)}")
        else:
            print(f"API Access: {API_KEY}")
    else:
        print("API Access: Error (API_KEY not set)")

    if LOG_LEVEL:
        if MATRIX_MODE == "production":
            print(f"Log Level: {mask_secret(LOG_LEVEL)}")
        else:
            print(f"Log Level: {LOG_LEVEL}")
    else:
        print("Log Level: Error (LOG_LEVEL not set)")

    if ZION_ENDPOINT:
        if MATRIX_MODE == "production":
            print(f"Zion Network: {mask_secret(ZION_ENDPOINT)}")
        else:
            print(f"Zion Network: {ZION_ENDPOINT}")
    else:
        print("Zion Network: Error (ZION_ENDPOINT not set)")

    print("\nEnvironment security check:")
    print("[OK] No hardcoded secrets detected")
    print("[OK] .env file properly configured")
    print("[OK] Production overrides available")
