import csv

with open('C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\Data\\mqtt_data.csv', 'r') as mqtt_data:
    mqtt_dict = csv.DictReader(mqtt_data)

    # mqtt_data.close()
    date_offset = "04/01/2023 12:00:00 PM"
    date_limit = "04/01/2023 13:00:00 PM"
    count_mqtt = 0
    for data in mqtt_dict:
        # print(data['Interval'])
        if(data['Interval'] >= date_offset):
            if(data['Interval'] <= date_limit):
                count_mqtt += 1
                print(data['Interval'])

    print(f"Succesfull requests on MQTT protocol: {count_mqtt}")