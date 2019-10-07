class DayHourStateSimulation:

    @staticmethod
    def get_new_state(day, hour):
        delta_day, new_hour = divmod(hour + 1, 24)
        new_day = day + delta_day
        return new_day, new_hour

