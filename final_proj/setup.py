from setuptools import Extension, setup

module = Extension("symmnfAPI", sources=['symmnf.c', 'symmnfmodule.c'])
setup(name='symmnfAPI',
      version='1.0',
      description='Python wrapper for custom C extension',
      ext_modules=[module])
