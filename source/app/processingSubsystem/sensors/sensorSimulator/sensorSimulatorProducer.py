import sys
import time
import random
import ctypes
import signal
from multiprocessing import shared_memory

class SensorData(ctypes.Structure):
    _fields_ = [
        ('rain_ctr', ctypes.c_int),
        ('windSpeed_ctr', ctypes.c_int),
        ('windDir_adc', ctypes.c_int),
    ]

running = True  # Global flag to control the simulation loop
sensor_data = None

def handle_exit(signum, frame):
    global running
    print(f"\nReceived signal {signum}, stopping simulation...")
    running = False

def simulateTickPluviometer():
    global sensor_data
    rainIncrement = random.randint(0, 1)
    sensor_data.rain_ctr += rainIncrement
    print(f"Rain incremented by {rainIncrement}, total: {sensor_data.rain_ctr}")
    time.sleep(1)

def simulateTickAnemometer():
    windSpeedIncrement = random.randint(0, 1)
    sensor_data.windSpeed_ctr += windSpeedIncrement
    print(f"Wind Speed incremented by {windSpeedIncrement}, total: {sensor_data.windSpeed_ctr}")
    time.sleep(1)

def main():
    if len(sys.argv) < 2:
        print("Usage: python sim_sensors.py <shared_memory_name>")
        sys.exit(1)

    global sensor_data

    sharedMemoryName = sys.argv[1]
    shm = shared_memory.SharedMemory(name=sharedMemoryName)

    # Register signal handlers for SIGTERM and SIGINT (Ctrl+C)
    signal.signal(signal.SIGTERM, handle_exit)
    signal.signal(signal.SIGINT, handle_exit)

    try:
        sensor_data = SensorData.from_buffer(shm.buf)

        print("Sensor simulation started. Waiting for termination signal...")
        while running:
            simulateTickPluviometer()
            simulateTickAnemometer()
            
    finally:
        del sensor_data
        shm.close()
        print("Shared memory closed. Exiting.")

if __name__ == "__main__":
    main()
