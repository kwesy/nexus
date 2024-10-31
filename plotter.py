import sys
import serial
import argparse
from collections import deque
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# plot class
class AnalogPlot:
    def __init__(self, strPort, maxLen):
        self.ser = serial.Serial(strPort, 9600, timeout=1)
        self.ax = deque([0.0] * maxLen)
        self.ay = deque([0.0] * maxLen)
        self.az = deque([0.0] * maxLen)
        self.aX = deque([0.0] * maxLen)
        self.aY = deque([0.0] * maxLen)
        self.aZ = deque([0.0] * maxLen)
        self.maxLen = maxLen

    def addToBuf(self, buf, val):
        if len(buf) < self.maxLen:
            buf.append(val)
        else:
            buf.popleft()
            buf.append(val)

    def add(self, data):
        assert len(data) >= 1
        self.addToBuf(self.ax, data[0]) # RotX
        self.addToBuf(self.ay, data[1]) # RotY
        self.addToBuf(self.az, data[2]) # RotZ
        # self.addToBuf(self.aX, data[3]) # RotZ
        # self.addToBuf(self.aY, data[4]) # RotZ
        # self.addToBuf(self.aZ, data[5]) # RotZ

    def update(self, frameNum, a0, a1, a2, a3, a4, a5):
        try:
            line = self.ser.readline().decode('utf-8').strip()
            data = [float(val) for val in line.split()]
            if len(data) >= 1:
                self.add(data)
                a0.set_data(range(self.maxLen), self.ax)
                a1.set_data(range(self.maxLen), self.ay)
                a2.set_data(range(self.maxLen), self.az)
                # a3.set_data(range(self.maxLen), self.aX)
                # a4.set_data(range(self.maxLen), self.aY)
                # a5.set_data(range(self.maxLen), self.aZ)
        except Exception as e:
            print(f"Error reading line: {e}")

        return a0, a1, a2, a3, a4, a5

    def close(self):
        self.ser.flush()
        self.ser.close()

def main():
    parser = argparse.ArgumentParser(description="LDR serial")
    parser.add_argument('--port', dest='port', required=True)
    args = parser.parse_args()

    strPort = args.port
    print(f'reading from serial port {strPort}...')

    analogPlot = AnalogPlot(strPort, 100)
    print('plotting data...')

    fig, ax = plt.subplots()
    ax.set_xlim(0, 100)
    ax.set_ylim(-500, 500)
    a0, = ax.plot([], [], label='accelx')
    a1, = ax.plot([], [], label='accely')
    a2, = ax.plot([], [], label='accelz')
    a3, = ax.plot([], [], label='angleX')
    a4, = ax.plot([], [], label='angleY')
    a5, = ax.plot([], [], label='angleZ')
    ax.legend()

    anim = animation.FuncAnimation(
        fig, analogPlot.update, fargs=(a0, a1, a2, a3, a4, a5), interval=50, blit=True
    )

    plt.show()
    analogPlot.close()
    print('exiting.')

if __name__ == '__main__':
    main()
