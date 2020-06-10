# DAS

*This directory contains scripts to log and test various components of the data data-acquisition-system*

## **Basic Setup and Usage**
These steps assume that you are in the data-acquisition-system root directory.
1. `cd DAS`
2. `poetry install`
3. `poetry shell`
4. `cd ../`
5. Run any DAS command, eg. `python -m DAS.mqtt_wireless_logger`

<br>

## **[MQTT Wireless Logger](https://github.com/monash-human-power/data-acquisition-system/blob/master/DAS/mqtt_wireless_logger.py)**
Once started this script will log any data coming in on the MQTT module data and battery channels. The saved csv files will be stored in the `DAS/csv_data`. 

**Usage**: From the data-acquisition-system root directory run `python -m DAS.mqtt_wireless_logger --host 192.168.1.100`. This will connect to the MQTT broker set to 192.168.1.100 and run a logger indefinitely. If no host is specified it will default to `localhost`.

<br>

## **[Fake Module Tester](https://github.com/monash-human-power/data-acquisition-system/blob/master/DAS/tests/fake_module_tester.py)**
This script mocks module data over MQTT similar to the real sensors on V3.

**Usage**: From the data-acquisition-system root directory run `python -m DAS.tests.fake_module_tester --host 192.168.1.100 --time 10 --rate 2 --id 3`. This will run a fake test with the MQTT broker on 192.168.1.100 for 10 seconds, pinging out data 2 times per second only for module 3.





