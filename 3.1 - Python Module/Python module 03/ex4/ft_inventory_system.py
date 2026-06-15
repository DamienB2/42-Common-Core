import sys


def fill_inventory(inventory: dict) -> None:
    for item in sys.argv[1:]:
        if ":" in item:
            name, quantity = item.split(":")
            try:
                quantity = int(quantity)
                if name in inventory:
                    print(f"Redundant item '{name}' - discarding")
                else:
                    inventory[name] = quantity
            except ValueError as e:
                print(f"Quantity error for '{name}': {e}")
        else:
            print(f"Error - invalid parameter '{item}'")


def main() -> None:
    print("=== Inventory System Analysis ===")

    inventory = {}
    fill_inventory(inventory)
    if inventory:
        total_quantity = sum(inventory.values())

        print(f"Got inventory: {inventory}")
        print(f"item list: {list(inventory.keys())}")
        print(f"Total quantity of the {len(inventory.keys())} items: {total_quantity}")

        for name, quantity in inventory.items():
            print(f"Item {name} represents {round((quantity / total_quantity) * 100, 1)}%")

    
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
