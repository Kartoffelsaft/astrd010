import os
import sys

BUILD_DIR = '.build'
SOURCE_DIR = 'src'

BUILD_DIR_SHADER = BUILD_DIR + '/shaders'
BUILD_DIR_SOURCE = BUILD_DIR + '/' + SOURCE_DIR

env = Environment(
    CCFLAGS=['-Wall', '-std=c++20', '-ffile-prefix-map='+BUILD_DIR_SOURCE+'='+SOURCE_DIR],
    LIBS=['raylib'],
    CPPPATH=['./includes']
)

if ARGUMENTS.get('debug', 0):
    env.Append(CCFLAGS=[
        '-DDEBUG',
        '-O0',
        '-g3'
    ])

if ARGUMENTS.get('release', 0):
    env.Append(CCFLAGS=['-O3'])

env.VariantDir(BUILD_DIR_SOURCE, 'src')
processSource = env.Object(source=Glob(BUILD_DIR_SOURCE + '/*.cpp'))

buildGame = env.Program(target='game', source=[processSource])
env.Default('game')
