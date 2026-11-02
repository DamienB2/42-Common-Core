from pydantic import BaseModel
from typing import Literal


class PromptInput(BaseModel):
    prompt: str


class TypeDefinition(BaseModel):
    type: Literal["string", "number", "boolean"]


class FunctionDefinition(BaseModel):
    name: str
    description: str
    parameters: dict[str, TypeDefinition]
    returns: TypeDefinition
