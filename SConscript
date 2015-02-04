Import ('env')

env.Append(CXXFLAGS=['--std=c++0x'])

name = 'fideo'
inc = env.Dir('.')
ext_inc = []
src = env.Glob('src/*.cpp') 
deps = ['etilico', 'mili', 'biopp', 'stl-debug', 'unafold', 'rnahybrid', 'vienna1.8.5', 'intarna', 'vienna2.0.7', 'inforna']

env.CreateSharedLibrary(name, inc, ext_inc, src, deps)
