Import('env')

# Build mbed library
mbed_lib = env.MbedLikeLibrary(
    'mbed', 'mbed/libraries/mbed/',
    ['api/', 'common/', 'hal/', 'targets/cmsis/', 'targets/hal/'])
env.Prepend(LIBS = mbed_lib)

env.Append(LINKFLAGS=[
  '-Wl,--whole-archive',  # used to compile mbed HAL, which uses funky weak symbols
  mbed_lib,
  '-Wl,--no-whole-archive',
])

# env.Prepend(LIBS =
#   env.MbedLikeLibrary(
#     'mbed-rtos', 'mbed/libraries/rtos/',
#     ['rtx/', 'rtos/'])
# )
#
# env.Append(CPPPATH=[Dir('mbed/libraries/rpc/')])
# env.Prepend(LIBS =
#   env.StaticLibrary(
#     target='mbed-rpc',
#     source=Glob('mbed/libraries/rpc/*.cpp'))
# )

# Only compile warnings when compiling top-level code (not libraries)
env.Append(CCFLAGS = ['-Wall', '-Wextra'])

# Top-level build flow
prog = env.Program(
  target = 'main',
  source = Glob('*.cpp'),
)
env.Objcopy(prog)