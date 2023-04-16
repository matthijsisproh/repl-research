import csv
import os


# Results of blob files MQTT Protocol
blob_files = ["mqtt_blob_32.csv", "mqtt_blob_33.csv", "mqtt_blob_34.csv", "mqtt_blob_35.csv", "mqtt_blob_36.csv" ]

for blob in blob_files:
    with open(f'{os.getcwd()}/{blob}', 'r') as mqtt_data:
        mqtt_dict = csv.DictReader(mqtt_data)
        count = 0
        for data in mqtt_dict:
            count += 1
    
    print(f"File {__file__} has {count} successful data points")


# Results of blob files HTTP Protocol