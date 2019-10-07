from flask import Flask, make_response, jsonify
from plant_controller import PlantController
from apscheduler.schedulers.background import BackgroundScheduler as Scheduler

app = Flask(__name__)


def make_json_respond(input_func):
    def _decorator(*args, **kwargs):
        try:
            result = input_func(*args, **kwargs)
        except Exception as e:
            return make_response(jsonify(e), 500)
        return jsonify(success=True, result=result)

    return _decorator


@app.route('/')
def hello():
    return """EVAA REST API
        (not ready yet, but we are building as fast as we can :))"""


@app.route('/light_height/<int:plant_id>/<int:percentage>/',
           methods=['GET'])
def set_light_height(plant_id, percentage):
    return make_json_respond(controllers[plant_id].set_light_height)(
        percentage)


@app.route('/red_led_intensity/<int:plant_id>/<int:percentage>/',
           methods=['GET'])
def set_red_light_intensity(plant_id, percentage):
    return make_json_respond(controllers[plant_id].set_red_light_intensity)(
        percentage)


@app.route('/blue_light_intensity/<int:plant_id>/<int:percentage>/',
           methods=['GET'])
def set_blue_light_intensity(plant_id, percentage):
    return make_json_respond(controllers[plant_id].set_blue_light_intensity)(
        percentage)


@app.route('/ventilation_level/<int:plant_id>/<int:percentage>/',
           methods=['GET'])
def set_ventilation_level(plant_id, percentage):
    return make_json_respond(controllers[plant_id].set_ventilation_level)(
        percentage)


@app.route('/pomp_in/<int:plant_id>/<int:pomp_id>/<int:amount_ml>/',
           methods=['GET'])
def pomp_in(plant_id, pomp_id, amount_ml):
    return make_json_respond(controllers[plant_id].pomp_in)(pomp_id, amount_ml)


@app.route('/get_temperature/<int:plant_id>/<int:sensor_id>/', methods=['GET'])
def get_temperature(plant_id, sensor_id):
    return make_json_respond(controllers[plant_id].get_temperature)(sensor_id)


@app.route('/get_water_level/<int:plant_id>/', methods=['GET'])
def get_water_level(plant_id):
    return make_json_respond(controllers[plant_id].get_water_level)()


@app.route('/get_humidity/<int:plant_id>/', methods=['GET'])
def get_humidity(plant_id):
    return make_json_respond(controllers[plant_id].get_humidity)()


@app.route('/get_ph/<int:plant_id>/', methods=['GET'])
def get_ph(plant_id):
    return make_json_respond(controllers[plant_id].get_ph)()


@app.route('/set_relay_state/<int:sensor_id>/<int:state>/', methods=['GET'])
def set_relay_state(sensor_id, state):
    return make_json_respond(controllers[sensor_id].set_relay_state)(
        state)


@app.route('/led_power/<int:plant_id>/<int:status>/', methods=['GET'])
def led_power(plant_id, status):
    return make_json_respond(controllers[plant_id].led_power)(status)


def init_controllers():
    my_list = [PlantController(plant_id=0, device_name="/dev/ttyUSB0"),
               PlantController(plant_id=1, device_name="/dev/ttyUSB1"),
               PlantController(plant_id=2, device_name="/dev/ttyUSB2"),
               PlantController(plant_id=3, device_name="/dev/ttyUSB3"),
               PlantController(plant_id=4, device_name="/dev/ttyUSB4"),
               PlantController(plant_id=5, device_name="/dev/ttyUSB5"),
               PlantController(plant_id=6, device_name="/dev/ttyUSB6"),
               PlantController(plant_id=7, device_name="/dev/ttyUSB7"),
               PlantController(plant_id=8, device_name="/dev/ttyUSB8"),
               PlantController(plant_id=9, device_name="/dev/ttyUSB9")]
    return my_list


if __name__ == '__main__':
    controllers = init_controllers()
    scheduler = Scheduler()

    scheduler.add_job(controllers[0].get_temperature,
                      trigger='interval', args=[1], seconds=5)

    scheduler.start()
    app.run(host='0.0.0.0')
