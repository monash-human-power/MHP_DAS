import csv
import json
import argparse
import paho.mqtt.client as mqtt
import os
from datetime import datetime

"""
# USE 3 TEMP CSV and then merge at the end

1. Subscribe to the chanels
2. Capture and decode the JSON
3. Convert the json to csv
4. append the data to the csv file

# NOTE: The battery will be implemented later. Just do the key data first.

"""


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected Sucessfully! Result code: " + str(rc))

    else:
        print("Something went wrong! Result code: " + str(rc))

    # Subscribe to all of the data topics
    client.subscribe("/v3/wireless-module/1/data")
    client.subscribe("/v3/wireless-module/2/data")
    client.subscribe("/v3/wireless-module/3/data")

    # Subscribe to all of the battery topics
    client.subscribe("/v3/wireless-module/battery/low")
    client.subscribe("/v3/wireless-module/1/battery")
    client.subscribe("/v3/wireless-module/2/battery")
    client.subscribe("/v3/wireless-module/3/battery")


def on_message(client, userdata, msg):
    # Capture the data and decode the JSON

    # IMPLEMENT A FUNCTION FOR THIS
    # /v3/wireless-module/<id>/start
    # /v3/wireless-module/<id>/stop

    # Check to see if the topic ends in "data", selecting only the msg's that
    # have wireless_module_data
    if msg.topic[:19] == "/v3/wireless-module" and msg.topic[-4:] == "data":
        sensor_id = str(msg.topic[20])
        data_dict = parse_wireless_module_data(msg)
        temp_csv('S'+sensor_id, data_dict)


def merge_temps(output_filename):
    """Searches throught the current directory and finds all the temp files and merges them
    it finds a """
    current_dir = os.path.dirname(__file__)
    filenames = os.listdir(current_dir)

    temp_filenames = []

    for filename in filenames:
        # Find the temp files and store in the array
        if filename[-4:]==".csv" and filename[:6]==".~temp":
            temp_filenames.append(current_dir + '/' + filename)

    merge_temps_continued(output_filename, temp_filenames)

def merge_temps_continued(output_filename, temp_filenames):
    end_of_csv = False
    current_line_num = 0
    while not end_of_csv:

        merged_csv_fieldnames = []
        current_merged_dict = {}
        current_dir = os.path.dirname(__file__)
        merged_csv_path = os.path.join(current_dir, str(output_filename + '.csv'))

        for filename in temp_filenames:
            csv_file = open(filename, mode='r')
            csv_reader = csv.reader(csv_file, delimiter=',')

            line_count = 0
            for row in csv_reader:
                if line_count == 0:

                    print(row)
                else:
                    print('0')

                line_count += 1
                # for_row_num
                #
                # current_row_num
                # # First line
                # if line_num == 0:
                #
                #
                #     print('HEADERS -->', )
                # else:
                #     for i in range(line_num):
                #         if i == line_num:
                #             print()

        end_of_csv = True


        #write to merged csv
        with open(merged_csv_path, mode='a') as csv_file:
            csv_writer = csv.DictWriter(csv_file, fieldnames=merged_csv_fieldnames)
            csv_writer.writeheader()
            csv_writer.writerow(current_merged_dict)


def parse_wireless_module_data(msg):
    module_data = msg.payload.decode("utf-8")
    module_data = json.loads(module_data)
    sensor_data = module_data["sensors"]
    # data_dict will be used to store the data before being entered into the csv
    data_dict = {}

    for sensor in sensor_data:
        sensor_type = sensor["type"]
        sensor_value = sensor["value"]
        if isinstance(sensor_value, dict):
            # print(sensor_value)
            pass
        else:
            data_dict[sensor_type] = sensor_value

    # Add in the time and date that the data came in
    data_dict["datetime"] = str(datetime.now())

    return data_dict


def temp_csv(data_name, data_dict):
    # make sure that the temp file is
    current_dir = os.path.dirname(__file__)
    temp_file_path = os.path.join(current_dir, str('.~temp_' + data_name + '.csv'))

    fieldnames = []
    for key in data_dict.keys():
        fieldnames.append(key)
    # print(temp_file_path)

    if not os.path.exists(temp_file_path):
        # If the temp file does not exist write the headers for the CSV
        with open(temp_file_path, mode='a') as temp_file:
            csv_writer = csv.DictWriter(temp_file, fieldnames=fieldnames)
            csv_writer.writeheader()

    with open(temp_file_path, mode='a') as temp_file:
        csv_writer = csv.DictWriter(temp_file, fieldnames=fieldnames)
        csv_writer.writerow(data_dict)
        # csv_writer.writerow(['Erica Meyers', 'IT', 'March'])


if __name__ == "__main__":
    merge_temps('merged_output')
    # broker_address = 'localhost'
    # client = mqtt.Client()
    #
    # client.on_connect = on_connect
    # client.on_message = on_message
    #
    # client.connect(broker_address)
    #
    # client.loop_forever()
