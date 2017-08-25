NoA - Car
Baudrate
AMB : 15200 / Kid_car : 9600
setting 
Line 13 of SerialComm.cpp
serial.ConfigurePort(CBR_115200, 8, FALSE, NOPARITY, ONESTOPBIT); => AMB
serial.ConfigurePort(CBR_9600, 8, FALSE, NOPARITY, ONESTOPBIT); => Kid_car

