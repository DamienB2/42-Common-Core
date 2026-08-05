from pydantic import BaseModel, Field, model_validator, ValidationError
from enum import Enum
from datetime import datetime
from typing import Optional

class ContactType(Enum):
    RADIO = "radio"
    VISUAL = "visual"
    PHYSICAL = "physical"
    TELEPATHIC = "telepathic"

class AlienContact(BaseModel):
    contact_id: str = Field(min_length=5, max_length=15)
    timestamp: datetime
    location: str = Field(min_length=3, max_length=100)
    contact_type: ContactType
    signal_strength: float = Field(ge=0.0, le=10.0)
    duration_minutes: int = Field(ge=1, le=1440)
    witness_count: int = Field(ge=1, le=100)
    message_received: Optional[str] = Field(default=None, max_length=500)
    is_verified: bool = Field(default=False)

    @model_validator(mode="after")
    def check(self) -> "AlienContact":
        if not self.contact_id.startswith("AC"):
            raise ValueError('Contact ID must start with "AC"')
        elif self.contact_type.name == "PHYSICAL" and self.is_verified == False:
            raise ValueError('Physical contact must be verified')
        elif self.contact_type.name == "TELEPATHIC" and self.witness_count < 3:
            raise ValueError('Telepathic contact requires at least 3 witnesses')
        elif self.signal_strength > 7.0 and self.message_received == None:
            raise ValueError('Strong signals (> 7.0) should include received messages')
        else:
            return self


if __name__ == "__main__":
    print("Alien Contact Log Validation")
    print("======================================")
    print("Valid contact report:")
    try:
        alien_contact = AlienContact(
            contact_id="AC_2024_001",
            timestamp=datetime(2026, 7, 31),
            location="Area 51, Nevada",
            contact_type='radio',
            signal_strength=8.5,
            duration_minutes=45,
            witness_count=5,
            message_received="Greetings from Zeta Reticuli"
        )
        print(f"ID: {alien_contact.contact_id}")
        print(f"Type: {alien_contact.contact_type.value}")
        print(f"Location: {alien_contact.location}")
        print(f"Signal: {alien_contact.signal_strength}/10")
        print(f"Duration: {alien_contact.duration_minutes} minutes")
        print(f"Witnesses: {alien_contact.witness_count}")
        print(f"Message: '{alien_contact.message_received}'")
    except ValidationError as e:
        for err in e.errors():
            print(err["msg"])
    except ValueError as e:
        print(e)
    print()

    print("======================================")
    print("Expected validation error:")
    try:
        alien_contact = AlienContact(
            contact_id="AC_2024_001",
            timestamp=datetime(2026, 7, 31),
            location="Area 51, Nevada",
            contact_type='telepathic',
            signal_strength=8.5,
            duration_minutes=45,
            witness_count=2,
            message_received="Greetings from Zeta Reticuli"
        )

    except ValidationError as e:
        for err in e.errors():
            print(err["ctx"]['error'])
    except ValueError as e:
        print(e)