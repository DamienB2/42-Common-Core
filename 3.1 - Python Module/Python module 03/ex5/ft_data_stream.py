import random
from typing import Generator

def gen_event() -> Generator[tuple[str, str], None, None]:
    names_list = ['Alice', 'Bob', 'Charlie', 'Dylan']
    action_list = ['run', 'eat', 'sleep', 'grab', 'move', 'climb', 'swim', 'use', 'release']
    while True:
        yield (random.choice(names_list), random.choice(action_list))


def consume_event(events: list[tuple[str, str]]) -> Generator[tuple[str, str], None, None]:
    while len(events) > 0:
        event_id = random.randrange(len(events))
        poped_event = events.pop(event_id)
        yield poped_event
        


def main() -> None:
    print("=== Game Data Stream Processor ===")

    gen = gen_event()
    for i in range(1000):
        name, action = next(gen)
        print(f"Event {i}: Player {name} did action {action}")

    event_list = []
    for i in range(10):
        event_list.append(next(gen))
    
    print(f"Build list of 10 events: {event_list}")

    for consumed_event in consume_event(event_list):
        print(f"Got event from list: {consumed_event}")
        print(f"Remains in list: {event_list}")



if __name__ == "__main__":
    main()
