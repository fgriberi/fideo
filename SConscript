Import ('env')

name = 'fideo'
inc = env.Dir('.')
ext_inc = []
src = env.Glob('src/*.cpp') 
deps = ['etilico', 'mili', 'biopp', 'stl-debug']

env.CreateSharedLibrary(name, inc, ext_inc, src, deps)
