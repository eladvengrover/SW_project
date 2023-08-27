from setuptools import Extension, setup

module = Extension("symmnfAPI", sources=['symnmf.c', 'symnmfmodule.c'])
setup(name='symmnfAPI',
      version='1.0',
      description='Python wrapper for custom C extension',
      ext_modules=[module])
