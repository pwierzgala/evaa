import serial.serialutil
from serial_thread import SerialThread
from message import *
import logging
from db_controller import add_measurement_to_db


class PlantController:
    def __init__(self, plant_id, device_name):
        self.id = plant_id
        self.device_name = device_name
        self.enabled = False
        self.log_file = f"serial_log_{plant_id}.txt"
        try:
            self.thread = SerialThread(device_name, self.log_file)
            self.enabled = True
            self.thread.start()
        except serial.serialutil.SerialException:
            logging.error("Unable to open " + self.device_name)
        self.message = None

    @staticmethod
    def limit_input(value, bottom_limit=0, upper_limit=100):
        if value > upper_limit:
            value = upper_limit
        if value < bottom_limit:
            value = bottom_limit
        return value

    def set_light_height(self, height):
        height = self.limit_input(height, upper_limit=70000)
        self.message = Message(device_id=self.id,
                               plant_id=self.id,
                               command=Commands.SET,
                               data_type=DataTypes.elevator_motor_data,
                               data=height
                               ).create()
        if self.enabled:
            self.thread.send(self.message)

    def set_red_light_intensity(self, percentage):
        percentage = self.limit_input(percentage)
        duty_reg_max = 255
        duty_reg_val = int((percentage * duty_reg_max) / 100)
        self.message = Message(device_id=self.id,
                               plant_id=self.id,
                               command=Commands.SET,
                               data_type=DataTypes.blue_plant_light,
                               data=duty_reg_val
                               ).create()
        if self.enabled:
            add_measurement_to_db("plant_light_duty", percentage)
            if percentage == 0:
                add_measurement_to_db("plant_light_enable", 0)
            else:
                add_measurement_to_db("plant_light_enable", 1)
            self.thread.send(self.message)

    def set_blue_light_intensity(self, percentage):
        duty_reg_max = 255
        percentage = self.limit_input(percentage)
        duty_reg_val = int((percentage * duty_reg_max) / 100)
        self.message = Message(device_id=self.id,
                               plant_id=self.id,
                               command=Commands.SET,
                               data_type=DataTypes.blue_plant_light,
                               data=duty_reg_val
                               ).create()

        if self.enabled:
            add_measurement_to_db("plant_light_duty", percentage)
            if percentage == 0:
                add_measurement_to_db("plant_light_enable", 0)
            else:
                add_measurement_to_db("plant_light_enable", 1)
            self.thread.send(self.message)

    def set_ventilation_level(self, percentage):
        duty_reg_max = 255
        percentage = self.limit_input(percentage)
        duty_reg_val = int((percentage * duty_reg_max) / 100)
        self.message = Message(device_id=self.id,
                               plant_id=self.id,
                               command=Commands.SET,
                               data_type=DataTypes.ventilation_level,
                               data=duty_reg_val
                               ).create()
        if self.enabled:
            add_measurement_to_db("plant_ventilation", percentage)
            if percentage == 0:
                add_measurement_to_db("plant_ventilation_enable", 0)
            else:
                add_measurement_to_db("plant_ventilation_enable", 1)
            self.thread.send(self.message)

    def set_relay_state(self, state):
        self.message = Message(device_id=self.id,
                               plant_id=self.id,
                               command=Commands.SET,
                               data_type=DataTypes.led_power,
                               data=state).create()
        if self.enabled:
            self.thread.send(self.message)

    def pomp_in(self, pomp_id, amount_ml):
        raise NotImplementedError("To be implemented")

    def get_pressure(self, sensor_id):
        self.message = Message(device_id=self.id,
                               plant_id=self.id,
                               command=Commands.GET,
                               data_type=DataTypes.pressure_sensor,
                               data=sensor_id).create()
        if self.enabled:
            self.thread.send(self.message)

    def get_temperature(self, sensor_id):
        self.message = Message(device_id=self.id,
                               plant_id=self.id,
                               command=Commands.GET,
                               data_type=DataTypes.temperature_1_wire_sensor,
                               data=sensor_id).create()
        if self.enabled:
            self.thread.send(self.message)

    def get_water_level(self, sensor_id):
        self.message = Message(device_id=self.id,
                               plant_id=self.id,
                               command=Commands.GET,
                               data_type=DataTypes.water_level_sensor,
                               data=sensor_id
                               ).create()
        if self.enabled:
            self.thread.send(self.message)

    def get_humidity(self, sensor_id):
        self.message = Message(device_id=self.id,
                               plant_id=self.id,
                               command=Commands.GET,
                               data_type=DataTypes.humidity_sensor,
                               data=sensor_id
                               ).create()
        if self.enabled:
            self.thread.send(self.message)

    def get_ph(self, sensor_id):
        self.message = Message(device_id=self.id,
                               plant_id=self.id,
                               command=Commands.GET,
                               data_type=DataTypes.humidity_sensor,
                               data=sensor_id
                               ).create()
        if self.enabled:
            self.thread.send(self.message)

    def get_weight(self, sensor_id):
        self.message = Message(device_id=self.id,
                               plant_id=self.id,
                               command=Commands.GET,
                               data_type=DataTypes.weight_sensor,
                               data=sensor_id
                               ).create()
        if self.enabled:
            self.thread.send(self.message)

    def led_power(self, status):
        self.message = Message(device_id=self.id,
                               plant_id=self.id,
                               command=Commands.SET,
                               data_type=DataTypes.led_power,
                               data=status
                               ).create()
        if self.enabled:
            self.thread.send(self.message)
