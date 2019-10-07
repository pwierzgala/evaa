class WaterLevelAction:

    NOTHING = 0
    ADD = 1

    @classmethod
    def values(cls):
        return [
            cls.NOTHING,
            cls.ADD
        ]
