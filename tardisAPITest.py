
import requests
import subprocess

bytesize = 1048576
count = 100
filename = "file1"
filesize = bytesize * count

tardis_server = "118.138.254.158"
url = "http://" + tardis_server  + "/api/v1/dataset_file/"
username = "testroce"
apikey = "testroceapikey"

subprocess.Popen( ["dd", "if=/dev/urandom", "of="+filename, 
    "bs="+str(bytesize), "count="+str(count)]);


#
#
#headers = {
#    "Authorization": "ApiKey %s:%s" % (username, apikey),
#    "Content-Type": "application/json",
#    "Accept": "application/json"
#}
#
#metadata = {
#   "dataset": "/api/v1/dataset/1/",
#   "filename": "file1",
#   "md5sum": "ab493cf004be87ab263dcfd58b1a044d",
#   "size": "33",
#   "mimetype": "application/octet-stream",
#   "replicas": [{
#       "uri": "dataset1/file1.txt",
#       "location": "testroce",
#       "protocol": "file"
#   }]
#}
#
#response = requests.post(url, headers=headers, json=metadata)
#
#print response.text
#
