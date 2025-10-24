

xChange = 0
yChange = 0



for i in range(0, 1):
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

           
            val[0] = val[0] + ","
            val[1] = val[1] + ","
            val[2] = val[2]+ ","
            
            val3 = ""
            if (val[3] == "2"):
                val[3] = "3,"
            elif (val[3] == "3"):
                val[3] = "2,"
            else:
                val[3] = val[3] + ","


            val[4] = val[4] + "\n"


            newLine = val[0] + val[1] + val[2] + val[3] + val[4]

            lineList[j] = newLine

            j = j+1

    file.close
    wFile = open(str(i)+".txt", "w")
    for line in lineList:
        wFile.write(line)