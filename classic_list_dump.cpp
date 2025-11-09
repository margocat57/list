#include "list_struct.h"
#include "classic_list_dump.h"
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>

#define CHECK_AND_RET_DUMP(bad_condition, msg)\
    if(bad_condition){ \
        fprintf(stderr, msg); \
        return; \
    } \

static filenames_for_dump_cl filename_ctor_cl();

static int check_and_dump_system_work_cl(int sys_res);

static void generate_dot_file_cl(list_classic_head* head, const char* dot_filename);

static void generate_svg_file_cl(const filenames_for_dump_cl* dump);

static void list_dump_html_cl(const char* img, const char* debug_msg, const char *file, const char *func, int line, va_list args)  __attribute__ ((format (printf, 2, 7)));

void list_dump_func_cl(list_classic_head* head, const char* debug_msg, const char *file, const char *func,  int line, ...){
    filenames_for_dump_cl dump = filename_ctor_cl();
    generate_dot_file_cl(head, dump.dot_filename_cl);
    generate_svg_file_cl(&dump);
    if(dump.svg_filename_cl){
        va_list args = {};
        va_start(args, line);
        list_dump_html_cl(dump.svg_filename_cl, debug_msg, file, func, line, args);
        free(dump.svg_filename_cl);
        va_end(args);
    }
    if(dump.dot_filename_cl){
        free(dump.dot_filename_cl);
    }
}

static void list_dump_html_cl(const char* img, const char* debug_msg, const char *file, const char *func, int line, va_list args){
    static int launch_num = 0;
    FILE* html_output = NULL;
    if(launch_num == 0){
        html_output = fopen(LOG_FILE, "w");
        CHECK_AND_RET_DUMP(!html_output, "Can't open html file\n");
        launch_num++;
        fprintf(html_output, "<pre style=\"background-color: #FFFAFA; color: #000000;\">");
        fprintf(html_output, "<p style=\"font-size: 50px; text-align: center;\"> LIST DUMP\n");
        fprintf(html_output, "<p style=\"font-size: 30px; \"> Legend:\n");
        fprintf(html_output, "<p style=\"font-size: 25px; \"> <span style=\"color: #CD5C5C;\">Red</span> - zero element\n");
        fprintf(html_output, "<p style=\"font-size: 25px; \"> <span style=\"color: #98FB98;\">Green</span> - Ocuppied elememt\n");
        fprintf(html_output, "<p style=\"font-size: 25px; \"> <span style=\"color: #bb0d12;\">Red octagon</span> - index of elemet out of array\n");
        fprintf(html_output, "<p style=\"font-size: 25px; \"> <span style=\"color: #0000FF; font-weight: bold;\">Blue bold</span> or <span style=\"color: #FF4F00; font-weight: bold;\">orange bold</span> - for incorrect connections\n");
    }
    else{
        html_output = fopen(LOG_FILE, "a+");
        CHECK_AND_RET_DUMP(!html_output, "Can't open html file\n");
    }
    fprintf(html_output, "<p style=\"font-size: 20px; \">Dump was called at %s function %s line %d\n", file, func, line);
    fprintf(html_output, "<p style=\"font-size: 17.5px; color: #bb0d12;\">");
    vfprintf(html_output, debug_msg, args); 
    fprintf(html_output, "\n");

    fprintf(html_output, "<img src=\"%s\" alt=\"Array visualization\" width=\"95%%\">\n", img);
    fprintf(html_output, "\n");
    fclose(html_output);
}

static filenames_for_dump_cl filename_ctor_cl(){
    filenames_for_dump_cl dump = {};
    static int num = 0;
    num++;

    char* dot_filename = (char*)calloc(100, sizeof(char));
    if(!dot_filename){
        fprintf(stderr, "Allocation error for string with .dot filename");
        return dump;
    }

    char* svg_filename = (char*)calloc(100, sizeof(char));
    if(!svg_filename){
        fprintf(stderr, "Allocation error for string with .svg filename");
        return dump;
    }

    dump.svg_filename_cl = svg_filename;
    dump.dot_filename_cl = dot_filename;

    time_t now = time(NULL); 
    struct tm *t = localtime(&now); 

    if (snprintf(dump.dot_filename_cl, 100, 
                "images/dump%d_%04d%02d%02d_%02d%02d%02d.dot", num,
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec) == -1) {
        fprintf(stderr, "Can't generate dot filename\n");
        free(dump.svg_filename_cl);
        return {};
    }
    
    if (snprintf(dump.svg_filename_cl, 100, 
                "images/dump%d_%04d%02d%02d_%02d%02d%02d.svg", num,
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec) == -1) {
        fprintf(stderr, "Can't generate svg filename\n");
        free(dump.dot_filename_cl);
        return {};
    }
    return dump;
}

static void generate_dot_file_cl(list_classic_head* head, const char* dot_filename){
    if(!dot_filename){
        fprintf(stderr, "NULL dot_filename pointer - can't work\n");
        return;
    }

    FILE* dot_file = fopen(dot_filename, "w");
    if(!dot_file){
        fprintf(stderr, "Can't open dot file\n");
        return;
    }

    fprintf(dot_file, "digraph G{\n");
    fprintf(dot_file, " rankdir=LR;\n");
    // fprintf(dot_file, " splines=ortho;\n");
    fprintf(dot_file, " graph [bgcolor=\"#FFFAFA\", nodesep = 0.3, ranksep=0.8];\n");
    list_classic* elem = head->root;

    for(ssize_t idx = 0; idx < head->capacity; idx++){
        if(idx == 0){
            fprintf(dot_file, " node_%p[shape=\"Mrecord\", style=\"filled\", fillcolor=\"#CD5C5C\", color = \"#000000\", penwidth=1.0, label=\"elem = %.2lf | {prev = %p | next = %p}\"];\n", elem, elem->data_cl, elem->prev_cl, elem->next_cl);
            elem = elem->next_cl;
            continue;
        }
        if(!elem->prev_cl || !elem->next_cl){
            if(!elem->prev_cl && !elem->next_cl){
                elem = elem->next_cl;
                continue;
            }
            if(!elem->prev_cl){
                fprintf(dot_file, " node_%p[shape=\"octagon\", style=\"filled\", fillcolor=\"#bb0d12\", color = \"#000000\", penwidth=2.0, label=\"%p\"];\n", elem->next_cl);
            }
            if(!elem->next_cl){
                fprintf(dot_file, " node_%p[shape=\"octagon\", style=\"filled\", fillcolor=\"#bb0d12\", color = \"#000000\", penwidth=2.0, label=\"%zd\"];\n", elem->prev_cl);
                break;
            }
            elem = elem->next_cl;
            continue;
        }
        fprintf(dot_file, " node_%p[shape=\"Mrecord\", style=\"filled\", fillcolor=\"#98FB98\", color = \"#000000\", penwidth=1.0, label=\"elem = %.2lf | {prev = %p | next = %p}\"];\n", elem, elem->data_cl, elem->prev_cl, elem->next_cl);
        elem = elem->next_cl;
    }

    
    fprintf(dot_file, " head_label [shape=\"Mrecord\", style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"HEAD\"]\n");
    fprintf(dot_file, " tail_label [shape=\"Mrecord\", style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"TAIL\"]\n");

    // fprintf(dot_file, "{rank=same; %zd; head_label;}\n", list->next[0]);
    // fprintf(dot_file, "{rank=same; %zd; tail_label;}\n", list->prev[0]);
    // fprintf(dot_file, "{rank=same; %zd; free_label;}\n", list->free);

    fprintf(dot_file, "head_label -> tail_label[style=\"invis\", weight=500]");

    if(head->root->prev_cl && head->root->next_cl){
        fprintf(dot_file, "tail_label -> node_%p [color=\"#20B2AA\", penwidth=1, arrowsize=0.85, style=\"solid\"]\n", head->root->prev_cl);
        fprintf(dot_file, "head_label -> node_%p [color=\"#20B2AA\", penwidth=1, arrowsize=0.85, style=\"solid\"]\n", head->root->next_cl);
    }

    // connect elements (invis)
    ssize_t last_norm_idx = 0;
    for(ssize_t idx = 0; idx < head->capacity; idx++){
        if(!elem->next_cl){
            continue;
        }
        fprintf(dot_file," node_%p -> node_%p [style=\"invis\", weight=1000]\n", elem, elem->next_cl);
        elem = elem->next_cl;
    }

    for(ssize_t idx = 0; idx < head->capacity; idx++){
        if(!elem->next_cl || !elem->prev_cl){
            if(!elem->next_cl && !elem->prev_cl){
                continue;
            }
            if(!elem->next_cl){
                fprintf(dot_file, " node_%p -> node_%p [color = \"#FF4F00\", penwidth = 2, arrowsize = 1, constraint=false, weight=0]\n",  elem->prev_cl, elem);
                break;
            }
            if(!elem->prev_cl){
                fprintf(dot_file, " node_%p -> node_%p [color = \"#0000FF\", penwidth = 2, arrowsize = 1, constraint=false, weight=0]\n",  elem, elem->next_cl);
                elem = elem->next_cl;
                continue;
            }
        }

        if(elem->prev_cl->next_cl != elem || elem->next_cl->prev_cl != elem){
            if(elem->prev_cl->next_cl != elem){
                fprintf(dot_file, " node_%p -> node_%p [color = \"#FF4F00\", penwidth = 2, arrowsize = 1, constraint=false, weight=0]\n", elem->prev_cl, elem);
            }
            if(elem->next_cl->prev_cl != elem){
                fprintf(dot_file, " node_%p -> node_%p [color = \"#0000FF\", penwidth = 2, arrowsize = 1, constraint=false, weight=0]\n", elem, elem->next_cl);
            }
            elem = elem->next_cl;
            continue;
        }

        fprintf(dot_file," node_%p -> node_%p [color = \"#000000\", penwidth = 1, arrowsize = 0.85, dir=both, weight=0]\n", elem, elem->next_cl);
        elem = elem->next_cl;
    }

    fprintf(dot_file,"}\n");
    fclose(dot_file);
}

static void generate_svg_file_cl(const filenames_for_dump_cl* dump){
    char command[200] = {0};

    if(snprintf(command, sizeof(command) ,"dot -Tsvg %s -o %s", dump->dot_filename_cl, dump->svg_filename_cl) == -1){
        fprintf(stderr, "Can't parse command to do\n");
        return;
    }
    int sys_res = system(command);
    check_and_dump_system_work_cl(sys_res);
}

static int check_and_dump_system_work_cl(int sys_res){
    if(sys_res == 0){
        return 0;
    }
    if(sys_res == -1){
        fprintf(stderr, "Cannot start to do command\n");
        return -1;
    }
    if(WIFEXITED(sys_res)){
        int exit_stat = WEXITSTATUS(sys_res);
        fprintf(stderr, "Creating svg file finished with mistake %d\n", exit_stat);
        return exit_stat;
    }
    return 0;
}
