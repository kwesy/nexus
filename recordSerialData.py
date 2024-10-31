import time, serial, csv
from serial.tools import list_ports



# ports = list_ports.comports()
# for port in ports:
#     print(port)

serialCom = serial.Serial('/dev/ttyUSB0', '9600')
serialCom.setDTR(False)
time.sleep(1)
serialCom.flushInput()
serialCom.setDTR(True)

samples = 5000 # number of sample to collect

f = open('data_point.csv', 'w', newline='')
f.truncate()

for sample in range(samples):

    try:
        # Read a line of data
        byte_data = serialCom.readline()

        #
        data = byte_data.decode("utf-8").strip('\r\n')
        print(data)

        data = data.split(" ")

        writter = csv.writer(f, delimiter=",")
        writter.writerow(data)
    except:
        print("[WARNING] Line skipped")

f.close()