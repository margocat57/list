#include "list_struct.h"
#include "list_dump.h"
#include <stdio.h>
#include <stdlib.h>

static char* generate_dot_png_file(const list* list, const char* name_file);

static void list_dump_html(const list* list, const char* output, const char* img, const char* debug_msg, const char *file, const char *func, int line);

void list_dump_func(const list* list, const char* input_file, const char* output_file, const char* debug_msg, const char *file, const char *func,  int line){
    char* img = generate_dot_png_file(list, input_file);
    if(img){
        list_dump_html(list, output_file, img, debug_msg, file, func, line);
    }
    free(img);
}

static void list_dump_html(const list* list, const char* output, const char* img, const char* debug_msg, const char *file, const char *func,  int line){
    static int launch_num = 0;
    FILE* html_output = NULL;
    if(launch_num == 0){
        html_output = fopen(output, "w");
        if(!html_output){
            fprintf(stderr, "Can't open html file\n");
            return;
        }
        launch_num++;
        fprintf(html_output, "<pre style=\"background-color: #1E2A36; color: #FFFFFF;\">\n");
        fprintf(html_output, "<p style=\"font-size: 50px; text-align: center;\"> LIST DUMP\n");
    }
    else{
        html_output = fopen(output, "a+");
        if(!html_output){
            fprintf(stderr, "Can't open html file\n");
            return;
        }
    }
    fprintf(html_output, "<p style=\"font-size: 20px; \">Dump was called at %s function %s line %d\n", file, func, line);
    fprintf(html_output, "<p style=\"font-size: 15px; \">%s\n" ,debug_msg);
    fprintf(html_output, "\n");
    fprintf(html_output, "<img src=\"%s\" alt=\"Array visualization\" width=\"95%%\">\n", img);
    fprintf(html_output, "\n");
    fclose(html_output);
}

static char* generate_dot_png_file(const list* list, const char* name_file){
    char dot_filename[100] = {0};
    char* png_filename = (char*)calloc(100, sizeof(char));
    if(!png_filename){
        fprintf(stderr, "Allocation error");
        return NULL;
    }
    char command[200]      = {0};

    if(snprintf(dot_filename, sizeof(dot_filename), "%s.dot", name_file) == -1){
        fprintf(stderr, "Can't parse name of dot file\n");
        return NULL;
    }
    if(snprintf(png_filename, 100, "%s.svg", name_file) == -1){
        fprintf(stderr, "Can't parse name of png file\n");
        return NULL;
    }

    FILE* dot_file = fopen(dot_filename, "w");
    if(!dot_file){
        fprintf(stderr, "Can't open dot file\n");
        return NULL;
    }

    fprintf(dot_file, "digraph G{\n");
    fprintf(dot_file, " rankdir=LR;\n");
    fprintf(dot_file, " splines=ortho;\n");
    fprintf(dot_file, " graph [bgcolor=\"#1E2A36\"];\n");
    fprintf(dot_file, " info[shape=\"record\", style=\"filled\", fillcolor=\"#4A5257\", color = \"#FFF4CC\", penwidth=2.0, label=\"head = %d | tail = %d | free = %d\"];\n", list->next[0], list->pred[0], list->free);
    for(int idx = 1; idx < (int)list->num_of_elem; idx++){
        if(list->data[idx]==POISON){
            continue;
        }
        fprintf(dot_file, " %d[shape=\"record\", style=\"filled\", fillcolor=\"#82898F\", color = \"#FFFFFF\", penwidth=2.0, label=\"phys idx = %d | elem = %.2lf | {prev = %d | next = %d}\"];\n", idx ,idx, list->data[idx], list->pred[idx], list->next[idx]);
    }
    for(int idx = 1; idx < (int)list->num_of_elem; idx++){
        if(list->data[idx]!=POISON){
            continue;
        }
        fprintf(dot_file, " %d[shape=\"record\", style=\"filled\", fillcolor=\"#5C646B\", color = \"#ffe766\", penwidth=2.0, label=\"phys idx = %d | elem = PZN | {prev = %d | next = %d} \"];\n", idx ,idx, list->pred[idx], list->next[idx]);
    }

    // connect normal elements
    int last_norm_idx = 0;
    for(int idx = 1; idx < (int)list->num_of_elem; idx++){
        if(list->data[idx]!=POISON){
            if(last_norm_idx == 0){
                last_norm_idx = idx;
                fprintf(dot_file," info -> %d [style=\"invis\", weight=500]\n", last_norm_idx);
                continue;
            }
            fprintf(dot_file," %d -> %d [style=\"invis\", weight=500]\n", last_norm_idx, idx);
            last_norm_idx = idx;
        }
    }

    // connect poison elements
    int poison_idx = 0;
    for(int idx = 1; idx < (int)list->num_of_elem; idx++){
        if(list->data[idx]==POISON){
            if(poison_idx == 0){
                poison_idx = idx;
                fprintf(dot_file," %d -> %d [style=\"invis\", weight=1000]\n", last_norm_idx, poison_idx);
                continue;
            }
            fprintf(dot_file," %d -> %d [style=\"invis\", weight=500]\n", poison_idx, idx);
            poison_idx = idx;
        }
    }

    // connect next elements
    for(int idx = 1; idx < (int)list->num_of_elem; idx++){
        if(list->data[idx]==POISON){
            continue;
        }
        if(list->next[idx]==0){
            continue;
        }
        fprintf(dot_file," %d -> %d [color = \"#D4A798\", penwidth = 1, arrowsize = 0.85]\n", idx, list->next[idx]);
    }


    fprintf(dot_file,"}\n");
    fclose(dot_file);

    if(snprintf(command, sizeof(command) ,"dot -Tsvg %s -o %s", dot_filename, png_filename) == -1){
        fprintf(stderr, "Can't parse command to do\n");
        return NULL;
    }
    int mistake = system(command);
    if(mistake != 0){
        fprintf(stderr, "System can't do parsing command, error code %d\n", mistake);
        return NULL;
    }
    return png_filename;
}