
def artifact_sorter(artifacts: list[dict]) -> list[dict]:
    return sorted(artifacts, key=lambda artifact: artifact["power"], reverse=True)


def power_filter(mages: list[dict], min_power: int) -> list[dict]:
    return list(filter(lambda mage: mage['power'] >= min_power, mages))
# def spell_transformer(spells: list[str]) -> list[str]:

# def mage_stats(mages: list[dict]) -> dict:

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

    print(artifact_sorter(artifacts))
    print(power_filter(mages, 20))
    