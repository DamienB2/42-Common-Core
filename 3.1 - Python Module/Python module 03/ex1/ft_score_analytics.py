import sys


def print_usage() -> None:
    print(
        "No scores provided."
        " Usage: python3 ft_score_analytics.py <score1> <score2> ..."
    )


def display_stats(score_list: list[int]) -> None:
    total_player = len(score_list)
    total_score = sum(score_list[0:])
    max_score = max(score_list)
    min_score = min(score_list)

    print(f"\033[0;34mScores processed:\033[0m {score_list}")
    print(f"\033[0;34mTotal players:\033[0m {total_player}")
    print(f"\033[0;32mTotal score:\033[0m {total_score}")
    print(f"\033[0;32mAverage score:\033[0m {total_score / total_player}")
    print(f"\033[0;33mHigh score:\033[0m {max_score}")
    print(f"\033[0;31mLow score:\033[0m {min_score}")
    print(f"\033[0;35mScore range:\033[0m {max_score - min_score}")


def main() -> None:
    score_list: list[int] = []
    print("=== Player Score Analytics ===")
    if len(sys.argv) <= 1:
        print_usage()
        return
    for item in sys.argv[1:]:
        try:
            score = int(item)
            score_list.append(score)
        except ValueError:
            print(f"Invalid parameter: '{item}'")
    if not score_list:
        print_usage()
        return
    display_stats(score_list)


if __name__ == "__main__":
    main()
