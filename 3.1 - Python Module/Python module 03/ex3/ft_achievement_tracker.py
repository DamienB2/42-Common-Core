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

    alice = gen_player_achievement()
    print(f"Player Alice: {alice}")
    bob = gen_player_achievement()
    print(f"Player Bob: {bob}")
    charlie = gen_player_achievement()
    print(f"Player Charlie: {charlie}")
    dylan = gen_player_achievement()
    print(f"Player Dylan: {dylan}")
    print()

    all_distinct_achievements = alice.union(bob, charlie, dylan)
    print(f"All distinct achievements: {all_distinct_achievements}")
    print()

    common_achievements = alice.intersection(bob, charlie, dylan)
    print(f"Common achievements: {common_achievements}")
    print()

    print(f"Only Alice has: {alice.difference(bob, charlie, dylan)}")
    print(f"Only Bob has: {bob.difference(alice, charlie, dylan)}")
    print(f"Only Charlie has: {charlie.difference(alice, bob, dylan)}")
    print(f"Only Dylan has: {dylan.difference(alice, bob, charlie)}")
    print()

    print(f"Alice is missing: {all_distinct_achievements.difference(alice)}")
    print(f"Bob is missing: {all_distinct_achievements.difference(bob)}")
    print(f"Charlie is missing: {all_distinct_achievements.difference(charlie)}")
    print(f"Dylan is missing: {all_distinct_achievements.difference(dylan)}")


if __name__ == "__main__":
    main()