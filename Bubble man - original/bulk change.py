

xChange = 3
yChange = 0



#for i in range(4, 5):
file = open(str(5)+".txt", "r")

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

        
        val[0] = val[0] + ","
        val[1] = str(x) +","
        val[2] = str(y) + ","
        

        val[3] = val[3] + ","

        val[4] = val[4] + "\n"


        newLine = val[0] + val[1] + val[2] + val[3] + val[4]

        lineList[j] = newLine

        j = j+1

file.close
wFile = open(str(5)+".txt", "w")
for line in lineList:
    wFile.write(line)