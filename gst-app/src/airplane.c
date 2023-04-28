/**
 * This is the main entry file of airplane
*/
#include "airpipeline.h"

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

Params params = {
    .host = "127.0.0.1",
    .port = 1234,
    .outhost = "127.0.0.1",
    .outport = 4567,
    .uuid = "086f3693-b7b3-4f2c-9653-21492feee5b8",
    .video = "h.264",
    .verbose = FALSE,
    .version = FALSE};
gchar version[] = "version: 0.1";

guint8 SEI_UUID[16];

static void print_params(Params *param)
{
    g_print("------- params -------\n");
    g_print("host: %s\n", param->host);
    g_print("port: %d\n", param->port);
    g_print("out host: %s\n", param->outhost);
    g_print("out port: %d\n", param->outport);
    g_print("uuid: %s\n", param->uuid);
    g_print("video format: %s\n", param->video);
    g_print("verbose: %d\n", param->verbose);
    g_print("----------------------\n");
}
gboolean valid_byte(gchar ch){ 
    guint8 num = ch;
     
    return (num >= '0' && num <= '9') || (num >= 'a' && num <= 'f');
}
int check_params(Params *param){
    guint index = 0;
    guint uuid_index = 0;
    guint64 hex;
    gchar hexstr[3];

    // check uuid format,
    for(index = 0; index < strlen(param->uuid); index++){
        if(valid_byte(param->uuid[index]) == TRUE){
            hexstr[0] = param->uuid[index];
	}else{
	    continue;
	}
	index++;
	if(valid_byte(param->uuid[index]) == TRUE){
	    hexstr[1] =  param->uuid[index];
	}else{
	    continue;
	}
	hexstr[2] = 0;
	if(FALSE == g_ascii_string_to_unsigned(hexstr, 16, 0, 0xff, &hex, NULL)){
	    continue;
	}
        //g_print("hexstr: %s, hex: %lx\n",hexstr,  hex);
	SEI_UUID[uuid_index++] = (guint8)hex;
    }
    g_print("SEI_UUID: ");
    for(guint i=0; i< 16; i++){
	g_print("%02x ", SEI_UUID[i]);
    }
    g_print("\n");

    if(uuid_index != 16){
	g_print("wrong uuid format!\n");
	return -1;
    }

    return 0;
}

int parse_params(int argc, char **argv)
{
    const GOptionEntry entries[] = {
	{"version", 'r',0,G_OPTION_ARG_NONE,&params.version,"version", NULL},
        {"verbose", 'v', 0, G_OPTION_ARG_NONE, &params.verbose, "executable verbose", NULL},
        {"host", 'h',0, G_OPTION_ARG_STRING, &params.host, "in host ip address", NULL},
	{"port", 'p',0,G_OPTION_ARG_INT, &params.port, "in host port", NULL},
	{"outhost", 'g',0,G_OPTION_ARG_STRING, &params.outhost, "out host ip address", NULL},
	{"outport", 'q',0,G_OPTION_ARG_INT, &params.outport, "out port", NULL},
	{"uuid", 'u',0,G_OPTION_ARG_STRING, &params.uuid, "uuid of SEI", NULL},
	{"video",'v',0,G_OPTION_ARG_STRING,&params.video,"format of video: h.264,h.265", NULL},
	{NULL}	
        };
    GOptionContext *ctx;
    GError *err = NULL;
    // gint i, num;

    ctx = g_option_context_new("airplane params");
    g_option_context_add_group(ctx, gst_init_get_option_group());
    g_option_context_add_main_entries(ctx, entries, NULL);

    if (!g_option_context_parse(ctx, &argc, &argv, &err))
    {
        g_print("Error command line: %s\n", GST_STR_NULL(err->message));
        return -1;
    }

    g_option_context_free(ctx);

    return 0;
}

int main(int argc, char *argv[])
{


    // commandline options parsing
    if (parse_params(argc, argv) != 0)
    {
        return -1;
    }
    if(params.version == TRUE){
	g_print("version: %s\n", version);
	return 0;
    }

    if (params.verbose == 1)
    {
        g_print("verbose: %d\n",  params.verbose);
    }

    g_print("Create the GStreamer 1.0 pipeline ...\n");
    print_params(&params);

    if(check_params(&params) != 0){
	g_print("Wrong params!\n");
	return -1;
    }

    g_print("In airplane app ...\n");
    
    // For it to run on macos
    #if defined(__APPLE__) && TARGET_OS_MAC && !TARGET_OS_IPHONE
        return gst_macos_main (run_pipeline_macos, argc, argv, NULL);
    #elif defined(__linux__)
        return run_pipeline_linux (argc, argv, NULL);
    #else
        g_print("Unknown OS type\n");
    #endif
}
