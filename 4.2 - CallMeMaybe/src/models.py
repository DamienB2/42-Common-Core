from pydantic import BaseModel
from typing import Literal


class PromptInput(BaseModel):
    """A natural-language request read from the input file.

    Attributes:
        prompt: The text of the request.
    """

    prompt: str


class TypeDefinition(BaseModel):
    """Type of a function parameter or return value.

    Attributes:
        type: One of "string", "number" or "boolean".
    """

    type: Literal["string", "number", "boolean", "integer"]


class FunctionDefinition(BaseModel):
    """Definition of a callable function, read from the definitions file.

    Attributes:
        name: Name of the function.
        description: Short description of what the function does.
        parameters: Mapping of parameter name to its type.
        returns: Type of the value returned by the function.
    """

    name: str
    description: str
    parameters: dict[str, TypeDefinition]
    returns: TypeDefinition


class FunctionCall(BaseModel):
    """A function call generated for a prompt (one entry of the output).

    Attributes:
        prompt: The original request.
        name: Name of the chosen function.
        parameters: Mapping of parameter name to its generated value.
    """

    prompt: str
    name: str
    parameters: dict[str, str | float | bool | int]