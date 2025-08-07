import os
import sys
import subprocess
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from pathlib import Path

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed")
        super().run()

    def build_extension(self, ext):
        ext_fullpath = Path(self.get_ext_fullpath(ext.name))
        extdir = ext_fullpath.parent.resolve()

        cfg = 'Release'

        build_temp = Path(self.build_temp) / ext.name.replace('.', '_')
        build_temp.mkdir(parents=True, exist_ok=True)

        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE={cfg}",
        ]

        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=build_temp)
        subprocess.check_call(['cmake', '--build', '.', '--config', cfg], cwd=build_temp)


setup(
    name='cminiopy',
    version='0.1.0',
    author='Vadim Task',
    packages=['cminiopy'],
    package_dir={'cminiopy': 'cminiopy'},  # Указывает на папку
    ext_modules=[
        CMakeExtension('cminiopy._core', sourcedir='src'),
        CMakeExtension('cminiopy.sync', sourcedir='src/sync'),
    ],
    cmdclass={'build_ext': CMakeBuild},
    zip_safe=False,
)
