#ifndef _MY_APP_AIR_PIPELINE_INCLUDED_
#define _MY_APP_AIR_PIPELINE_INCLUDED_

#include <gst/gst.h>

#define DATA_ARR_LEN    128

// send with camera, read direct from camera,
// #define ENABLE_CAMERA
//
#define ENABLE_RK3568      1

// send with RTP, it's for Linux, read from RTP,
// #define ENABLE_RTP      1


typedef struct _Params
{
    gchar *host;
    guint port;
    gchar *outhost;
    gchar *uuid;
    guint outport;
    gchar *video;
    gboolean version;
    gboolean verbose;
} Params;

typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *xenc;
    GstCaps *xenccaps;
    GstElement *h264parse;
    GstCaps *parsecaps;
    GstElement *queue;
    GstElement *avdec;
    GstElement *sink;
} CustomData;

#ifdef ENABLE_CAMERA
typedef struct _LinuxData{
    GstElement *pipeline;
    GstElement *source;
    GstElement *capssource;
    GstElement *convert;
    GstElement *enc;
    GstElement *capsenc;
    GstElement *parse;
    GstElement *capsparse;
    GstElement *myfilter;
    GstElement *avdec;
    GstElement *convert2;
    GstElement *sink;
} LinuxData;
#elif defined ENABLE_RTP
// ENABLE_RTP
typedef struct _LinuxData{
    GstElement *pipeline;
    GstElement *source;
    GstElement *capssource;
    GstElement *depay;
    GstElement *parse;
    GstElement *capsparse;
    GstElement *queue;
    GstElement *myfilter;
    GstElement *avdec;
    GstElement *convert2;
    GstElement *text;
    GstElement *sink;

} LinuxData;

#else
typedef struct _LinuxData{
    GstElement *pipeline;
    GstElement *source;
    GstElement *capssource;
    GstElement *depay;
    GstElement *parse;
    GstElement *capsparse;
    GstElement *queue;
    GstElement *myfilter;
    GstElement *rtph264pay;
    GstElement *sink;

} LinuxData;



#endif

int run_pipeline_macos(int argc, char *argv[], void *args);
int run_pipeline_linux(int argc, char *argv[], void *args);

#endif
