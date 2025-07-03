# Introduction
Environmental alert system.

# Installtion
1. Install prerequisites for nRF Connect SDK:

    - nRF Util with `device` command installed
    - SEGGER J-Link
    - `nrf-udev` module
    - nRF Connect SDK toolchain - should be the same version as the version of the `nrf` project specified in `west.yml`

    See the [nRF Connect SDK installation guide](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/installation/install_ncs.html) for details on how to install each of these tools.

2. Clone the project and its dependencies:
    ```
    west init -m git@github.com:denis-koshenkov/env-alert-system env-alert-system-project
    cd env-alert-system-project
    west update
    ```

3. Export Zephyr CMake package:
    ```
    west zephyr-export
    ```

3. Install python dependencies:
    ```
    pip3 install -r zephyr/scripts/requirements.txt
    pip3 install -r nrf/scripts/requirements.txt
    pip3 install -r bootloader/mcuboot/scripts/requirements.txt
    ``` 

4. Install the Zephyr SDK:
    ```
    cd zephyr
    west sdk install
    ```

5. Set the following Zephyr environmental variables:
    ```
    export ZEPHYR_BASE=<absolute_path_to_proj>/env-alert-system-project/zephyr
    export ZEPHYR_SDK_INSTALL_DIR=<absolute_path_to_zephyr_sdk>/zephyr-sdk-<version>
    ```
    Zephyr SDK is installed by default into the home directory, i.e. `~`, so the path to the Zephyr SDK is likely:
    ```
    ZEPHYR_SDK_INSTALL_DIR=~/zephyr-sdk-<version>
    ```

# Installation using Docker
TODO: install docker instructions

docker build -t env-alert-system .

# Building
```
west build -b nrf52840dk/nrf52840 -p
```

# Flashing
```
west flash
```
