#
#

TARGET = PointList PrimeNumber
CFLAGS = -Wall
LOCALS = \
 jsonproc.awk \
 iotedgeinit \
 iothubadd \
 iothubman \
 pointaddlist.awk \
 azure_iot_edge_v1

TEMPLATES = \
 template.json 

all: $(TARGET)

PointList: PointList.c
	$(CC) $(CFLAGS) -I ~/github/iot-edge-v1/v1/samples/simulated_device_cloud_upload/src/ -o PointList PointList.c

PrimeNumber: PrimeNumber.c

clean:
	/bin/rm -rf $(TARGET) *~

install: $(TARGET)
	cp -p $(TARGET) /usr/local/bin
	cp -p $(LOCALS) /usr/local/bin
	cp -p $(TEMPLATES) /var/tmp/dpride
