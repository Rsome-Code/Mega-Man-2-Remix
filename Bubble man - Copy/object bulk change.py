

xChange = 8 * (16*4)
yChange = 0



for i in range(8, 14):
    file = open(str(i)+"-objects.txt", "r")

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

           
            val[0] = val[0] + ","
            val[1] = str(x + xChange) + ","
            val[2] = val[2]+ ","
            #val[3] = val[3] + ","
            val[3] = val[3] + "\n"


            newLine = val[0] + val[1] + val[2] + val[3]

            lineList[j] = newLine

            j = j+1

    file.close
    wFile = open(str(i)+"-objects.txt", "w")
    for line in lineList:
        wFile.write(line)