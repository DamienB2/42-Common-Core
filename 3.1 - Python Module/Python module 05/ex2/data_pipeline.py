from abc import ABC, abstractmethod
from typing import Any, Union, Protocol


class DataProcessor(ABC):
    def __init__(self) -> None:
        self._queue: list[str] = []
        self._counter: int = 0
        self._processed: int = 0

    @abstractmethod
    def validate(self, data: Any) -> bool:
        pass

    @abstractmethod
    def ingest(self, data: Any) -> None:
        self._processed += 1
        pass

    def output(self) -> tuple[int, str]:
        if not self._queue:
            raise IndexError("No data available")
        value = self._queue.pop(0)
        rank = self._counter
        self._counter += 1
        return rank, value


class ExportPlugin(Protocol):
    def process_output(self, data: list[tuple[int, str]]) -> None:

class DataStream():
    def __init__(self) -> None:
        self._processors: list[DataProcessor]= []

    def register_processor(self, proc: DataProcessor) -> None:
        if isinstance(proc, DataProcessor):
            self._processors.append(proc)

    def process_stream(self, stream: list[Any]) -> None:
        for item in stream:
            for proc in self._processors:
                if proc.validate(item):
                    proc.ingest(item)
                    break
            else:
                print(f"DataStream error - Can't process element in stream: {item}")

    def print_processors_stats(self):
        print("== DataStream statistics ==")
        if len(self._processors) != 0:
            for proc in self._processors:
                class_name = proc.__class__.__name__
                pos = class_name.find("Processor")
                class_name = class_name[:pos] + " " + class_name[pos:]

                print(f"{class_name}: total {proc._processed} items processed, remaining {len(proc._queue)} on processor")
        else:
            print("No processor found, no data")

    def output_pipeline(self, nb: int, plugin: ExportPlugin) -> None:
        test = test


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
            self._processed += 1
        else:
            for x in data:
                self._queue.append(str(x))
                self._processed += 1       


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
            self._processed += 1
        else:
            for x in data:
                self._queue.append(x)
                self._processed += 1


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
    
    def ingest(self, data: Union[dict[str, str], list[dict[str, str]]]) -> None:
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
            self._processed += 1


def main() -> None:
    print("=== Code Nexus - Data Pipeline ===")
    print()

    print("Initialize Data Stream...")
    data_stream = DataStream()
    data_stream.print_processors_stats()
    print()

    print("Registering Processors")
    numeric_processor = NumericProcessor()
    text_processor = TextProcessor()
    log_processor = LogProcessor()
    data_stream.register_processor(numeric_processor)
    data_stream.register_processor(text_processor)
    data_stream.register_processor(log_processor)
    print()

    batch = ['Hello world', [3.14, -1, 2.71], [{'log_level': 'WARNING', 'log_message': 'Telnet access! Use ssh instead'}, {'log_level': 'INFO', 'log_message': 'User wil is connected'}], 42, ['Hi', 'five']]
    print(f"Send first batch of data on stream: {batch}")
    data_stream.process_stream(batch)
    data_stream.print_processors_stats()
    print()


if __name__ == "__main__":
    main()
