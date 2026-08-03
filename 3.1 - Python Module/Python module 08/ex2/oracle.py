from dotenv import load_dotenv
import os

def check_no_hardcoded_secrets():
    with open(__file__, 'r') as f:
        lines = f.readlines()
    suspicious_patterns = ['API_KEY = "', "API_KEY = '", 'API_KEY="', "API_KEY='"]
    for line in lines:
        if 'suspicious_patterns' in line:
            continue
        for pattern in suspicious_patterns:
            if pattern in line:
                return False
    return True

def check_env_file_configured():
    if not os.path.exists('.env'):
        return False
    required_vars = ['MATRIX_MODE', 'DATABASE_URL', 'API_KEY', 'LOG_LEVEL', 'ZION_ENDPOINT']
    with open('.env', 'r') as f:
        env_content = f.read()
    return all(var in env_content for var in required_vars)

def check_production_overrides():
    test_var = os.environ.get('MATRIX_MODE')
    return test_var is not None

def mask_secret(value):
    if value:
        return value[0] + ("*" * len(value)) + value[len(value) - 1]
    return value

if __name__ == "__main__":
    print("ORACLE STATUS: Reading the Matrix...\n")

    load_dotenv()
    MATRIX_MODE = os.getenv('MATRIX_MODE')
    DATABASE_URL = os.getenv('DATABASE_URL')
    API_KEY = os.getenv('API_KEY')
    LOG_LEVEL = os.getenv('LOG_LEVEL')
    ZION_ENDPOINT = os.getenv('ZION_ENDPOINT')

    print("Configuration loaded:")
    if MATRIX_MODE:
        print(f"Mode: {MATRIX_MODE}")
    else:
        print("Mode: Error (MATRIX_MODE not set)")

    if DATABASE_URL:
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
        print(f"Log Level: {LOG_LEVEL}")
    else:
        print("Log Level: Error (LOG_LEVEL not set)")

    if ZION_ENDPOINT:
        print(f"Zion Network: {ZION_ENDPOINT}")
    else:
        print("Zion Network: Error (ZION_ENDPOINT not set)")

    print("\nEnvironment security check:")
    print(f"[{'OK' if check_no_hardcoded_secrets() else 'FAIL'}] No hardcoded secrets detected")
    print(f"[{'OK' if check_env_file_configured() else 'WARNING'}] .env file properly configured")
    print(f"[{'OK' if check_production_overrides() else 'WARNING'}] Production overrides available")