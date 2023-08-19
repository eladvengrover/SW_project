from setuptools import Extension, setup

module = Extension("fit_capi", sources=['kmeansmodule.c'])
setup(name='fit_capi',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])

# TODO - change it according to this project
