

xChange = -50
yChange = 0



for i in range(2, 3):
    file = open(str(i)+".txt", "r")

    lineList = []
    for line in file:
        lineList.append(line.strip())
        
    j = int (0)
    for line in lineList:

        if len(line) >1:
        

            val = []
            val = line.split(",")
            x = int(val[1])
            y = int(val[2])


            if (int (val[4]) >= 2):
                x = x + xChange
                y = y + yChange

            val[0] = val[0] + ","
            val[1] = str(x) + ","
            val[2] = str(y) + ","
            
            val[3] = val[3] + ","

            val[4] = val[4] + "\n"


            newLine = val[0] + val[1] + val[2] + val[3] + val[4]

            lineList[j] = newLine

            j = j+1

    file.close
    wFile = open(str(i)+".txt", "w")
    for line in lineList:
        wFile.write(line)