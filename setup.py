#!/usr/bin/env python
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules = [
    Extension('sidewinder._sidewinder', ['sidewinder/_sidewinder.pyx', 'sidewinder/sidewinder.c']),
]

setup(
    name='Sidewinder',
    version='0.0.1',
    author='Alex Lee',
    author_email='alex@thirdbeat.com',
    description='Utility to transfer file descriptors using (send|recv)msg.',
    cmdclass={'build_ext': build_ext},
    packages=['sidewinder'],
    ext_modules = ext_modules
)
