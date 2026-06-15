import random


def gen_player_achievement() -> set:
    achievements_list = ['Crafting Genius', 'Strategist', 'World Savior',
                   'Speed Runner', 'Survivor', 'Master Explorer',
                   'Treasure Hunter', 'Unstoppable', 'First Steps',
                   'Collector Supreme', 'Untouchable', 'Sharp Mind',
                   'Boss Slayer']
    achievements_set = set(
        random.sample(
            achievements_list,
            random.randrange(1, len(achievements_list) + 1)
        )
    )
    return achievements_set


def main() -> None:
    print("=== Achievement Tracker System ===")
    print()

    Alice = gen_player_achievement()
    print(f"Player Alice: {Alice}")
    Bob = gen_player_achievement()
    print(f"Player Bob: {Bob}")
    Charlie = gen_player_achievement()
    print(f"Player Chralie: {Charlie}")
    Dylan = gen_player_achievement()
    print(f"Player Dylan: {Dylan}")
    print()

    all_distinct_achievements = Alice.union(Bob, Charlie, Dylan)
    print(f"All distinct achievements: {all_distinct_achievements}")
    print()

    common_achievements = Alice.intersection(Bob, Charlie, Dylan)
    print(f"Common achievements: {common_achievements}")
    print()

    print(f"Only Alice has: {Alice.difference(Bob, Charlie, Dylan)}")
    print(f"Only Bob has: {Bob.difference(Alice, Charlie, Dylan)}")
    print(f"Only Charlie has: {Charlie.difference(Alice, Bob, Dylan)}")
    print(f"Only Dylan has: {Dylan.difference(Alice, Bob, Charlie)}")
    print()

    print(f"Alice is missing: {all_distinct_achievements.difference(Alice)}")
    print(f"Bob is missing: {all_distinct_achievements.difference(Bob)}")
    print(f"Charlie is missing: {all_distinct_achievements.difference(Charlie)}")
    print(f"Dylan is missing: {all_distinct_achievements.difference(Dylan)}")


if __name__ == "__main__":
    main()