import json
from datetime import datetime, timedelta
import os
from dateutil.parser import parse

# JSON-bestandspad en naam
json_file = "http_blob_1.json"

# Tijd waarnaar moet worden gezocht
target_time = parse("2023-04-16T15:56:27.2730000Z")

# Tolerantie in seconden
tolerance = 60

# Laad JSON-bestand in een Python-dictionary
with open(f'{os.getcwd()}\{json_file}', "r") as f:
    data = json.load(f)

# Tel het aantal datapunten binnen 60 seconden na de target_time
count = 0
for x in range(0, len(data)):
    enqueued_time = parse(data[x]['IoTHub']['EnqueuedTime'])
    if target_time <= enqueued_time <= target_time + timedelta(seconds=tolerance):
        count += 1


print(f"Aantal datapunten binnen {tolerance} seconden na {target_time}: {count}")