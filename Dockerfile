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

# Set ZEPHYR_BASE instead of exporting Zephyr CMake package
# find_package(Zephyr) will work as long as ZEPHYR_BASE is set, so exporting the Zephyr CMake package is not necessary
ENV ZEPHYR_BASE=/home/dev/env-alert-system-project/nrf-sdk/zephyr

# Install Python dependencies
COPY nrf-sdk/zephyr/scripts/requirements*.txt /tmp/requirements/zephyr/
RUN pip3 install -r /tmp/requirements/zephyr/requirements.txt

COPY nrf-sdk/nrf/scripts/requirements*.txt /tmp/requirements/nrf/
RUN pip3 install -r /tmp/requirements/nrf/requirements.txt

COPY nrf-sdk/bootloader/mcuboot/scripts/requirements.txt /tmp/requirements/bootloader/requirements.txt
RUN pip3 install -r /tmp/requirements/bootloader/requirements.txt

RUN rm -rf /tmp/requirements/

SHELL ["/bin/bash", "-eo", "pipefail", "-c"]

# Install Zephyr SDK
COPY nrf-sdk/zephyr/SDK_VERSION tmp/ZEPHYR_SDK_VERSION
RUN <<EOF
  ZSDK_VERSION="$(cat tmp/ZEPHYR_SDK_VERSION)"
  echo "The variable contains: $ZSDK_VERSION"
  mkdir -p /opt/toolchains
	cd /opt/toolchains
	wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v${ZSDK_VERSION}/zephyr-sdk-${ZSDK_VERSION}_linux-${HOSTTYPE}_minimal.tar.xz
	tar xf zephyr-sdk-${ZSDK_VERSION}_linux-${HOSTTYPE}_minimal.tar.xz
	zephyr-sdk-${ZSDK_VERSION}/setup.sh -t arm-zephyr-eabi -c
	rm zephyr-sdk-${ZSDK_VERSION}_linux-${HOSTTYPE}_minimal.tar.xz
EOF
RUN rm /tmp/ZEPHYR_SDK_VERSION

# This is done instead of `source zephyr-env.sh`. ZEPHYR_BASE is already set above, all that is left is to add $ZEPHYR_BASE/scripts to $PATH
ENV PATH="$ZEPHYR_BASE/scripts:$PATH"

ENV ZEPHYR_TOOLCHAIN_VARIANT=zephyr

# So that the user could immediately run `west build` in the container
WORKDIR /home/dev/env-alert-system-project/env-alert-system
