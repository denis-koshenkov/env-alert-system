# syntax=docker/dockerfile:1
FROM ubuntu:24.04

RUN apt update && apt upgrade

# Add extra repositories for the dependencies below
RUN wget https://apt.kitware.com/kitware-archive.sh
RUN bash kitware-archive.sh

# TODO: remove unnecessary packages, this is from official zephyr installation guide
# TODO: check CMake version that gets installed, maybe install the newest one manually
RUN apt install -y --no-install-recommends git cmake ninja-build gperf \
  ccache dfu-util device-tree-compiler wget \
  python3-dev python3-pip python3-setuptools python3-tk python3-wheel xz-utils file \
  make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1 \
  python3-venv

# TODO: check the installed versions of cmake, python3, dts here

# Create and activate python venv
RUN python3 -m venv /home/dev/.venv
RUN source /home/dev/.venv/bin/activate

# Install west
RUN pip3 install west

# Export Zephyr CMake package
WORKDIR /home/dev/env-alert-system-project
RUN west zephyr-export

# Install Python dependencies
WORKDIR /home/dev/env-alert-system-project/nrf-sdk
pip3 install -r zephyr/scripts/requirements.txt
pip3 install -r nrf/scripts/requirements.txt
pip3 install -r bootloader/mcuboot/scripts/requirements.txt

# Install Zephyr SDK
WORKDIR /home/dev/env-alert-system-project/nrf-sdk/zephyr
RUN west sdk install --install-dir /home/dev/toolchains

# Define Zephyr-related environment variables
RUN source zephyr-env.sh
