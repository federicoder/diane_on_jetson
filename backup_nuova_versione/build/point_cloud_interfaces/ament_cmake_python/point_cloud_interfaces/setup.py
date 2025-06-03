from setuptools import find_packages
from setuptools import setup

setup(
    name='point_cloud_interfaces',
    version='1.0.11',
    packages=find_packages(
        include=('point_cloud_interfaces', 'point_cloud_interfaces.*')),
)
