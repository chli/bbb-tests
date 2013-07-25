A collection of tests run on the BeagleBone Black with different attached peripherals. As described on [Inbedded](http://blog.inbedded.net):  

 * lcd-test  : Bouncing object on Nokia 3330's LCD attached to BeagleBone Black.
 * leds-demo : Random stuff with LED strip
 
Compilation and deployment on the BBB :
    
    CROSS_COMPILE=arm-linux-gnueabihf TARGET=chli@192.168.0.11:/home/chli/workspace/ make install
