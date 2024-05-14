from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

extensions = [Extension('wrapper',
                        ['wrapper.pyx', 'helper.cpp'],
                        language="c++",
                        extra_link_args=["-lz"]
                    )]
setup(
    name='wrapper', 
    ext_modules=cythonize(extensions, compiler_directives={'language_level' : "3"} )
)

