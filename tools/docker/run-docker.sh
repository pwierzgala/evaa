#!/bin/sh

MOUNT_USB_DEVS_PARAM=""
EVAA_SOURCES_DIR=""
REPO_ROOT_DIR_NAME=$(basename $(dirname $(dirname $(pwd)))) # name of ../.. folder
REPO_ROOT_DIR=$(dirname $(dirname $(pwd)))                  # absolute path of ../.. folder

# if 1st parameter == "build-n-flash" mount USB devices to Docker container
if [ "$1" = "build-n-flash" ]; then
    MOUNT_USB_DEVS_PARAM="--privileged -v /dev:/dev"
fi

# if ../.. directory is named the same as root folder of EVAA repository mount
# it to the container, else mount current directory
if [ "$REPO_ROOT_DIR_NAME" = "evaa" ]; then
    EVAA_SOURCES_DIR=$REPO_ROOT_DIR
else
    EVAA_SOURCES_DIR=$(pwd)
fi

docker run -i -t --rm \
    -v ${EVAA_SOURCES_DIR}:/evaa \
    ${MOUNT_USB_DEVS_PARAM} \
    evaa-docker \
    /bin/bash
