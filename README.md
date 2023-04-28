

## app
gst-app/src/airplane.c

## plugin
gst-plugin/src/gstmyfilter.c

## compile
```
meson build
ninja -C build
```
## execute

```shell
$ export GST_PLUGIN_PATH=/root/gst-prj/build/gst-plugin

$ ./build/gst-app/airplane --help



$ ./build/gst-app/airplane  --port=9988 --outhost=192.168.31.133 --outport=9989 --uuid=086f3693-b7b3-4f2c-9653-21492feee5b8

```

## H.264 NAL Unit format

From camera,

From rtp server,


