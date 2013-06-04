Import ('env')

name = 'fideo'
inc = env.Dir('.')
ext_inc = []
src = env.Glob('src/*.cpp') 
deps = ['etilico', 'mili', 'biopp', 'stl-debug']

env.CreateSharedLibrary(name, inc, ext_inc, src, deps)

test_env = env.Clone()
test_env.Append(CPPDEFINES={'DEBUG_MODE' : '1'})
test_env.CreateSharedLibrary(name+'-test', inc, ext_inc, src, deps)

