from functools import wraps
from typing import Callable, Any
from time import perf_counter


def timer(func: Callable) -> Callable:
    """Decorator that measures and prints execution time of a function.

    Args:
        func (Callable): The function to be timed.

    Returns:
        Callable: Wrapped function that prints execution timing logs.
    """
    @wraps(func)
    def wrapper(*args: str, **kwargs: int) -> Any:
        """Execute wrapped function, log execution time, and return result.

        Args:
            *args (str): Positional arguments passed to the function.
            **kwargs (int): Keyword arguments passed to the function.

        Returns:
            Any: The return value of the wrapped function.
        """
        print(f"executing {func.__name__}...")

        start_time = perf_counter()
        result = func(*args, **kwargs)
        end_time = perf_counter()

        execution_time = end_time - start_time
        print(f"algorithm completed in {execution_time:.3f} seconds")
        return result
    return wrapper
