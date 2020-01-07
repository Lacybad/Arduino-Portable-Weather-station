# Arduino-Portable-Weather-station
An arduino based portable weather station using arduino Uno/Nano/Mega2560 and a 2.4 Parallel TFT LCD and a BME280 I2C sensor.
(ILI9320/9325/9328/9340/9341) Other driver IC not really supported by library.

I have made the sketch to run the lcd with arduinos reset pin by skipping the Analog 4 pin, which is commonly used to reset the display.
Pls take a look at this instructable how to achieve this: 
https://www.instructables.com/id/24-TFT-LCD-With-I2C-Modules/

If you are intrested using a Nano with a pcb adapter you might read this instructable and download this great pcb gerber files.
https://www.instructables.com/id/Arduino-Nano-to-Arduino-Uno-Adapter/#C9MP4FOJZD6RTO9

Note: Please exuse me for the quality of materials i have used. Theese are a bit defective modules and only good for prototyping.
