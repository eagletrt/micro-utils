#!/usr/bin/env bash

DOCKER_DIR=/home/$USER/custom-message-sender

docker run --rm -it \
	-v "$(pwd)":$DOCKER_DIR \
	-w $DOCKER_DIR \
	gcc14-aarch64 \
	bash
