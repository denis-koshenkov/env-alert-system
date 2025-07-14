# Introduction
Environmental alert system.

# Installtion
1. Install `pip`, because it is needed to install `west`:
    ```
    sudo apt install python3-pip
    ```

2. Install `west`:
    ```
    pip3 install west
    ```

3. Install nRF Util, including its prerequisites, by following the [instructions](https://docs.nordicsemi.com/bundle/nrfutil/page/guides/installing.html). The prerquisites are:
    - SEGGER J-Link
    - libusb-1.0-0
    - nrf-udev

4. Install the `device` command for `nrfutil`:
    ```
    nrfutil install device
    ```
    Check [here](https://docs.nordicsemi.com/bundle/nrfutil/page/guides/installing_commands.html) for more advanced installation information.

5. Install Docker by following the [instructions](https://docs.docker.com/engine/install/ubuntu/).

6. Make sure `docker` can be run as non-root user by following the [instructions](https://docs.docker.com/engine/install/linux-postinstall/).

7. Clone the project and its dependencies:
    ```
    west init -m git@github.com:denis-koshenkov/env-alert-system env-alert-system-project
    cd env-alert-system-project
    west update
    ```

Explanation:
- `west` is used to clone the project source code and the source code of third-party dependencies, such as `nrf-sdk`.
- nRF Util `device` command is used for flashing the firmware to the target hardware.
- `docker` is used for containerization of the build system. All dependencies necessary for producing the firmware binary are managed by the docker image, so they do not have to be installed on the host machine.

# Building
The following should be run from the `env-alert-system` directory.

Build the docker image:
```
docker build -f Dockerfile -t env-alert-system ..
```

Start the docker container based on that image:
```
docker run --rm -it -v "..:/workdir/env-alert-system-project" env-alert-system:latest bash
```

Inside the docker container, build the firmware:
```
west build -b nrf52840dk/nrf52840 -p
```

Or build the firmware using CMake directly:
```
cmake -GNinja -Bbuild -DBOARD=nrf52840dk/nrf52840 -DAPP_DIR=. ../nrf-sdk/zephyr/share/sysbuild
ninja -Cbuild
```

Build unit tests:
```
cmake -GNinja -Bbuild -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/gcc.cmake -DBUILD_TESTS=ON
ninja -Cbuild
```

The resulting firmware is the `build/merged.hex` file.

## Rebuilding the Docker image
In the usual workflow, it is not necessary to rebuild the docker image. However, the docker image should be rebuilt when the version of `nrf-sdk` used for this project is updated.

The reason for this is that the docker image uses various `requirements*.txt` files to install python dependencies necessary for `nrf-sdk`, `zephyr`, and `mcuboot`. It also uses the `nrf-sdk/zephyr/SDK_VERSION` file to determine the version of the Zephyr SDK to be installed.

As the `nrf-sdk` version changes, the dependencies listed in the `requirements*.txt` may change. The `SDK_VERSION` that is used in `zephyr` may also change. Because of this, if any of the following files are changed, the docker image needs to be rebuilt:
- `nrf-sdk/zephyr/scripts/requirements*.txt` - `*` stands for wildcard, as there are several requirements files the names of which start with `requirements` . If any of these files change, the docker image should be rebuilt. 
- `nrf-sdk/nrf/scripts/requirements*.txt` - the wildcard explanation above also applies here.
- `nrf-sdk/bootloader/mcuboot/scripts/requirements.txt`
- `nrf-sdk/zephyr/SDK_VERSION`

In order not to keep track if these files were updated as a part of a `nrf-sdk` version update, it is easier to always rebuild the docker image whenever the `nrf-sdk` version used in this project changes. In order to rebuild the docker image, use the `docker build` command mentioned above.


# Flashing
Run the following outside the docker container from the `env-alert-system` directory:
```
nrfutil device program --firmware build/merged.hex --options chip_erase_mode=ERASE_ALL,reset=RESET_SYSTEM
```

Flashing the firmware happens outside of the docker container. The purpose of the docker container is create the firmware binary in a reproducable manner, not upload the firmware to the target hardware. The reasoning for this is to allow developers to use GUI-based debuggers (e.g. SEGGER Ozone) which is impossible from inside the docker container.
