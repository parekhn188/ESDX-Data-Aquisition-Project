#*********************************************************************  
#*                          Final Project                            
#*                       McMaster University                         
#*                      2DP4 Microcontrollers                        
#*                          Lab Section 03                           
#*                    Nisarg Parekh  MACID 400129104                             
#*********************************************************************
#*********************************************************************
#*                          Final Project                               
#*            Script to calculate the baud divisors/baud rate 
#*                         for a 6MHz clock                                                                                              
#*********************************************************************
# Clock Speed is 6.00 MHz as given in the requirements                    
#*********************************************************************
# 
#*********************************************************************

#Define all standard baud rates
possibleRates = [2400, 4800, 9600, 19200, 38400, 57600]
#Define array for percent error and baud divisors 
percentErr = [] 
divisorArr = []
#Clock speed definition 
busClk = 6e6 

#Go through all standard baud rates to find %error
for rate in possibleRates:
    #Calculate the divisor and append to divisor array  
    currentDivisor = busClk/(16*rate)
    divisorArr.append(currentDivisor) 
    
    #Calculate approximate rate 
    currentRate = busClk/(round(currentDivisor) * 16) 
    #Calculate percent error and append to percent error array 
    currentErr = (abs((currentRate - rate)) / rate) * 100
    percentErr.append(currentErr)


#output both arrays to analyze the values 
print(percentErr)
print(divisorArr) 



















