#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef LONG_OPTIONS
#include <getopt.h>
#endif

// ---------------------------------------------------------------
//  Definition of the default values
// ---------------------------------------------------------------


#define DEFAULT_SERVER_ADDRESS  "localhost"
#define DEFAULT_SERVER_PORT     "4000"
#define DEFAULT_AUTO_CONNECT    0
#define DEFAULT_CONNECT_DELAY   4


// --------------------------------------------------------------
//       Declaration of application private routines
// --------------------------------------------------------------


void parse_sub_options(char *ssopt,int *auto_connect, int *delay);

void run_server(char *server_address,
                char *server_port,
                int   auto_connect,
                int   connect_delay,
                int   argc,
                char  *argv []);

void display_help(char *program_name);

// ------------------------------------------------------------
//                 Start of main program
// ------------------------------------------------------------


int main (int arc, char *argv[])
{

    /*
    * Copying environment strings
    * It is not essential under Linux to make a copy
    * but it is a good habit to ensure the
    * portability of the program
    */
    char *opt_addr = NULL;
    char *opt_port = NULL;
    int opt_delay = 0;
    char *content;

    /*
     * Variables containing the actual values ​​of our parameters
     */

    static char *server_address = DEFAULT_SERVER_ADDRESS;
    static char *server_port = DEFAULT_SERVER_PORT;
    int auto_connect = DEFAULT_AUTO_CONNECT;
    int connect_delay = DEFAULT_CONNECT_DELAY;

    int option;

   /*
    * Reading environment variables, we hardcode here
    * the name of the variables, but we could also group them
    * (by #define) at the top of the file.
    */

    content = getenv("OPT_ADDR");
    if((content != NULL) && (strlen(content) != 0)) {
        opt_addr = malloc(strlen(content) + 1);
        if(opt_addr != NULL)
        {
            strcpy(opt_addr, content);
            server_address = opt_addr;
        } else {
            perror("malloc");
            exit(1);
        }
    }

    content = getenv("OPT_PORT");
    if((content != NULL) && (strlen(content) != 0)) {
        opt_port = malloc(strlen(content) + 1);
        if(opt_port != NULL)
        {
            strcpy(opt_port, content);
            server_port = opt_port;
        } else {
            perror("malloc");
            exit(1);
        }
    }
    
    content = getenv("OPT_AUTO");
    /* It is enough that the variable exists in the environment */
    /* its value does not matter to us */
    if (content != NULL)
      auto_connect = 1;

    content = getenv("OPT_DELAY");
    if (content != NULL)
        if (sscanf(content, "%d", &opt_delay) == 1)
            connect_delay = opt_delay;

    /* Reading command line options. */

    opterr = 1;

    while (1) {

#ifdef LONG_OPTIONS
        int index = 0;
        static struct option longopts[] = {
            { "address", 1, NULL, 'a' },
            { "port", 1, NULL, 'p' },
            { "option", 1, NULL, 'o' },
            { "help", 0, NULL, 'h' },
            { "NULL", 0, NULL, 0 },
        };

        option = getopt_long(arc,argv, "a:p:o:h", longopts, &index);
#else
        option = getopt(arc, argv, "a:p:o:h");
#endif
        if (option == -1)
            break;

        switch (option) {
        case 'a':
            /* Frees a possible copy of the string */
            /* of an equivalent environment. */
            if (opt_addr != NULL)
                free(opt_addr);
            opt_addr = NULL;
            server_address = optarg;
            break;
        case 'p':
            /* same */
            if (opt_port != NULL)
                free(opt_port);
            opt_port = NULL;
            server_port = optarg;
            break;
        case 'o':
            /* Analysing sub option */
            parse_sub_options(optarg,
                              & auto_connect,
                              & connect_delay);
            break;
        case 'h':
            /* display help */
            display_help(argv[0]);
            exit(0);
            break;
        default:
            break;
        }
    }

    run_server(server_address, server_port, auto_connect, connect_delay, arc - optind, &(argv[optind]));
    return 0;
}

void parse_sub_options(char *ssopt,int *auto_connect, int *delay)
{
    int subopt;
    char *string = ssopt;
    char *value = NULL;
    int  delay_value;

    char *tokens[] = {
        "auto", "nonauto", "delay", NULL
    };
    while ((subopt = getsubopt(&string, tokens, &value)) != -1)
    {
        switch (subopt) {
        case 0: /* auto */
            *auto_connect = 1;
            break;
        case 1: /* nonauto */
            *auto_connect = 0;
            break;
        case 2:
            if(value == NULL) {
                fprintf(stderr, "expecting delay value\n");
                break;
            }
            if(sscanf(value, "%d", &delay_value) != 1) {
                fprintf(stderr, "invalid delay value\n");
                break;
            }
            *delay = delay_value;
            break;
        default:
            break;
        }
    }
}

void run_server(char *server_address,
                char *server_port,
                int   auto_connect,
                int   connect_delay,
                int   argc,
                char  *argv [])
{
    int i;

    fprintf(stdout, "Serveur : %s - %s\n", server_address, server_port);
    fprintf(stdout, "Connexion auto : %s\n", auto_connect ? "oui" : "non");
    fprintf(stdout, "Delai : %d\n", connect_delay);
    fprintf(stdout, "Arguments supplementaires : ");
    for (i = 0; i< argc; i++)
        fprintf(stdout, "%s - ", argv[i]);
    fprintf(stdout, "\n");
}

void display_help (char *program_name)
{
    fprintf(stderr, "Usage: %s - [options] [files...]\n", program_name);
    fprintf(stderr, "Options :\n");
#ifdef LONG_OPTIONS
    fprintf(stderr, " --help\n");
#endif
    fprintf(stderr, " -h                 This help screen\n");
#ifdef LONG_OPTIONS
    fprintf(stderr, "  --address <server>\n");
#endif
    fprintf(stderr, " -a                 Server IP address\n");
#ifdef LONG_OPTIONS
    fprintf(stderr, "  --port <num>\n");
#endif
    fprintf(stderr, " -p <num>           Server TCP port number\n");
#ifdef LONG_OPTIONS
    fprintf(stderr, "  --option [sub_options]\n");
#endif
    fprintf(stderr, " -o [sub_options]       \n");
    fprintf(stderr, "Sub-options :\n");
    fprintf(stderr, "auto / nonauto Automatic connection\n");
    fprintf(stderr, "delay=<sec>    Delay before reconnecting\n");
}
