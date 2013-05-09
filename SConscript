Import ('env')

name = 'fideo'
inc = env.Dir('.')
ext_inc = []
src = env.Glob('src/*.cpp') 
deps = ['mili', 'biopp', 'stl-debug']

env.CreateSharedLibrary(name, inc, ext_inc, src, deps)

