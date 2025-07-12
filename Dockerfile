# syntax=docker/dockerfile:1
FROM ubuntu:24.04

# Add extra repositories for the dependencies below
RUN apt-get update && apt-get install -y wget
RUN wget https://apt.kitware.com/kitware-archive.sh
RUN bash kitware-archive.sh

# TODO: remove unnecessary packages, this is from official zephyr installation guide
RUN apt-get update && apt-get install -y --no-install-recommends git cmake ninja-build gperf \
  ccache dfu-util device-tree-compiler wget \
  python3-dev python3-pip python3-setuptools python3-tk python3-wheel xz-utils file \
  make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1 \
  python3-venv

# Create and activate python venv
ENV VIRTUAL_ENV=/home/dev/.venv
RUN python3 -m venv $VIRTUAL_ENV
ENV PATH="$VIRTUAL_ENV/bin:$PATH"

# Install west
RUN pip3 install west

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
	wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v${ZSDK_VERSION}/zephyr-sdk-${ZSDK_VERSION}_linux-${HOSTTYPE}.tar.xz
	tar xf zephyr-sdk-${ZSDK_VERSION}_linux-${HOSTTYPE}.tar.xz
	zephyr-sdk-${ZSDK_VERSION}/setup.sh -t arm-zephyr-eabi -h -c
	rm zephyr-sdk-${ZSDK_VERSION}_linux-${HOSTTYPE}.tar.xz
EOF

# Create user account
ARG USERNAME=user
ARG UID=1000
ARG GID=1000
RUN <<EOF
	# Remove 'ubuntu' user to free UID 1000
	userdel -r ubuntu || true

	# Create user group and user account
	groupadd -g $GID -o $USERNAME
	useradd -u $UID -m -g $USERNAME -G plugdev $USERNAME

	# Add user account to sudoers
  mkdir etc/sudoers.d/
	echo $USERNAME ' ALL = NOPASSWD: ALL' > /etc/sudoers.d/$USERNAME
	chmod 0440 /etc/sudoers.d/$USERNAME
EOF

RUN apt-get update && apt-get install -y --no-install-recommends sudo

# Run the Zephyr SDK setup script as 'user' in order to ensure that the
# `Zephyr-sdk` CMake package is located in the package registry under the
# user's home directory.
USER $USERNAME

RUN <<EOF
  export ZSDK_VERSION="$(cat tmp/ZEPHYR_SDK_VERSION)"
  echo "The variable contains: $ZSDK_VERSION"
	sudo -E -- bash -c '
	/opt/toolchains/zephyr-sdk-${ZSDK_VERSION}/setup.sh -c &&
	chown -R $USERNAME:$USERNAME /home/$USERNAME/.cmake
	'
EOF

USER root

RUN rm /tmp/ZEPHYR_SDK_VERSION

# Set ZEPHYR_BASE instead of exporting Zephyr CMake package
# find_package(Zephyr) will work as long as ZEPHYR_BASE is set, so exporting the Zephyr CMake package is not necessary
ENV ZEPHYR_BASE=/home/dev/env-alert-system-project/nrf-sdk/zephyr

# This is done instead of `source zephyr-env.sh`. ZEPHYR_BASE is already set above, all that is left is to add $ZEPHYR_BASE/scripts to $PATH
ENV PATH="$ZEPHYR_BASE/scripts:$PATH"

ENV ZEPHYR_TOOLCHAIN_VARIANT=zephyr

USER ${USERNAME}

# So that the user could immediately run `west build` in the container
WORKDIR /home/dev/env-alert-system-project/env-alert-system
