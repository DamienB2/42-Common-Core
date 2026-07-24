from abc import ABC, abstractmethod
from typing import Any, Union


class DataProcessor(ABC):
    def __init__(self) -> None:
        self._queue: list[str] = []
        self._counter: int = 0

    @abstractmethod
    def validate(self, data: Any) -> bool:
        pass

    @abstractmethod
    def ingest(self, data: Any) -> None:
        pass

    def output(self) -> tuple[int, str]:
        if not self._queue:
            raise IndexError("No data available")
        value = self._queue.pop(0)
        rank = self._counter
        self._counter += 1
        return rank, value


class NumericProcessor(DataProcessor):
    def validate(self, data: Any) -> bool:
        if isinstance(data, (int, float)):
            return True

        if isinstance(data, list):
            return all(isinstance(x, (int, float)) for x in data)

        return False

    def ingest(self, data: Union[int, float, list[Union[int, float]]]) -> None:
        if not self.validate(data):
            raise ValueError("Improper numeric data")

        if isinstance(data, (int, float)):
            self._queue.append(str(data))
        else:
            for x in data:
                self._queue.append(str(x))


class TextProcessor(DataProcessor):
    def validate(self, data: Any) -> bool:
        if isinstance(data, str):
            return True

        if isinstance(data, list):
            return all(isinstance(x, str) for x in data)

        return False

    def ingest(self, data: Union[str, list[str]]) -> None:
        if not self.validate(data):
            raise ValueError("Improper text data")

        if isinstance(data, str):
            self._queue.append(data)
        else:
            for x in data:
                self._queue.append(x)


class LogProcessor(DataProcessor):
    def validate(self, data: Any) -> bool:
        if isinstance(data, dict):
            return all(
                isinstance(k, str) and isinstance(v, str)
                for k, v in data.items()
            )

        if isinstance(data, list):
            return all(
                isinstance(entry, dict)
                and all(
                    isinstance(k, str) and isinstance(v, str)
                    for k, v in entry.items()
                )
                for entry in data
            )
        return False

    def ingest(
        self, data: Union[dict[str, str], list[dict[str, str]]]
    ) -> None:
        if not self.validate(data):
            raise ValueError("Improper log data")

        if isinstance(data, dict):
            entries = [data]
        else:
            entries = data

        for entry in entries:
            level = entry.get("log_level", "").strip()
            message = entry.get("log_message", "")
            formatted = f"{level}: {message}"
            self._queue.append(formatted)


def main() -> None:
    print("=== Code Nexus - Data Processor ===")
    print()

    print("Testing Numeric Processor...")
    numeric_processor = NumericProcessor()
    print(f" Trying to validate input '42': {numeric_processor.validate(42)}")
    print(
        f" Trying to validate input 'Hello': {numeric_processor.validate('Hello')}"
    )
    try:
        print(
            " Test invalid ingestion of string 'foo' without prior validation:"
        )
        numeric_processor.ingest("foo")
    except Exception as e:
        print(" Got exception:", e)
    print(" Processing data: [1, 2, 3, 4, 5]")
    numeric_processor.ingest([1, 2, 3, 4, 5])
    print(" Extracting 3 values...")
    for i in range(3):
        rank, value = numeric_processor.output()
        print(f" Numeric value {rank}: {value}")
    print()

    print("Testing Text Processor...")
    text_processor = TextProcessor()
    print(f" Trying to validate input '42': {text_processor.validate(42)}")
    print(" Processing data: ['Hello', 'Nexus', 'World']")
    text_processor.ingest(["Hello", "Nexus", "World"])
    print(" Extracting 1 value...")
    rank, value = text_processor.output()
    print(f" Text value {rank}: {value}")
    print()

    print("Testing Log Processor...")
    log_processor = LogProcessor()
    print(
        f" Trying to validate input 'Hello': {log_processor.validate('Hello')}"
    )
    print(
        " Processing data: [{'log_level': 'NOTICE', 'log_message': 'Connection to server'}, {'log_level': 'ERROR', 'log_message': 'Unauthorized access!!'}]"
    )
    log_processor.ingest(
        [
            {"log_level": "NOTICE", "log_message": "Connection to server"},
            {"log_level": "ERROR", "log_message": "Unauthorized access!!"},
        ]
    )
    print(" Extracting 2 value...")
    for j in range(2):
        log_level, log_message = log_processor.output()
        print(f" Log entry {log_level}: {log_message}")
    print()


if __name__ == "__main__":
    main()
