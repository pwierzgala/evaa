#!/bin/sh

sudo mkdir -p /opt/evaa-rest-api
sudo cp -r rpi/rest_api_app/* /opt/evaa-rest-api/
sudo cp rpi/rest_api_app/evaa-rest-api.service  /etc/systemd/system/

sudo systemctl enable evaa-rest-api.service
sudo systemctl start evaa-rest-api.service
