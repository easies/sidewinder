#!/usr/bin/env python
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules = [
    Extension('sidewinder._sidewinder', [
        'sidewinder/_sidewinder.pyx', 'sidewinder/sidewinder.c'
    ]),
]

setup(
    name='sidewinder',
    version='0.1.0',
    author='Alex Lee',
    author_email='alex@thirdbeat.com',
    url='http://sidewinder.thirdbeat.com',
    description='Utility to transfer file descriptors using (send|recv)msg.',
    long_description='''
Utility package to transfer file descriptors using sendmsg and recvmsg.
''',
    classifiers=[
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Cython',
        'Programming Language :: C',
        'Programming Language :: Python',
    ],
    license='MIT License',
    cmdclass={'build_ext': build_ext},
    packages=['sidewinder'],
    ext_modules = ext_modules
)
