from setuptools import Extension, setup

module = Extension("symmnfmodule", sources=['symmnfmodule.c'])
setup(name='symmnfmodule',
      version='1.0',
      description='Python wrapper for custom C extension',
      ext_modules=[module])
