#include "list_struct.h"
#include <stdio.h>
#include <stdlib.h>

void list_dump_html(list* list, const char* file_name, const char* debug_msg){
    static int launch_num = 0;
    FILE* html_output = fopen(file_name, "a+");
    if(!html_output){
        fprintf(stderr, "Can't open html file\n");
        return;
    }
    if(launch_num == 0){
        launch_num++;
        fprintf(html_output, "<pre>\n");
    }
    fprintf(html_output, "%s\n" ,debug_msg);
    fprintf(html_output, "\n");
    // быть готовым вывести массивы но как будто они должны быть на картинке
    fprintf(html_output, "<img src=\"\" alt=\"Array visualization\" width=\"10%\">\n");
    fclose(html_output);
}

void gerenate_dot_png_file(list* list, const char* name_file){
    char dot_filename[100] = {0};
    char png_filename[100] = {0};
    char command[200]      = {0};

    if(snprintf(dot_filename, sizeof(dot_filename), "%s.dot", name_file) == -1){
        fprintf(stderr, "Can't parse name of dot file\n");
        return;
    }
    if(snprintf(png_filename, sizeof(png_filename), "%s.png", name_file) == -1){
        fprintf(stderr, "Can't parse name of png file\n");
        return;
    }

    FILE* dot_file = fopen(dot_filename, "w");
    if(!dot_file){
        fprintf(stderr, "Can't open dot file\n");
        return;
    }

    fprintf(dot_file, "digraph G{\n");
    fprintf(dot_file, " rankdir=LR;\n");
    fprintf(dot_file, " splines=ortho;\n");
    fprintf(dot_file, " graph [bgcolor=\"#1E2A36\"];\n");
    fprintf(dot_file, " info[shape=\"record\", style=\"filled\", fillcolor=\"#82898F\", color = \"#ffe766\", label=\"head = %d | tail = %d | free = %d\"];\n", list->head, list->tail, list->free);
    for(int idx = 1; idx < list->num_of_elem; idx++){
        if(list->data[idx]!=POISON){
            fprintf(dot_file, " %d[shape=\"record\", style=\"filled\", fillcolor=\"#82898F\", color = \"#FFFFFF\", label=\"phys idx = %d | elem = %.2lf | {prev = %d | next = %d} \"];\n", idx ,idx, list->data[idx], list->pred[idx], list->next[idx]);
        }
    }
    for(int idx = 1; idx < list->num_of_elem; idx++){
        if(list->data[idx]==POISON){
            fprintf(dot_file, " %d[shape=\"record\", style=\"filled\", fillcolor=\"#82898F\", color = \"#ffe766\", label=\"phys idx = %d | elem = PZN | {prev = %d | next = %d} \"];\n", idx ,idx, list->pred[idx], list->next[idx]);
        }
    }

    // connect normal elements
    int last_norm_idx = 0;
    for(int idx = 1; idx < list->num_of_elem; idx++){
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
    for(int idx = 1; idx < list->num_of_elem; idx++){
        if(list->data[idx]==POISON){
            if(poison_idx == 0){
                poison_idx = idx;
                fprintf(dot_file," info -> %d [style=\"invis\", weight=500]\n", poison_idx);
                continue;
            }
            fprintf(dot_file," %d -> %d [style=\"invis\", weight=500]\n", poison_idx, idx);
            poison_idx = idx;
        }
    }

    // connect next elements
    for(int idx = 1; idx < list->num_of_elem; idx++){
        if(list->data[idx]==POISON){
            continue;
        }
        if(list->next[idx]==0){
            continue;
        }
        fprintf(dot_file," %d -> %d [arrowhead = \"vee\", color = \"#79A0C1\", penwidth = 1.25, arrowsize = 1.5]\n", idx, list->next[idx]);
    }
    fprintf(dot_file,"}\n");
    fclose(dot_file);

    if(snprintf(command, sizeof(command) ,"dot -Tpng %s -o %s", dot_filename, png_filename) == -1){
        fprintf(stderr, "Can't parse command to do\n");
        return;
    }
    int mistake = system(command);
    if(mistake != 0){
        fprintf(stderr, "System can't do parsing command, error code %d\n", mistake);
    }
}