import csv
import os


# Results of blob files MQTT Protocol
blob_files = ["mqtt_blob_43.csv", "mqtt_blob_44.csv", "mqtt_blob_45.csv"]

for blob in blob_files:
    with open(f'{os.getcwd()}/Data/{blob}', 'r') as mqtt_data:
        mqtt_dict = csv.DictReader(mqtt_data)
        count = 0
        for data in mqtt_dict:
            count += 1
    
    print(f"File {__file__} has {count} successful data points")


# Results of blob files HTTP Protocol
