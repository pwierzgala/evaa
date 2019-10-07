FROM ubuntu:18.04

RUN apt-get update && apt-get install -y \
    apt-utils \
    sudo \
    build-essential \
    gcc-avr \
    avr-libc \
    avrdude \
    git \
    libpq-dev \
    python3 \
    python3-pip \
    libgtk-3-dev \
    libpulse-dev

RUN pip3 install --upgrade pip \
    psycopg2==2.8.3 \
    flask==1.1.1 \
    pyserial==3.4 \
    crcmod==1.7 \
    pathlib2==2.3.4 \
    robotframework==3.1.2 \
    RPi.GPIO==0.7.0 \
    apscheduler==3.6.1

RUN mkdir /evaa
WORKDIR /evaa
