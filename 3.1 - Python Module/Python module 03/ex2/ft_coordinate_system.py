import math


def get_player_pos() -> tuple:
    while True:
        raw_coord = input("Enter new coordinates as floats in format 'x,y,z': ")
        coord = raw_coord.split(',')
        if len(coord) != 3:
            print("Invalid syntax")
            continue

        x_str = coord[0].strip()
        y_str = coord[1].strip()
        z_str = coord[2].strip()

        try:
            x = float(x_str)
            y = float(y_str)
            z = float(z_str)
            return (x, y, z)
        except ValueError as e:
            bad_value = str(e).split("'")[1]
            print(f"Error on parameter '{bad_value}': {e}")


def main() -> None:
    print("=== Game Coordinate System ===")
    print("Get a first set of coordinate")
    coords = get_player_pos()
    print(f"Got a first tuple: {coords}")
    print(f"It includes: X={coords[0]}, Y={coords[1]}, Z={coords[2]}")
    print(f"Distance to center: {round(math.sqrt(coords[0]**2 + coords[1]**2 + coords[2]**2), 4)}")
    print("Get a second set of coordinate")
    coords2 = get_player_pos()
    distance = math.sqrt((coords2[0] - coords[0])**2 + (coords2[1] - coords[1])**2 + (coords2[2] - coords[2])**2)
    print(f"Distance between the 2 sets of coordinates: {round(distance, 4)}")

if __name__ == "__main__":
    main()
