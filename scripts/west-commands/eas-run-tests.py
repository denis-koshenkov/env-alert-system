from west.commands import WestCommand
from west.manifest import Manifest

import shutil
import subprocess
from pathlib import Path

class EasRunTests(WestCommand):

    def __init__(self):
        super().__init__(
            'eas-run-tests',
            'Build and run unit test executable',
            ''
        )


    def do_add_parser(self, parser_adder):
        parser = parser_adder.add_parser(self.name,
                                        help=self.help,
                                        description=self.description)
        return parser


    def do_run(self, args, unknown_args):
        cmake = shutil.which('cmake')
        if cmake is None:
            print('CMake is not installed or cannot be found; cannot build.')
            return

        manifest = Manifest.from_topdir()
        build_dir_path = Path(manifest.repo_abspath) / 'build'
        source_dir_path = Path(manifest.repo_abspath)
        toolchain_file_path = Path(manifest.repo_abspath) / 'cmake' / 'toolchains' / 'gcc.cmake'

        cmd = cmake + ' -GNinja -B' + str(build_dir_path) + ' -S' + str(source_dir_path) + \
            ' -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_TOOLCHAIN_FILE=' + str(toolchain_file_path) + ' -DBUILD_TESTS=ON'
        print('Running command: ' + cmd)
        p = subprocess.run(cmd, shell=True)
        p.check_returncode() # Raises an exception if the return code is not 0

        cmd = cmake + ' --build ' + str(build_dir_path) + ' --' 
        print('Running command: ' + cmd)
        p = subprocess.run(cmd, shell=True)
        p.check_returncode() # Raises an exception if the return code is not 0

        # Run the produced test executable
        # TODO: run all test executables, probably using CTest
        app_tests_executable_path = Path(manifest.repo_abspath) / 'build' / 'test' / 'exec1' / 'test_exec1'
        cmd = str(app_tests_executable_path)
        print('Running command: ' + cmd)
        p = subprocess.run(cmd, shell=True)
        p.check_returncode() # Raises an exception if the return code is not 0
