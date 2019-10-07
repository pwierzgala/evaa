import crcmod


class Commands:
    ACK = 0
    GET = 1
    SET = 2
    RESPONSE = 3


class DataTypes:
    error = 0
    temperature_1_wire_sensor = 1
    light_intensity_sensor = 2
    humidity_sensor = 3
    pomp_motor_data = 4
    elevator_motor_data = 5
    pressure_sensor = 6
    led_power = 7
    water_level_sensor = 8
    blue_plant_light = 9
    red_plant_light = 10
    ph_sensor = 11
    weight_sensor = 12
    ventilation_level = 13


class Message:
    length = 11

    def __init__(self, device_id, plant_id, command, data_type, data,
                 number_of_chunks=1, chunk_index=0):
        self.device_id = device_id
        self.plant_id = plant_id
        self.command = command
        self.data_type = data_type
        self.data = data
        self.number_of_chunks = number_of_chunks
        self.chunk_index = chunk_index

    @staticmethod
    def calculate_crc(data):
        crc8 = crcmod.predefined.mkPredefinedCrcFun('crc-8-maxim')
        return crc8(data)

    def create(self):
        message_frame = bytearray([self.device_id,
                                   self.plant_id,
                                   self.number_of_chunks,
                                   self.chunk_index,
                                   self.command,
                                   self.data_type])
        message_frame.extend(self.data.to_bytes(4, 'little'))
        crc = self.calculate_crc(message_frame)
        message_frame.append(crc)
        return message_frame
