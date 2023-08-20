from setuptools import Extension, setup

module = Extension("symnmfAPI", sources=['symnmfmodule.c'])
setup(name='symnmfAPI',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])
