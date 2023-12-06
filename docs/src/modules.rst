=======
Modules
=======

Here are all the modules

.. contents:: :local:
    :depth: 2

API
---

.. doxygenclass:: api_lib
    :members:

Wifi
----

.. doxygenclass:: wifi_connection
    :members:

Dht
---

.. doxygenclass:: DHTSensor
    :members:

Logging
-------

.. doxygenclass:: myLogger
    :members:

SD
--

.. doxygenclass:: SDCustom
    :members:

Voltage Sensor
--------------

.. doxygenclass:: VoltageSensor
    :members:

Current Sensor
--------------

.. doxygenclass:: CurrentSensor
    :members:

Main code
---------

First all, we need to import all the modules we need. Then we will create akk the variables we will need later. After that, we need to create a logger object, for developpment purpose, it will be set to debug. This is useful to see all the messages in the console. To finish, we declare all the prototype for the functions.

Next we can write the main code. It starts with the setup :
 1. We establish the serial communication
 2. We declare the pins we will use
 3. We initialize the logger
 4. We initialize the wifi
 5. We setup the sensors

Then we can write the loop, the loop is basically a state machine. All the states are described in the following diagram :
    * ``INIT``: State during the setup function
    * ``CHECKING``: Check the device status (SD, wifi, ...)
    * ``FETCHING``: Fetch the data from the sensors
    * ``SENDING``: Send the data to the server
    * ``ERROR``: handle the errors
    * ``SLEEP``: Sleep for a certain amount of time

Now we will describe each state of the state machine :

Checking 
^^^^^^^^

Check wether the SD card is present or not. If it is not, we will disable the logging in the SD and enable it in the Serial.

Fetching 
^^^^^^^^

Sending 
^^^^^^^

Error 
^^^^^

Sleeep 
^^^^^^
