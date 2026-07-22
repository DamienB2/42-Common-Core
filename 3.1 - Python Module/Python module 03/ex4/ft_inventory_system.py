import sys


def fill_inventory(inventory: dict[str, int]) -> None:
    for item in sys.argv[1:]:
        parts = item.split(":")
        if len(parts) != 2:
            print(f"Error - invalid parameter '{item}'")
            continue

        name, quantity_str = parts
        try:
            quantity = int(quantity_str)
        except ValueError as e:
            print(f"Quantity error for '{name}': {e}")
            continue

        if name in inventory:
            print(f"Redundant item '{name}' - discarding")
        else:
            inventory[name] = quantity


def main() -> None:
    print("=== Inventory System Analysis ===")

    inventory: dict[str, int] = {}
    fill_inventory(inventory)
    if inventory:
        total_quantity = sum(inventory.values())

        item_count = len(inventory.keys())
        print(f"Got inventory: {inventory}")
        print(f"Item list: {list(inventory.keys())}")
        print(f"Total quantity of the {item_count} items: {total_quantity}")

        for name, quantity in inventory.items():
            percentage = round((quantity / total_quantity) * 100, 1)
            print(f"Item {name} represents {percentage}%")

        items_list = list(inventory.items())
        first_item, first_quantity = items_list[0]

        max_item, min_item = first_item, first_item
        max_quantity, min_quantity = first_quantity, first_quantity

        for name, quantity in items_list[1:]:
            if quantity > max_quantity:
                max_quantity = quantity
                max_item = name
            if quantity < min_quantity:
                min_quantity = quantity
                min_item = name

        print(f"Item most abundant: {max_item} with quantity {max_quantity}")
        print(f"Item least abundant: {min_item} with quantity {min_quantity}")
        inventory.update({"magic_item": 1})
        print(f"Updated inventory: {inventory}")
    else:
        print("Your inventory is empty")


if __name__ == "__main__":
    main()
