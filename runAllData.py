def run(name, method):
    import pylab as p
    import shutil, subprocess
    import sys

    path = "./outData/"
    inifilename = "mybody.ini"

    
    
    inifile = open(inifilename, 'r')
    tmpinifile = open(inifilename + ".tmp", "w")

    for line in inifile:
        if line[0:18] == "unbindingMethod = ":
            line = "unbindingMethod = " + str(method)+"\n "
        if line[0:9] == "inFile = ":
            line = "inFile = " + str(name)+"\n "
            
        tmpinifile.write(line)
    
    inifile.close()
    tmpinifile.close()
    shutil.move(inifilename + ".tmp", inifilename)

    #Run my halofinder
    #print "Running for: " +"ScaleDensity="+ str(ScaleDensity) + " b=" + str(b) + " f="+str(f)
    subprocess.call(["mpirun","-n","50", "./MORPH"])
    
    #Read data from files
    filename = "Data_" + name[:-4] + "_UnbindingMethod=" + str(method)
    shutil.move(path + "data.dat", path + filename + ".dat")
    filename2 = "Mass_" + name[:-4] + "_UnbindingMethod=" + str(method)
    shutil.move(path + "mass.dat", path + filename2 + ".dat")
    


#names = ["lcdm1.bin"]
names = ["lcdm1.bin", "symm_A.bin", "symm_B.bin", "symm_C.bin", "symm_D.bin", "fofr4.bin", "fofr5.bin", "fofr6.bin"]
count = 1.
runs = len(names)*3

infile = open("bounding.dat", "w")
infile.write("#dataset unboundingmethod percentage")
infile.close()
for name in names:
    for method in range(3):
        print "----------------------------"
        print (count/runs)*100
        print "----------------------------"
        run(name,method)
        count += 1
    


