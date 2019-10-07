#!/usr/bin/python3

import psycopg2
import time

filename = ['res_1_wire_dev_0.txt', 'res_1_wire_dev_1.txt']

db_connect_str = "dbname='evaa'" + \
                 "user='arek'" + \
                 "host='localhost'" + \
                 "password='salata'"     # ROTFL password in plaintext


def make_sensor_id(file):
    return file.split(".")[0]


def make_timestamp(line):
    return int((line.split('\t')[0]).split(' ')[2])


def make_value(line):
    return float(line.split('\t')[1])


def connect_to_db(db_conn_str):
    return psycopg2.connect(db_conn_str)


def add_measurement_to_db(db_conn, db_cursor, sensor_id, timestamp, value):
    db_cursor.execute("INSERT INTO measurements (sensor_id, timestamp, value) " + \
                     f"""VALUES ('{sensor_id}', current_timestamp, {value})""")
    db_conn.commit()

#
# program main code below
#

# open connection with database
try:
    conn = connect_to_db(db_connect_str)
    cursor = conn.cursor()
except Exception as e:
    print(e)

# read data from file and place them in database
for file in filename:
    time.sleep(1)
    try:
        data = open(file, 'r')
        for line in data:
            add_measurement_to_db(conn,
                                  cursor,
                                  make_sensor_id(file),
                                  make_timestamp(line),
                                  make_value(line))
    except Exception as e:
        print(e)

# close connection with database
try:
    cursor.close()
    conn.close()
except Exception as e:
    print(e)
