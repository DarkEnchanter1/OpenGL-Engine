def FlagsForFile( filename, **kwargs ):
  return {
    'flags': [ '-x', 'c++', '-lm', '-lGLU', '-lGL', '-lglut', '-lGLEW', '-lglfw'],
  }

