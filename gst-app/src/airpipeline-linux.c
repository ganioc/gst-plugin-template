#include "airpipeline.h"

extern Params params;

static LinuxData data;

static void print_params(Params *param){
    g_print("------- params -------\n");
    g_print("host: %s\n", param->host);
    g_print("port: %d\n", param->port);
    g_print("video format: %s\n", param->video);
    g_print("----------------------\n");
}

int run_pipeline_linux(int argc, char *argv[], void *args)
{
    // GstElement *pipeline, *source, *sink;
    GstBus *bus;
    GstMessage *msg;
    GstStateChangeReturn ret;
    // GstElement *local_source;

    /* Initialize GStreamer */
    gst_init(&argc, &argv);

    // demo();
    g_print("run pipeline()\n");
    print_params(&params);


    /* Create the empty pipeline */
    g_print("create pipeline\n");
    data.pipeline = gst_pipeline_new("pipeline");

    /* Create the elements */
    g_print("create source\n");
    data.source = gst_element_factory_make("v4l2src", "source");

    g_print("create caps\n");
    data.capssource = gst_element_factory_make("capsfilter", "caps1");

    g_print("create convert\n");
    data.convert = gst_element_factory_make("videoconvert", "videoconvert");

    g_print("create x264enc\n");
    data.enc = gst_element_factory_make("x264enc", "x264enc");
    // data.xenccaps = gst_caps_new_simple("video/x-h264", 
    //     "stream-format", G_TYPE_STRING, "byte-stream",
    //     NULL);
    g_print("create caps after enc\n");
    data.capsenc = gst_element_factory_make("capsfilter", "capsenc");

    g_print("create h264parse\n");
    data.parse = gst_element_factory_make("h264parse","h264parse");
    // data.parsecaps = gst_caps_new_simple("video/x-h264", 
    //     "stream-format", G_TYPE_STRING, "byte-stream",
    //     "alignment", G_TYPE_STRING, "nal",
    //     NULL);
    // g_print("create queue\n");
    // data.queue = gst_element_factory_make("queue", "queue");
    g_print("create caps2\n");
    data.cpasparse = gst_element_factory_make("capsfilter", "caps2");
    g_print("create myfilter\n");
    data.myfilter = gst_element_factory_make("myfilter", "myfilter");
    g_print("create avdec\n");
    data.avdec = gst_element_factory_make("avdec_h264", "avdec");
    g_print("create convert2\n");
    data.convert2 = gst_element_factory_make("videoconvert", "convert2");
    g_print("create sink\n");
    data.sink = gst_element_factory_make("autovideosink", "sink");

    g_print("set caps after source\n");
    g_object_set(G_OBJECT(data.capssource),
        "caps",
        gst_caps_new_simple(
            "video/x-raw",
            "width", G_TYPE_INT,640,
            "height",G_TYPE_INT, 480,
            NULL
        ), NULL);
    
    g_print("set caps after enc\n");
    g_object_set(G_OBJECT(data.capsenc),
        "caps",
        gst_caps_new_simple(
            "video/x-h264",
            "stream-format",G_TYPE_STRING,"byte-stream",
            NULL
        ), NULL);
    g_print("set caps after parse\n");
    g_object_set(G_OBJECT(data.cpasparse),
        "caps",
        gst_caps_new_simple(
            "video/x-h264",
            "stream-format", G_TYPE_STRING,"byte-stream",
            "alignment", G_TYPE_STRING, "nal",
            NULL
        ), NULL);
    if(!data.myfilter){
        g_printerr("Quit element creation\n");
        return -1;
    }

    if (!data.pipeline || 
        !data.source || 
        !data.capssource ||
        !data.convert ||
        !data.enc ||
        !data.capsenc ||
        !data.parse ||
        !data.cpasparse ||
        !data.myfilter ||
        !data.avdec ||
        !data.convert2 ||
        !data.sink
        )
    {
        g_printerr("Not all elements could be created.\n");
        return -1;
    }

    /* Build the pipeline */
    g_print("build the pipeline\n");
    gst_bin_add_many(GST_BIN(data.pipeline), 
        data.source, 
        data.capssource,
        data.convert,
        data.enc,
        data.capsenc,
        data.parse,
        data.cpasparse,
        data.myfilter,
        data.avdec,
        data.convert2,
        data.sink, NULL);

    g_print("link many\n");
    if (!gst_element_link_many(data.source, 
        data.capssource,
        data.convert,
        data.enc,
        data.capsenc,
        data.parse,
        data.cpasparse,
        data.myfilter,
        data.avdec,
        data.convert2,
        data.sink, NULL))
    {
        g_printerr("Elements could not be linked.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }

    // "device-index", 1, 
    g_print("set source /dev/video0\n");
    g_object_set(G_OBJECT(data.source), "device", "/dev/video0", NULL);
    g_object_set(G_OBJECT(data.sink), "sync", FALSE, NULL);

    
    // create capsfilter h264-filter

    /* Start playing */
    ret = gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        g_printerr("Unable to set the pipeline to the playing state.\n");
        gst_object_unref(data.pipeline);
        return -1;
    }

    /* Wait until error or EOS */
    bus = gst_element_get_bus(data.pipeline);

    msg =
        gst_bus_timed_pop_filtered(bus, 
            GST_CLOCK_TIME_NONE,
            GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

    /* Parse message */
    if (msg != NULL)
    {
        GError *err;
        gchar *debug_info;

        switch (GST_MESSAGE_TYPE(msg))
        {
        case GST_MESSAGE_ERROR:
            gst_message_parse_error(msg, &err, &debug_info);
            g_printerr("Error received from element %s: %s\n",
                       GST_OBJECT_NAME(msg->src), err->message);
            g_printerr("Debugging info: %s\n",
                       debug_info ? debug_info : "none");
            g_clear_error(&err);
            g_free(debug_info);
            break;
        case GST_MESSAGE_EOS:
            g_print("End-Of-Stream reached.\n");
            break;
        default:
            /* We should not reach here because we only asked for ERRORs and EOS */
            g_printerr("Unexpected message received.\n");
            break;
        }
        gst_message_unref(msg);
    }

    /* Free resources */
    g_print("Release resources\n");
    gst_object_unref(bus);
    gst_element_set_state(data.pipeline, GST_STATE_NULL);
    gst_object_unref(data.pipeline);

    // gst_object_unref(data.source);
    // gst_object_unref(data.convert);
    // gst_object_unref(data.enc);
    // gst_object_unref(data.parse);
    // gst_object_unref(data.avdec);
    // gst_object_unref(data.convert2);
    // gst_object_unref(data.sink);

    g_print("exit\n");
    return 0;
}
