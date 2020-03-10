#include <stdio.h>   /* for fprintf etc */
#include <stdlib.h>  /* for free etc */
#include <string.h>  /* for strcmp */
#include "header.h"


static FILE * get_output(symbol * b) {
    char * s = b_to_s(b);
    FILE * output = fopen(s, "w");
    if (output == 0) {
        fprintf(stderr, "Can't open output %s\n", s);
        exit(1);
    }
    free(s);
    return output;
}

static void set_options(struct options * o) {
    o->output_file = 0;
    o->syntax_tree = false;
    o->externals_prefix = NULL;
    o->variables_prefix = 0;
    o->runtime_path = 0;
    o->name = "";
    o->includes = 0;
    o->includes_end = 0;    
    o->make_lang = LANG_RUST;
    o->widechars = false;
    o->utf8 = true;
}

extern void compile_snowball(char * sb_file, char * out_file) {
    NEW(options, o);
    set_options(o);
    {
        o->output_file = out_file;
        symbol * filename = add_s_to_b(0, sb_file);
        char * file;
        symbol * u = get_input(filename, &file);
        if (u == 0) {
            fprintf(stderr, "Can't open input %s\n", sb_file);
            exit(1);
        }
        {
            struct tokeniser * t = create_tokeniser(u, file);
            struct analyser * a = create_analyser(t);
            t->widechars = o->widechars;
            t->includes = o->includes;
            a->utf8 = t->utf8 = o->utf8;
            read_program(a);
            if (t->error_count > 0) exit(1);
            if (o->syntax_tree) print_program(a);
            close_tokeniser(t);
            if (!o->syntax_tree) {
                struct generator * g;

                const char * s = o->output_file;
                if (!s) {
                    fprintf(stderr, "Please include the -o option\n");
                    exit(1);
                }
                g = create_generator(a, o);

                if (o->make_lang == LANG_RUST) {
                    symbol * b = add_s_to_b(0, s);
                    o->output_src = get_output(b);
                    lose_b(b);
                    generate_program_rust(g);
                    fclose(o->output_src);
                }

                close_generator(g);
            }
            close_analyser(a);
        }
        lose_b(u);
        lose_b(filename);
    }
    FREE(o);
    if (space_count) fprintf(stderr, "%d blocks unfreed\n", space_count);
    return;
}

