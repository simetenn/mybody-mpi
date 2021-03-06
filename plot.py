import pylab as p
import sys
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.cm as cm

try:
    name = str(sys.argv[1])
except:
    name = "positionsFOF.dat"
    #name = "positions_all.dat"
#print "Using file: " + name 

path = "./outData/"
savepath = "./plots/"
    
particles = [] 
with open(path+name, "r") as infile:
    for line in infile:
        particles.append(line.split())
        #data = []
        #for number in line.split():
        #    data.append(float(number))
            
        #particles.append(p.array(data))
    

particles = p.array(particles,float)
#print len(particles[:,1])
##particles = particles[::5,:]

"""
p.scatter(particles[:,0],particles[:,1], c=particles[:,-1])
p.show()
"""
for i in xrange(int(max(particles[:,-1]))+1):
    print i
    index = p.where(particles[:,-1] == i)

    fig = p.figure()
    ax = fig.add_subplot(111, projection='3d')
#ax.scatter(particles[:,0],particles[:,1],particles[:,2], s=1,c=particles[:,-1],linewidths=0.1)
    ax.scatter(particles[index,0],particles[index,1],particles[index,2], s=10,c=particles[index,-1],linewidths=0.1)
    #print particles[index,-1]
    ax.set_xlabel('X-position [Mpc/h]')
    ax.set_ylabel('Y-position [Mpc/h]')
    ax.set_zlabel('Z-position [Mpc/h]')
    ax.set_title("Positions")
    name = "SingleHalo"+ str(i)
    p.savefig(savepath+name[:-4]+".png")
    #p.show()

