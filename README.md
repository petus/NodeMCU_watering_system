# NodeMCU_watering_system
Watering system for a few plants. The main component is WiFi module NodeMCU with ESP12E (ESP8266).<br>
The device contains two switching parts for pump/valve or another device. <br>
<br>
Websites:
(CZ) https://chiptron.cz
(EN) https://time4ee.com
<br>
Important components: <br>
Relays (Coil Resistance 500 ohm, Nominal Coil Voltage 5 VDC, Switching Voltage 200 VDC, Switching Current 0.5 A) https://www.gme.cz/data/attachments/dsh.634-072.1.pdf <br>
<br>
Transistors (Ultra Low On-Resistance, N-Channel MOSFET, using for switching from GPIO) https://www.infineon.com/dgdl/irlml2502.pdf?fileId=5546d462533600a401535667f44d2602<br>
<br>
Description of device:<br>
NodeMCU watering system includes two outputs (through relays), it can switch water pump, valve or another device. NodeMCU contains one analog input for measurement of soil moisture.<br>
The device contains pin header for connecting of temperature/humidity sensor (by I2C) like Si7021, two LEDs which can be used for indication.<br>
The watering system can be powered by external power supply (the board contains LF33 voltage regulator).<br>
