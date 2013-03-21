Import ('env')

name = 'fideo'
inc = env.Dir('.')
ext_inc = env.Dir('fideo')
src = env.Glob('src/*.cpp') 
deps = ['mili', 'biopp', 'stl-debug']

env.CreateSharedLibrary(name, inc, ext_inc, src, deps)

