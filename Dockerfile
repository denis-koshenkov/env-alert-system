# syntax=docker/dockerfile:1
FROM ubuntu:24.04

# Add extra repositories for the dependencies below
RUN apt-get update && apt-get install -y wget
RUN wget https://apt.kitware.com/kitware-archive.sh
RUN bash kitware-archive.sh

# TODO: remove unnecessary packages, this is from official zephyr installation guide
# TODO: check CMake version that gets installed, maybe install the newest one manually
RUN apt-get update && apt-get install -y --no-install-recommends git cmake ninja-build gperf \
  ccache dfu-util device-tree-compiler wget \
  python3-dev python3-pip python3-setuptools python3-tk python3-wheel xz-utils file \
  make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1 \
  python3-venv

# TODO: check the installed versions of cmake, python3, dts here

# Create and activate python venv
ENV VIRTUAL_ENV=/home/dev/.venv
RUN python3 -m venv $VIRTUAL_ENV
ENV PATH="$VIRTUAL_ENV/bin:$PATH"

# Install west
RUN pip3 install west

# COPY . /home/dev/env-alert-system-project
COPY env-alert-system/west.yml /home/dev/env-alert-system-project/env-alert-system/west.yml
COPY .west /home/dev/env-alert-system-project/.west

# Copy Zephyr west-related files to run Zephyr west commands 
COPY nrf-sdk/zephyr/west.yml /home/dev/env-alert-system-project/nrf-sdk/zephyr/west.yml
COPY nrf-sdk/zephyr/scripts/west-commands.yml /home/dev/env-alert-system-project/nrf-sdk/zephyr/scripts/west-commands.yml
COPY nrf-sdk/zephyr/scripts/west_commands /home/dev/env-alert-system-project/nrf-sdk/zephyr/scripts/west_commands

# Export Zephyr CMake package
WORKDIR /home/dev/env-alert-system-project
RUN west zephyr-export

# # Set ZEPHYR_BASE instead of exporting Zephyr CMake package
# # find_package(Zephyr) will work as long as ZEPHYR_BASE is set, so exporting the Zephyr CMake package is not necessary
# ENV ZEPHYR_BASE=/home/dev/env-alert-system-project/nrf-sdk/zephyr

# # Install Python dependencies
# # COPY nrf-sdk/zephyr /home/dev/env-alert-system-project/nrf-sdk/zephyr
# RUN pip3 install -r /home/dev/env-alert-system-project/nrf-sdk/zephyr/scripts/requirements.txt

# # COPY nrf-sdk/nrf /home/dev/env-alert-system-project/nrf-sdk/nrf
# RUN pip3 install -r /home/dev/env-alert-system-project/nrf-sdk/nrf/scripts/requirements.txt

# RUN --mount=type=bind,source=nrf-sdk/bootloader/mcuboot/scripts/requirements.txt,dst=/home/dev/requirements/bootloader/requirements.txt \
#   pip3 install -r /home/dev/requirements/bootloader/requirements.txt

# # Install Zephyr SDK
# WORKDIR /home/dev/env-alert-system-project/nrf-sdk/zephyr
# RUN west sdk install --install-dir /home/dev/toolchains

# # This is done instead of `source zephyr-env.sh`. ZEPHYR_BASE is already set above, all that is left is to add $ZEPHYR_BASE/scripts to $PATH
# ENV PATH="$ZEPHYR_BASE/scripts:$PATH"

# WORKDIR /home/dev/env-alert-system-project/env-alert-system

RUN rm -rf /home/dev/env-alert-system-project
