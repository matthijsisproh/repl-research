from azure.storage.blob import BlobServiceClient
import pandas as pd
import time

STORAGEACCOUNTURL= "https://storageafstuderen.blob.core.windows.net"
STORAGEACCOUNTKEY= "znWCu4IObuL+BHP1q7wcyweJa8bcxHE+paQ9H/3303dKZ0OedBMIxzUw7jhDpFJNzsLbGitlOPMj+AStJbaN8w=="
LOCALFILENAME= "mqtt_blob_36.csv"
CONTAINERNAME= "raw/91d34f09-839b-446e-93b5-674baf0a1af6/23/2023/04/16/17/36"
BLOBNAME= "yawj46lcpo3yw"

#download from blob
t1=time.time()
blob_service_client_instance = BlobServiceClient(account_url=STORAGEACCOUNTURL, credential=STORAGEACCOUNTKEY)
blob_client_instance = blob_service_client_instance.get_blob_client(CONTAINERNAME, BLOBNAME, snapshot=None)
with open(LOCALFILENAME, "wb") as my_blob:
    blob_data = blob_client_instance.download_blob()
    blob_data.readinto(my_blob)
t2=time.time()
print(("It takes %s seconds to download "+BLOBNAME) % (t2 - t1))

# LOCALFILE is the file path
dataframe_blobdata = pd.read_csv(LOCALFILENAME)
