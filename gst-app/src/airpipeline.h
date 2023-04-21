#ifndef _MY_APP_AIR_PIPELINE_INCLUDED_
#define _MY_APP_AIR_PIPELINE_INCLUDED_

#include <gst/gst.h>


typedef struct _Params
{
    gchar host[128];
    guint port;
    gchar video[24];
    gboolean version;
} Params;

typedef struct _CustomData {
    GstElement *pipeline;
    GstElement *source;
    GstElement *convert;
    GstElement *resample;
    GstElement *sink;
} CustomData;

int demo(void);
int run_pipeline(int argc, char *argv[], void *args);

#endif