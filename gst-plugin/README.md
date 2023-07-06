## Custom pass through plugin

### on RK3568
visit http://192.168.0.99:5000/one

Same plugin codes for PC and RK3568.



```shell
$ GST_PLUGIN_PATH=/home/pi/sei/gst-plugin-github/build/gst-plugin  gst-launch-1.0   rtspsrc location=rtsp://admin:nanchao.org@192.168.31.107:554/Streaming/Channels/101   !  rtph264depay ! h264parse ! video/x-h264,stream-format=byte-stream,alignment=nal  ! seifilter host=192.168.0.99 port=5000 uri=one  !  queue  !  rtph264pay ! udpsink host=192.168.0.99 port=9988
```

### on PC

```shell
 GST_PLUGIN_PATH=/home/ruff/project/gstreamer/gst-plugin-github/build/gst-plugin  gst-launch-1.0    rtspsrc location=rtsp://admin:nanchao.org@192.168.31.107:554/Streaming/Channels/101   !  rtph264depay ! h264parse ! video/x-h264,stream-format=byte-stream,alignment=nal !  seifilter host=127.0.0.1 port=5000 uri=one  !  queue !   identity  ! fakesink sync=true
```

## Some H.264 NAL data

```
data: 00  00 00 01 41 9A
data size:6 99:99:99.
data: 00  00 00 01 09 10
data size:32
data: 00  00 00 01 67 64
data size:9
data: 00  00 00 01 68 EB
data size:25417
data: 00  00 00 01 65 88
data size:6 99:99:99.
data: 00  00 00 01 09 30
data size:15191
data: 00  00 00 01 41 9A
data size:6
data: 00  00 00 01 09 50
data size:9143
data: 00  00 00 01 41 9E

```

## SEIFilter
对SEI流进行修改,命名为seifilter,

属性:

```shell
host,
port,
uri,
# min interval of SEI information
interval,

```

读取到的服务器侧的信息格式:

```shell
SSP caught
Connection OK
read OK :: 172
HTTP/1.1 200 OK
Server: Werkzeug/2.3.6 Python/3.10.6
Date: Wed, 05 Jul 2023 08:14:17 GMT
Content-Type: text/html; charset=utf-8
Content-Length: 8
Connection: close

read OK :: 8
32:12:12
after read istream


# 解析Content-Length, 获取长度
# 解析Body, 获取最终的数据,
48 54 54 50 2f 31 2e 31 20 32 30 30 20 4f 4b 0d 0a 
53 65 72 76 65 72 3a 20 57 65 72 6b 7a 65 75 67 2f 32 2e 33 2e 36 20 50 79 74 68 6f 6e 2f 33 2e 31 30 2e 36 0d 0a 
44 61 74 65 3a 20 57 65 64 2c 20 30 35 20 4a 75 6c 20 32 30 32 33 20 30 39 3a 31 39 3a 30 31 20 47 4d 54 0d 0a 
43 6f 6e 74 65 6e 74 2d 54 79 70 65 3a 20 74 65 78 74 2f 68 74 6d 6c 3b 20 63 68 61 72 73 65 74 3d 75 74 66 2d 38 0d 0a 
43 6f 6e 74 65 6e 74 2d 4c 65 6e 67 74 68 3a 20 38 0d 0a 
43 6f 6e 6e 65 63 74 69 6f 6e 3a 20 63 6c 6f 73 65 0d 0a 
0d 0a 
# the GET body
33 32 3a 31 32 3a 31 32

```



## python3 

```shell
$ python3 -m http.server 8000

```

