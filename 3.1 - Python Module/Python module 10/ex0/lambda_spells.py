from typing import Any


def artifact_sorter(artifacts: list[dict[str, Any]]) -> list[dict[str, Any]]:
    return sorted(
        artifacts, key=lambda artifact: artifact["power"], reverse=True
    )


def power_filter(
    mages: list[dict[str, Any]], min_power: int
) -> list[dict[str, Any]]:
    return list(filter(lambda mage: mage["power"] >= min_power, mages))


def spell_transformer(spells: list[str]) -> list[str]:
    return list(map(lambda spell: "* " + spell + " *", spells))


def mage_stats(mages: list[dict[str, Any]]) -> dict[str, Any]:
    if not mages:
        return {"max_power": 0, "min_power": 0, "avg_power": 0.0}

    most_powerful = max(mages, key=lambda mage: mage["power"])
    least_powerful = min(mages, key=lambda mage: mage["power"])
    total_power = sum(map(lambda mage: mage["power"], mages))
    avg_power = round(total_power / len(mages), 2)

    return {
        "max_power": most_powerful["power"],
        "min_power": least_powerful["power"],
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

    print("Testing artifact sorter...")
    sorted_artifacts = artifact_sorter(artifacts)
    print(
        f"{sorted_artifacts[0]['name']} ({sorted_artifacts[0]['power']} power)"
        f"comes before {sorted_artifacts[1]['name']} "
        f"({sorted_artifacts[1]['power']} power)"
    )
    print()

    print("Testing power filter...")
    print(power_filter(mages, 20))
    print()

    print("Testing spell transformer...")
    print(" ".join(spell_transformer(spells)))
    print()

    print("Testing mage stats...")
    print(mage_stats(mages))
