from west.commands import WestCommand

import shutil
import subprocess

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

        cmd = 'west build -b nrf52840dk/nrf52840'
        print('Running command: ' + cmd)
        p = subprocess.run(cmd, shell=True)
        p.check_returncode() # Raises an exception if the return code is not 0
