from pydantic import BaseModel, Field
from datetime import datetime
from typing import Optional
from pydantic import ValidationError


class SpaceStation(BaseModel):
    station_id: str = Field(min_length=3, max_length=10)
    name: str = Field(min_length=1, max_length=50)
    crew_size: int = Field(ge=1, le=20)
    power_level: float = Field(ge=0.0, le=100.0)
    oxygen_level: float = Field(ge=0.0, le=100.0)
    last_maintenance: datetime
    is_operational: bool = Field(default=True)
    notes: Optional[str] = Field(default=None, max_length=200)


def show(space_station: SpaceStation) -> None:
    print("Space Station Data Validation")
    print("========================================")
    print("Valid station created:")
    print(f"ID: {space_station.station_id}")
    print(f"Name: {space_station.name}")
    print(f"Crew: {space_station.crew_size} people")
    print(f"Power: {space_station.power_level}%")
    print(f"Oxygen: {space_station.oxygen_level}%")
    if space_station.is_operational:
        print("Status: Operational")
    else:
        print("Status: Not Operational")


if __name__ == "__main__":
    try:
        space_station = SpaceStation(
            station_id="ISS001",
            name="International Space Station",
            crew_size=6,
            power_level=85.5,
            oxygen_level=92.3,
            last_maintenance=datetime(2026, 7, 31),
            is_operational=True,
        )
        show(space_station)
        print()

        print("========================================")
        print("Expected validation error:")

        space_station = SpaceStation(
            station_id="ISS001",
            name="International Space Station",
            crew_size=21,
            power_level=85.5,
            oxygen_level=92.3,
            last_maintenance=datetime(2026, 7, 31),
            is_operational=True,
        )
    except ValidationError as e:
        for err in e.errors():
            print(err["msg"])
    except ValueError as e:
        print(e)
