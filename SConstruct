
nameProgram = 'Evacuation'

agents = ['EvacAgent']
world = 'EvacWorld'
namespaceAgents = ['Evacuation']

srcFiles = ['main.cxx', 'EvacConfig.cxx', 'EvacAgent.cxx', 'EvacWorld.cxx']

###################################################
########## END OF CUSTOM INFORMATION  #############
##### don't modify anything below these lines #####
###################################################

import os, sys
from subprocess import call

pandoraPath = os.getenv('PANDORAPATH', '/usr/local/pandora')
sys.path.append(pandoraPath+'/bin')

import generateMpi 

vars = Variables('custom.py')
vars.Add(BoolVariable('debug', 'compile with debug flags', 'no'))
vars.Add(BoolVariable('edebug', 'compile with extreme debug logs', 'no'))

env = Environment(variables=vars, ENV=os.environ, CXX='mpicxx')
Help(vars.GenerateHelpText(env))

generateMPICodeBuilder = Builder(action=generateMpi.execute)
env.Append( BUILDERS = {'GenerateMPICode' : generateMPICodeBuilder})

env.Append(LINKFLAGS = '-fopenmp')
env.Append(CCFLAGS = '-std=c++0x -DTIXML_USE_STL'.split())
if env['debug'] == True:
    env.Append(CCFLAGS = '-g -O0 -Wall -DPANDORADEBUG'.split())
    if env['edebug']==True:
        env.Append(CCFLAGS = '-DPANDORAEDEBUG')
    env.Append(LIBS = 'pandorad')
else:
    env.Append(CCFLAGS = '-Ofast'.split())
    env.Append(LIBS = 'pandora')

env.Append(CPPPATH = ['.', pandoraPath+'/include'])
env.Append(LIBPATH = [pandoraPath+'/lib'])

# add the list of mpi code that must be generated & compiled
mpiAgentsSrc = ['mpiCode/FactoryCode.cxx']
agentsSrc = ['main.cxx']
for agent in agents:    
    if agent != '':
        agentsSrc.append(agent+".cxx")
        mpiAgentsSrc.append("mpiCode/"+agent+"_mpi.cxx")

env['namespaces'] = namespaceAgents
if (len(namespaceAgents) != len(agents)):
	print("The number of agents and name spaces declared should match!")
	sys.exit(1)
env.GenerateMPICode( target=mpiAgentsSrc, source=agentsSrc)
env.Depends(world+'.hxx',mpiAgentsSrc)
env.Program(nameProgram, srcFiles+mpiAgentsSrc)

