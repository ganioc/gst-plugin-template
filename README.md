

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
$ ./build/gst-app/airplane
```

## H.264 NAL Unit format

From camera,

From rtp server,


