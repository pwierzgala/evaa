from db_controller import add_measurement_to_db
from message import DataTypes, Commands
import logging


class MessageHandler:
    def __init__(self):
        self.ack_processes = { 
            DataTypes.temperature_1_wire_sensor: self.temperature_ack,
            DataTypes.blue_plant_light: self.blue_light_ack
        }

        self.response_processes = { 
            DataTypes.temperature_1_wire_sensor: self.temperature_response,
            DataTypes.blue_plant_light: self.blue_light_response
        }

    def get_processes(self, command):
        if command == Commands.ACK:
            return self.ack_processes
        elif command == Commands.RESPONSE:
            return self.response_processes
        else:
            return dict()

    def process(self, command, data_type, data):
        processes = self.get_processes(command)
        if data_type in processes:
            processes[data_type](data)

# Temperature
    def temperature_ack(self, data):
        logging.info("Temperature ACK")

    def temperature_response(self, data):
        logging.info("Temperature RESPONSE")
        id_hex = f"""{hex(int.from_bytes(data[0], "little"))[2:]}""" \
                 f"""{hex(int.from_bytes(data[1], "little"))[2:]}"""
        sensor_id = f"""temperature_{id_hex}"""
        temperature = float(int.from_bytes(data[2], "little")) / 1000.0
        add_measurement_to_db(sensor_id, temperature)

# Blue light
    def blue_light_ack(self, data):
        logging.info("Blue light ACK")

    def blue_light_response(self, data):
        logging.info("Blue light RESPONSE")

