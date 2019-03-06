
import requests
import subprocess

bytesize = 1048576
count = 100
filename = "file1"
filesize = bytesize * count

device = "mlx5_0"
server = "118.138.254.158"
handshake_port = 19000
# as a demo hack we require ports on both ends 
# to be the same value between 19000-20000

tardis_server = "118.138.254.158"
url = "http://" + tardis_server  + "/api/v1/dataset_file/"
username = "testroce"
apikey = "testroceapikey"

subprocess.Popen( ["dd", "if=/dev/urandom", "of="+filename, 
    "bs="+str(bytesize), "count="+str(count)]);

subprocess.Popen(["./ibv_basic_send", "-d", device, "-i", "1", "-s", 
    server, "-h", str(handshake_port)])
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
