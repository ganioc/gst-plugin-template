## 处理_chain()函数，
对输入的buf进行处理后，释放buf, 将outbuf传递给srcpad.

```c

static GstFlowReturn
gst_my_filter_chain (GstPad    *pad,
             GstObject *parent,
             GstBuffer *buf)
{
  GstMyFilter *filter = GST_MY_FILTER (parent);
  GstBuffer *outbuf;

  outbuf = gst_my_filter_process_data (filter, buf);
  gst_buffer_unref (buf);
  if (!outbuf) {
    /* something went wrong - signal an error */
    GST_ELEMENT_ERROR (GST_ELEMENT (filter), STREAM, FAILED, (NULL), (NULL));
    return GST_FLOW_ERROR;
  }

  return gst_pad_push (filter->srcpad, outbuf);
}

```

有些情况下，你需要，element可以对输入数据的速率进行控制,我们可以写一个loop-based element, some elements 可以成为get-based element,

## 处理query function

```c
/* configure event function on the pad before adding
   * the pad to the element */
gst_pad_set_query_function (filter->srcpad,
      gst_my_filter_src_query);

static gboolean
gst_my_filter_src_query (GstPad    *pad,
                 GstObject *parent,
                 GstQuery  *query)
{
  gboolean ret;
  GstMyFilter *filter = GST_MY_FILTER (parent);

  switch (GST_QUERY_TYPE (query)) {
    case GST_QUERY_POSITION:
      /* we should report the current position */
      [...]
      break;
    case GST_QUERY_DURATION:
      /* we should report the duration here */
      [...]
      break;
    case GST_QUERY_CAPS:
      /* we should report the supported caps here */
      [...]
      break;
    default:
      /* just call the default handler */
      ret = gst_pad_query_default (pad, parent, query);
      break;
  }
  return ret;
}

static gboolean my_plugin_sink_event(GstPad *pad, GstObject *parent, GstEvent *event)
{
    MyPlugin *plugin = MY_PLUGIN(parent);
    
    switch (GST_EVENT_TYPE(event))
    {
        case GST_EVENT_CAPS:
            // Handle caps event
            break;
        case GST_EVENT_EOS:
            // Handle EOS event
            break;
        case GST_EVENT_QUERY:
        {
            GstQuery *query = gst_event_get_query(event);
            GstCaps *caps;
            
            if (gst_query_parse_caps(query, &caps))
            {
                // Handle caps query
                GstStructure *structure = gst_caps_get_structure(caps, 0);
                const gchar *mime_type = gst_structure_get_name(structure);
                GST_INFO_OBJECT(plugin, "Received caps query with mime type %s", mime_type);
                
                // Set the caps on the sink pad
                gst_pad_set_caps(pad, caps);
            }
            else if (gst_query_has_name(query, "my-query"))
            {
                // Handle custom query
                gint value = 123;
                gst_query_set_result(query, GST_QUERY_OK, &value);
            }
            else
            {
                // Handle unknown query
                gst_query_set_result(query, GST_QUERY_NOT_IMPLEMENTED, NULL);
            }
            
            break;
        }
        default:
            break;
    }
    
    return gst_pad_event_default(pad, parent, event);
}
```

## 如何设置element的参数?
g_object_set (data.playbin, "uri", "https://gstreamer.freedesktop.org/data/media/sintel_trailer-480p.webm", NULL);

## 
