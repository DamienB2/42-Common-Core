def ft_count_harvest_recursive() -> None:
    days = int(input("Days until harvest: "))

    def count_days(current_day: int) -> None:
        if current_day > days:
            print("Harvest time!")
            return
        print("Day", current_day)
        count_days(current_day + 1)
    count_days(1)
