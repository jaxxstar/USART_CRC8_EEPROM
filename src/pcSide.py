# Importing Libraries
import serial
import time
import sys
arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=2400, timeout=.1)

text = 'Finance Minister Arun Jaitley Tuesday hitout at former RBI governor Raghuram Rajan for predicting that the next banking crisis would be triggered by MSME lending, saying postmortem is easier than taking action when it was required. Rajan, who had as the chief economist at IMF warned of impending financial crisis of 2008, in a note to a parliamentary committee warned against ambitious credit targets and loan waivers, saying that they could be the sources of next banking crisis. Government should focus on sources of the next crisis, not just the last one.In particular, government should refrain from setting ambitious credit targets or waiving loans. Credit targets are sometimes achieved by abandoning appropriate due diligence, creating the environment for future NPAs," Rajan said in the note." Both MUDRA loans as well as the Kisan Credit Card, while popular, have to be examined more closely for potential credit risk. Rajan, who was RBI governor for three years till September 2016, is currently.*'
end = 0
try:
    for i in text:
        print (i)
        start = time.time()
        arduino.write(bytes(i, 'utf-8'))
        # if(arduino.readline().decode('utf-8') == '+'):
        #     pass
        #     print("CRC8 error caught")
            # i = i-1
        end = time.time()
        print(f"Write Speed = {int(400/(end-start))} bps")
        if i=='*':
            start = time.time()
            # for i in text:
            data=arduino.readline().decode('utf-8')
            end = time.time()
            
            print(data)
            
            print(f"Read Speed = {int(8*sys.getsizeof(data)/(end-start))} bps")
        
except Exception as e:
    print(e)