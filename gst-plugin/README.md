## Custom pass through plugin

```shell
GST_PLUGIN_PATH=/Users/yango3/Documents/yangjun/project/raspberry/gstreamer/gst-template/build/gst-plugin/ gst-launch-1.0 --gst-debug-level=4 videotestsrc ! my_filter ! autovideosink
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
```



## python3 

```shell
$ python3 -m http.server 8000

```

