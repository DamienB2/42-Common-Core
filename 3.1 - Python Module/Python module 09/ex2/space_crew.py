from pydantic import BaseModel, Field, model_validator, ValidationError
from enum import Enum
from datetime import datetime
from typing import Optional


class Rank(Enum):
    CADET = "cadet"
    OFFICER = "officer"
    LIEUTENANT = "lieutenant"
    CAPTAIN = "captain"
    COMMANDER = "commander"


class CrewMember(BaseModel):
    member_id: str = Field(min_length=3, max_length=10)
    name: str = Field(min_length=2, max_length=50)
    rank: Rank
    age: int = Field(ge=18, le=80)
    specialization: str = Field(min_length=3, max_length=30)
    years_experience: int = Field(ge=0, le=50)
    is_active: bool = Field(default=True)


class SpaceMission(BaseModel):
    mission_id: str = Field(min_length=5, max_length=15)
    mission_name: str = Field(min_length=3, max_length=100)
    destination: str = Field(min_length=3, max_length=50)
    launch_date: datetime
    duration_days: int = Field(ge=1, le=3650)
    crew: list[CrewMember] = Field(min_length=1, max_length=12)
    mission_status: str = Field(default="planned")
    budget_millions: float = Field(ge=1.0, le=10000.0)

    @model_validator(mode="after")
    def mission_validation_rules(self) -> "SpaceMission":

        high_graded_in = any(
            member.rank in (Rank.COMMANDER, Rank.CAPTAIN)
            for member in self.crew
        )

        count = 0
        active_count = 0
        enough_experience = False
        all_crew_active = False

        for member in self.crew:
            if member.years_experience >= 5:
                count += 1
            if member.is_active:
                active_count += 1

        if count > (len(self.crew) / 2):
            enough_experience = True

        if active_count == len(self.crew):
            all_crew_active = True

        if not self.mission_id.startswith("M"):
            raise ValueError('Mission ID must start with "M"')
        elif not high_graded_in:
            raise ValueError("Mission must have at least one Commander or Captain")
        elif self.duration_days > 365 and not enough_experience:
            raise ValueError(
                "50% of the crew must be 5+ years experienced in long mission"
            )
        elif not all_crew_active:
            raise ValueError("All crew members must be active")
        else:
            return self


def show(spacemission: SpaceMission) -> None:
    print("Valid mission created:")
    print(f"Mission: {spacemission.mission_name}")
    print(f"ID: {spacemission.mission_id}")
    print(f"Destination: {spacemission.destination}")
    print(f"Duration: {spacemission.duration_days} days")
    print(f"Budget: ${spacemission.budget_millions}M")
    print(f"Crew size: {len(spacemission.crew)}")
    print("Crew members:")
    for member in spacemission.crew:
        print(f"- {member.name} ({member.rank.value}) - {member.specialization}")


if __name__ == "__main__":
    try:
        Sarah = CrewMember(
            member_id="USA001",
            name="Sarah Proche",
            rank=Rank.COMMANDER,
            age=35,
            specialization="Mission Command",
            years_experience=8
        )

        Louis = CrewMember(
            member_id="EU001",
            name="Louis Vuitton",
            rank=Rank.LIEUTENANT,
            age=45,
            specialization="Navigation",
            years_experience=18
        )

        Alico = CrewMember(
            member_id="EU002",
            name="Alico Merveille",
            rank=Rank.OFFICER,
            age=28,
            specialization="Engineering",
            years_experience=4
        )

        Mars_colony = SpaceMission(
            mission_id="M2024_MARS",
            mission_name="Mars Colony Establishment",
            destination="Mars",
            launch_date=datetime(2024, 3, 24),
            duration_days=900,
            crew=[Sarah, Louis, Alico],
            mission_status="ongoing",
            budget_millions=2500.0
        )
        print("Space Mission Crew Validation")
        print("=========================================")
        show(Mars_colony)
        print()
        
        print("=========================================")
        print("Expected validation error:")
        Sarah = CrewMember(
            member_id="USA001",
            name="Sarah Proche",
            rank=Rank.CADET,
            age=35,
            specialization="Mission Command",
            years_experience=8
        )

        Louis = CrewMember(
            member_id="EU001",
            name="Louis Vuitton",
            rank=Rank.CADET,
            age=45,
            specialization="Navigation",
            years_experience=18
        )

        Alico = CrewMember(
            member_id="EU002",
            name="Alico Merveille",
            rank=Rank.OFFICER,
            age=28,
            specialization="Engineering",
            years_experience=4
        )

        Mars_colony = SpaceMission(
            mission_id="M2024_MARS",
            mission_name="Mars Colony Establishment",
            destination="Mars",
            launch_date=datetime(2024, 3, 24),
            duration_days=900,
            crew=[Sarah, Louis, Alico],
            mission_status="ongoing",
            budget_millions=2500.0
        )

    except ValidationError as e:
        for err in e.errors():
            print(err["ctx"]['error'])
    except ValueError as e:
        print(e)

       
