#include "main.h"
#include "fib_v0.h"
#include "fib_v1.h"
#include "cvt.h"

#define MAXIMUM_INDEX 200000
#define MAXIMUM_ITERATION 50

int main(int argc, char **argv)
{

    static struct option long_options[] = {
        {"help", no_argument, 0, 0},
        {0, 0, 0, 0}};

    // suppress auto generated error message from getopt()
    opterr = 0;

    int opt;
    // index for the implementation to be used
    int imp = 0;
    // flag for the benchmark
    int bmk = 0;
    // counter for iteration used in benchmark
    int ite = 10;
    // index for fibonacci-number
    int n = -1;

    char *endptr;

    while (1)
    {

        int option_index = 0;

        // add a '-' before optstring treats all positional arguments as optional arguments
        opt = getopt_long(argc, argv, "-V::B::n:h", long_options, &option_index);
        // reach end of the command line arguments
        if (opt == -1)
            break;
        switch (opt)
        {
        // the same as --h
        case 0:
            print_help_message(argv[0]);
            return EXIT_FAILURE;
        case 'V':
            if (optarg != NULL)
            {
                imp = strtol(optarg, &endptr, 10);
                if (endptr == optarg || *endptr != '\0')
                {
                    fprintf(stderr, "%s could not be converted to integer\n", optarg);
                    print_help_message(argv[0]);
                    return EXIT_FAILURE;
                }
                if (imp < 0 || imp > 1)
                {
                    fprintf(stderr, "the implementation with index %d does not exist\n", imp);
                    print_help_message(argv[0]);
                    return EXIT_FAILURE;
                }
            }
            else
            {
                fprintf(stdout, "the default implementation with index 0 is used\n");
            }
            break;
        case 'B':
            bmk = 1;
            if (optarg != NULL)
            {
                ite = strtol(optarg, &endptr, 10);
                if (endptr == optarg || *endptr != '\0')
                {
                    fprintf(stderr, "%s could not be converted to integer\n", optarg);
                    print_help_message(argv[0]);
                    return EXIT_FAILURE;
                }
                if (ite <= 0 || ite > MAXIMUM_ITERATION)
                {
                    fprintf(stderr, "the iteration of %d is out of range\n", imp);
                    print_help_message(argv[0]);
                    return EXIT_FAILURE;
                }
            }
            else
            {
                fprintf(stdout, "the default iteration of benchmark 10 is used\n");
            }
            break;
        case 'n':
            n = strtol(optarg, &endptr, 10);
            if (endptr == optarg || *endptr != '\0')
            {
                fprintf(stderr, "%s could not be converted to integer\n", optarg);
                print_help_message(argv[0]);
                return EXIT_FAILURE;
            }
            if (n < 0 || n > MAXIMUM_INDEX)
            {
                fprintf(stderr, "the %d-th fibonacci number is out of range\n", n);
                print_help_message(argv[0]);
                return EXIT_FAILURE;
            }
            break;
        case 'h':
            print_help_message(argv[0]);
            break;
        // case '?' indicates an error in parsing
        case '?':
            // missing number n
            if (optopt == 'n')
                fprintf(stderr, "please specify the fibonacci number to be calculated\n");
            // unknown positional argument
            else
                fprintf(stderr, "unknown optional argument -%c\n was found, please remove it and try again", optopt);
            print_help_message(argv[0]);
            return EXIT_FAILURE;
        // case 1 indicates a positional argument
        case 1:
            fprintf(stderr, "unknown positional argument was found, please remove it and try again\n");
            return EXIT_FAILURE;
        default:
            fprintf(stderr, "error occurs with getopt(), please try again\n");
            return EXIT_FAILURE;
        }
    }
    // user does not input n, thus the value of n does not change
    if (n == -1)
    {
        fprintf(stderr, "please specify the fibonacci number to be calculated\n");
        print_help_message(argv[0]);
        return EXIT_FAILURE;
    }
    size_t len = calc_length(n);
    // len is the required bits and therefor must be converted into integer which represents 8 bit at once
    uint8_t *buf = malloc(sizeof(uint8_t) * len);
    if (buf == NULL)
    {
        fprintf(stderr, "error occurs with malloc(), please try again\n");
        return EXIT_FAILURE;
    }

    double time = handler(imp, bmk, ite, n, len, buf);

    reverse(buf, len);

    // Print f(n) in binary, decimal and hexadecimal form
    unsigned char *bin = malloc(len * 8);

    _bytes_to_bitstring(len, buf, bin);
    fprintf(stdout, "fib(%d) bin: %s\n", n, bin);

    unsigned char *dec = malloc(strlen(bin));
    cvt(bin, dec, 2, 10);
    fprintf(stdout, "fib(%d) dec: %s\n", n, dec);

    unsigned char *hex = malloc(strlen(dec));
    cvt(dec, hex, 10, 16);
    fprintf(stdout, "fib(%d) hex: %s\n", n, hex);

    if (bmk)
    {
        fprintf(stdout, "the benchmark was executed %d time(s)\n", ite);
        fprintf(stdout, "the total calculation last for %f second(s)\n", time);
        fprintf(stdout, "the average duration of each calculation is %f second(s)\n", time / ite);
    }
    return EXIT_SUCCESS;
}

void print_help_message(char *program)
{
    const char *help_msg =
        "Usage:\n"
        "%s -n<index> [-V<implementation>] [-B<iteration>] [-h] [--help]\n"
        "Positional arguments:\n"
        "-n<index>\t<index> specifies a certain fibonacci number to be calculated from 0 to 20 000\n"
        "\n"
        "Optional arguments:\n"
        "-V<implementation>\t<implementation> specifies the version of implementation to be used\n"
        "\t\t\tthis can be chosen from 0 and 1, by default it is set to 0\n"
        "-B<iteration>\tif this option is set, the runtime of calculation would be measured and printed in the console\n"
        "\t\t\t<iteration> specifies how many times the benchmark would be repeated from 1 to 50, by default is 10\n"
        "-h or --help prints the help message and quit the program after that\n";
    fprintf(stdout, help_msg, program);
}

size_t calc_length(int n)
{
    // determine the length of fibonacci number with its general formula
    // Fn = [phi ^ n / sqrt(5)]
    //[Fn] = log(2, Fn) + 1

    // edge case
    if (n < 2)
    {
        return 1;
    }

    // phi is the gold ratio
    double phi = (1.0 + sqrt(5)) / 2.0;
    size_t tmp = (size_t)floor(n * log2(phi) - 0.5 * log2(5.0)) + 1;
    return tmp % 8 == 0 ? tmp / 8 : tmp / 8 + 1;
}

double handler(int imp, int bmk, int ite, int n, size_t len, uint8_t *buf)
{
    if (!bmk)
    {
        if (!imp)
            fib_v0(n, len, buf);
        else
            fib_v1(n, len, buf);
    }
    else
    {
        double sum = 0;
        if (!imp)
        {
            for (int i = 0; i < ite; ++i)
            {
                double t = fib_v0_benchmark(n, len, buf);
                sum += t;
                // fprintf(stdout, "the %d-th fibonacci number was calculated after %f second(s)\n", n, t);
            }
        }
        else
        {
            for (int i = 0; i < ite; ++i)
            {
                double t = fib_v1_benchmark(n, len, buf);
                sum += t;
                // fprintf(stdout, "the %d-th fibonacci number was calculated after %f second(s)\n", n, t);
            }
        }
        // fprintf(stdout, "the benchmark was executed %d time(s)\n", ite);
        // fprintf(stdout, "the total calculation last for %f seconds\n", sum);
        // fprintf(stdout, "the average duration of each calculation is %f sum\n", sum / ite);
        return sum;
    }
    return 0;
}