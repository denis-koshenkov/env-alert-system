from west.commands import WestCommand
from west.manifest import Manifest

import shutil
import subprocess
from pathlib import Path
import os

class EasBuildNrf(WestCommand):

    def __init__(self):
        super().__init__(
            'eas-build-nrf',
            'Build firmware for nrf52840 MCU',
            ''
        )


    def do_add_parser(self, parser_adder):
        parser = parser_adder.add_parser(self.name,
                                        help=self.help,
                                        description=self.description)
        return parser


    def do_run(self, args, unknown_args):
        west = shutil.which('west')
        if west is None:
            print('west is not installed or cannot be found; cannot build.')
            return

        manifest = Manifest.from_topdir()
        conf_file_path = Path(manifest.repo_abspath) / 'src' / 'port' / 'nrf52840dk' / 'zephyr' / 'prj.conf'
        dt_overlay_path = Path(manifest.repo_abspath) / 'src' / 'port' / 'nrf52840dk' / 'zephyr' / 'boards' / 'nrf52840_nrf52840dk.overlay'

        cmd = ['west', 'build', '-b', 'nrf52840dk/nrf52840', '--', '-DCONF_FILE=' + str(conf_file_path)]
        print('Running command: ' + ' '.join(cmd))
        p = subprocess.run(cmd, check=True)

        # Rename firmware file to a descriptive and not zephyr-specific filename
        src_path = Path(manifest.repo_abspath) / 'build' / 'merged.hex'
        dest_path = Path(manifest.repo_abspath) / 'build' / 'eas.hex'
        os.rename(src_path, dest_path)
