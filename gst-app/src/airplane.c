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
    .uuid = "aaaa",
    .video = "h.264",
    .verbose = FALSE};
gchar version[] = "version: 0.1";

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


int parse_params(int argc, char **argv)
{
    const GOptionEntry entries[] = {
        {"verbose", 'v', 0, G_OPTION_ARG_INT, &params.verbose, "executable verbose", NULL},
        {"host", 'h',0, G_OPTION_ARG_STRING, &params.host, "in host ip address", NULL},
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

    g_print("In airplane app ...\n");

    // commandline options parsing
    if (parse_params(argc, argv) != 0)
    {
        return -1;
    }

    if (params.verbose == 1)
    {
        g_print("verbose: %d\n",  params.verbose);
    }

    g_print("Create the GStreamer 1.0 pipeline ...\n");
    print_params(&params);

    return 0;


    // For it to run on macos
    #if defined(__APPLE__) && TARGET_OS_MAC && !TARGET_OS_IPHONE
        return gst_macos_main (run_pipeline_macos, argc, argv, NULL);
    #elif defined(__linux__)
        return run_pipeline_linux (argc, argv, NULL);
    #else
        g_print("Unknown OS type\n");
    #endif
}
