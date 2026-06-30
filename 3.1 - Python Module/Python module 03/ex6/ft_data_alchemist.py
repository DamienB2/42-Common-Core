import random


def main() -> None:
    print("=== Game Data Alchemist ===")
    print()

    player_list = ['Alice', 'bob', 'Charlie', 'dylan', 'Emma', 'Gregory', 'john', 'kevin', 'Liam']
    print(f"Initial list of players: {player_list}")

    all_capitalized_list = [player.capitalize() for player in player_list]
    print(f"New list with all names capitalized: {all_capitalized_list}")

    capitalized_player_list = [player for player in player_list if player == player.capitalize()]
    print(f"New list of capitalized names only: {capitalized_player_list}")

    score_dict = {player: random.randint(0, 1000) for player in all_capitalized_list}
    print(f"Score dict: {score_dict}")

    average = sum(score_dict.values()) / len(score_dict)
    print(f"Score average is {round(average, 2)}")

    high_score_dict = {player: score for player, score in score_dict.items() if score > average}
    print(f"High scores: {high_score_dict}")
         

if __name__ == "__main__":
    main()
