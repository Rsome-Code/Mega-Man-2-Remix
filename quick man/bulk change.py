

xChange = 0
yChange = 0



#for i in range(0, 8):
file = open("9"+".txt", "r")

lineList = []
for line in file:
    lineList.append(line.strip())
    
j = int (0)
for line in lineList:

    if len(line) >1:
    

        val = []
        val = line.split(",")
        x = int(val[1]) + xChange
        y = int(val[2]) + yChange


       # if (val[0] == "0-0" or val[0] == "0-1" or val[0] == "0-2" or val[0] == "0-3" or val[0] == "0-4" or val[0] == "0-5"):
        #    val[0] = val[0] + "-3"
        
        val[0] = val[0] + ","
        val[1] = str(x) +","
        val[2] = str(y) + ","
        

        val[3] = val[3] + ","

        val[4] = val[4] + "\n"


        newLine = val[0] + val[1] + val[2] + val[3] + val[4]

        lineList[j] = newLine

        j = j+1

file.close
wFile = open("10"+".txt", "w")
for line in lineList:
    wFile.write(line)