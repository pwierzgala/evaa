import psycopg2

db_connect_str = "dbname='evaa'" + \
                 "user='evaa'" + \
                 "host='localhost'" + \
                 "password='salata'"


def add_measurement_to_db(sensor_id, value):
    db_conn = psycopg2.connect(db_connect_str)
    db_cursor = db_conn.cursor()

    db_cursor.execute("INSERT INTO measurements (sensor_id, timestamp, value) "
                      f"VALUES ('{sensor_id}', current_timestamp, {value})")
    db_conn.commit()
    db_cursor.close()
    db_conn.close()
