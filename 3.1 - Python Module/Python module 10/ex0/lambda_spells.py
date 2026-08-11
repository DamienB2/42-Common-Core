def artifact_sorter(artifacts: list[dict]) -> list[dict]:
    return sorted(
        artifacts, key=lambda artifact: artifact["power"], reverse=True
    )


def power_filter(mages: list[dict], min_power: int) -> list[dict]:
    return list(filter(lambda mage: mage["power"] >= min_power, mages))


def spell_transformer(spells: list[str]) -> list[str]:
    return list(map(lambda spell: "* " + spell + " *", spells))


def mage_stats(mages: list[dict]) -> dict:
    powers = list(map(lambda mage: mage["power"], mages))
    max_power = max(powers)
    min_power = min(powers)
    avg_power = round(sum(powers) / len(powers), 2)
    return {
        "max_power": max_power,
        "min_power": min_power,
        "avg_power": avg_power,
    }


if __name__ == "__main__":
    artifacts = [
        {"name": "Orb of Night", "power": 42, "type": "dark"},
        {"name": "Sun Crystal", "power": 80, "type": "light"},
        {"name": "Rusty Wand", "power": 5, "type": "chaos"},
    ]

    mages = [
        {"name": "Paul", "power": 42, "element": "fire"},
        {"name": "Jacques", "power": 80, "element": "water"},
        {"name": "Thierry", "power": 5, "element": "air"},
    ]

    spells = ["fireball", "heal", "shield"]

    print("Testing artifact_sorter...")
    print(artifact_sorter(artifacts))
    print()

    print("Testing power_filter...")
    print(power_filter(mages, 20))
    print()

    print("Testing spell_transformer...")
    print(spell_transformer(spells))
    print()

    print("Testing mage_stats...")
    print(mage_stats(mages))
