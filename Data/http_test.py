

from base64 import b64encode, b64decode
from hashlib import sha256
from time import time
from urllib import parse
from hmac import HMAC

def generate_sas_token(uri, key, policy_name, expiry=3600):
    ttl = time() + expiry
    sign_key = "%s\n%d" % ((parse.quote_plus(uri)), int(ttl))
    print(sign_key)
    signature = b64encode(HMAC(b64decode(key), sign_key.encode('utf-8'), sha256).digest())

    rawtoken = {
        'sr' :  uri,
        'sig': signature,
        'se' : str(int(ttl))
    }

    if policy_name is not None:
        rawtoken['skn'] = policy_name

    return 'SharedAccessSignature ' + parse.urlencode(rawtoken)

import requests
import json

print("Start sending...")

iotHub = "temperatuursensoren"
deviceId = "dht11_http"
api = "2016-11-14" # "2016-02-03"
restUriPost = f"https://{iotHub}.azure-devices.net/devices/{deviceId}/messages/events?api-version={api}"
# sas = generate_sas_token("temperatuursensors.azure-devices.net", "lYKx0lSu3JSTqarZh4jwfCQyVFVof9kWYyDtT0KxoIc=", "iothubowner", 3600 )
# print(sas)


while True:
    sas = generate_sas_token("temperatuursensoren.azure-devices.net", "1vUD4bATBx05zqv7WX5JzY+OJo/ObLdOJ4ZTBWX4F0Q=", "iothubowner", 3600 )

    headers = {"Authorization": sas}

    body = {"Temperature": 24.6, "Humidity": 32, "Signal": "ok"}
    content = json.dumps(body)
    stringContent = content.encode('utf-8')

    resultPost = requests.post(restUriPost, data=stringContent, headers=headers)


    print(f"Sent {resultPost}")