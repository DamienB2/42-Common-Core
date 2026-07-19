from .dark_spellbook import dark_spell_allowed_ingredients

def validate_ingredients(ingredients: str) -> str:
    allowed = dark_spell_allowed_ingredients()
    ingredients_lower = ingredients.lower()

    is_valid = False
    for ingredient in allowed:
        if ingredient.lower() in ingredients_lower:
            is_valid = True
            break

    if is_valid:
        return f"{ingredients} - VALID"
    return f"{ingredients} - INVALID"