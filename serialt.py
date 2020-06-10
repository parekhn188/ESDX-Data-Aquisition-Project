#*********************************************************************  
#*                          Final Project                            
#*                       McMaster University                         
#*                      2DP4 Microcontrollers                        
#*                          Lab Section 03                           
#*                    Nisarg Parekh  MACID 400129104                             
#*********************************************************************
#*********************************************************************
#*                          Final Project                               
#*              Live plotting script for the ADXL 337 
#*                                                                                                                             
#*********************************************************************
# Clock Speed is 6.00 MHz as given in the requirements                    
#*********************************************************************

#Import statements 
import serial 
import io
import matplotlib.pyplot as plt
from   matplotlib.animation import FuncAnimation
import datetime 


#Define serial port 
ser = serial.Serial() 

#Open serial port and define properties 
ser.port = "COM4"
ser.baudrate = 38400
ser.bytesize = serial.EIGHTBITS 
ser.parity = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
ser.xonxoff=1
ser.rtscts=0

#Define the ASCII incoming list from the serial port 
outlist = []
#Define the list to be plotted 
refinedList = []

#Configure the matplotlib plot 
fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

#Open serial port 
ser.open()

#Animation function for plotting 
def animate(i): 
    #Assign oulist to the incoming data 
    outlist = ser.readlines(1)
    #Parsing incoming data
    for value in outlist: 
        #Skip over any empty/invalid data 
        if (value == ''): 
            continue
        #Decode the ASCII value to a string 
        temp = value.decode('utf-8')
        #Strip newline character from all the strings 
        temp2 = temp.strip('\n')
        #Append only new values and values less than 95 degrees
        if(temp2 in refinedList or int(temp2) > 95):
            continue
        elif (temp2 not in refinedList):
            refinedList.append(int(temp2))
    #Clear the old plot 
    ax1.clear()       
    #Plot the new values 
    ax1.plot(refinedList)
    #Plot properties
    plt.title('Angle of accelerometer')
    plt.xlabel('Time (s)')
    plt.ylabel('Angle (Degrees)')

#Calls animation function to graph at an interval of 10ms 
ani = FuncAnimation(plt.gcf(), animate, interval = 10)
#Show the plot
plt.plot(refinedList)
plt.show()
#Close serial port 
ser.close()

