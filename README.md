Arduino-Xively
==============

Temperature Monitoring using Arduino, 2x TMP36 and Xively.

You'll need to download the [Xively Arduino Client](https://github.com/xively/xively_arduino) and place it on your system along with the [HTTP Client](https://github.com/amcewen/HttpClient). That's it as far as software goes, however for the hardware you'll need two TMP36 temperature sensors; you should wire them up as follows, GROUND(TMP36) to GROUND(Arduino) for both, VIN(TMP36) to the 3.3 pin out(Arduino) for both TMP's and take a feed from the Arduino's 3.3v pin out and place it into the AREF pin on the Arduino.

###Configuration

- You should first ensure that the mac address I've used is okay for your network, currently DE:AD:BE:EF:00:00
- Provide your own API Key with write permisson to the channels
- Ensure you have two channels, one called indoor and the other called outdoor exactly
- Change the feed number to your own feed number
- Have fun and push / fork your improvements!


You can view my live example over on [Xively](https://xively.com/feeds/974153165)
