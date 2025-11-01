#include "list_struct.h"
#include "list_dump.h"
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define CHECK_AND_RET_DUMP(bad_condition, msg)\
    if(bad_condition){ \
        fprintf(stderr, msg); \
        return; \
    } \

static filenames_for_dump filename_ctor();

static int check_and_dump_system_work(int sys_res);

static void generate_dot_file(const list* list, const char* dot_filename);

static void generate_svg_file(const filenames_for_dump* dump);

static void draw_table(const list* list, FILE* html_output);

static void list_dump_html(const list* list, const char* img, const char* debug_msg, const char *file, const char *func, int line);

void list_dump_func(const list* list, const char* debug_msg, const char *file, const char *func,  int line){
    filenames_for_dump dump = filename_ctor();
    generate_dot_file(list, dump.dot_filename);
    generate_svg_file(&dump);
    if(dump.svg_filename){
        list_dump_html(list, dump.svg_filename, debug_msg, file, func, line);
        free(dump.svg_filename);
    }
    if(dump.dot_filename){
        free(dump.dot_filename);
    }
}

static void list_dump_html(const list* list, const char* img, const char* debug_msg, const char *file, const char *func,  int line){
    static int launch_num = 0;
    FILE* html_output = NULL;
    if(launch_num == 0){
        html_output = fopen(LOG_FILE, "w");
        CHECK_AND_RET_DUMP(!html_output, "Can't open html file\n");
        launch_num++;
        fprintf(html_output, "<pre style=\"background-color: #FFFAFA; color: #000000;\">");
        fprintf(html_output, "<p style=\"font-size: 50px; text-align: center;\"> LIST DUMP\n");
        fprintf(html_output, "<p style=\"font-size: 30px; \"> Legend\n");
        fprintf(html_output, "<p style=\"font-size: 25px; \"> <span style=\"color: #82898f;\">Grey</span> - free places(now filled poison elements)\n");
        fprintf(html_output, "<p style=\"font-size: 25px; \"> <span style=\"color: #CD5C5C;\">Red</span> - zero element\n");
        fprintf(html_output, "<p style=\"font-size: 25px; \"> <span style=\"color: #98FB98;\">Green</span> - Ocuppied elememt\n");
        fprintf(html_output, "<p style=\"font-size: 25px; \"> <span style=\"color: #bb0d12;\">Red octagon</span> - index of elemet out of array\n");
    }
    else{
        html_output = fopen(LOG_FILE, "a+");
        CHECK_AND_RET_DUMP(!html_output, "Can't open html file\n");
    }
    fprintf(html_output, "<p style=\"font-size: 20px; \">Dump was called at %s function %s line %d\n", file, func, line);
    fprintf(html_output, "<p style=\"font-size: 15px; \">%s\n" ,debug_msg);
    fprintf(html_output, "\n");

    draw_table(list, html_output);

    fprintf(html_output, "<img src=\"%s\" alt=\"Array visualization\" width=\"95%%\">\n", img);
    fprintf(html_output, "\n");
    fclose(html_output);
}

static void draw_table(const list* list, FILE* html_output){
    fprintf(html_output, "<table style=\"text-align: center; font-size: 15px; border: 2px solid #000000; border-collapse: collapse; width: 50%%; color: #FFFFFF;\">\n");

    fprintf(html_output, "<tr>\n");
    fprintf(html_output, "<td style = \"color: #000000;\">Index</td>");
    for (ssize_t i = 0; i < list->num_of_elem; i++) {
        fprintf(html_output, "<td style = \"border: 1px solid #000000; color: #000000;\";>%zd</td>\n", i);
    }
    fprintf(html_output, "</tr>\n");

    fprintf(html_output, "<tr>\n");
    fprintf(html_output, "<td style = \"color: #000000;\">Data</td>");
    for (ssize_t i = 0; i < list->num_of_elem; i++) {
        fprintf(html_output, "<td style = \"border: 1px solid #000000; color: #000000;\";>%.2lf</td>\n", list->data[i]);
    }
    fprintf(html_output, "</tr>\n");

    fprintf(html_output, "<tr>\n");
    fprintf(html_output, "<td style = \"color: #000000;\">Next</td>");
    for (ssize_t i = 0; i < list->num_of_elem; i++) {
        fprintf(html_output, "<td style = \"border: 1px solid #000000; color: #000000;\";>%zd</td>\n", list->next[i]);
    }
    fprintf(html_output, "</tr>\n");

    fprintf(html_output, "<tr>\n");
    fprintf(html_output, "<td style = \"color: #000000;\">Prev</td>");
    for (ssize_t i = 0; i < list->num_of_elem; i++) {
        fprintf(html_output, "<td style = \"border: 1px solid #000000; color: #000000;\";>%zd</td>\n", list->prev[i]);
    }
    fprintf(html_output, "</tr>\n");

    fprintf(html_output, "</table>\n"); 
}

static filenames_for_dump filename_ctor(){
    filenames_for_dump dump = {};
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

    dump.svg_filename = svg_filename;
    dump.dot_filename = dot_filename;

    time_t now = time(NULL); 
    struct tm *t = localtime(&now); 

    if (snprintf(dump.dot_filename, 100, 
                "images/dump%d_%04d%02d%02d_%02d%02d%02d.dot", num,
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec) == -1) {
        fprintf(stderr, "Can't generate dot filename\n");
        free(dump.svg_filename);
        return {};
    }
    
    if (snprintf(dump.svg_filename, 100, 
                "images/dump%d_%04d%02d%02d_%02d%02d%02d.svg", num,
                t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                t->tm_hour, t->tm_min, t->tm_sec) == -1) {
        fprintf(stderr, "Can't generate svg filename\n");
        free(dump.dot_filename);
        return {};
    }
    return dump;
}

// короче занятые зеленым, стрелки черным, нулевой элемент красным, пойзоны серым
// фон белый, буквы и текст черным,
// и стрелки не параллельно блокам
// испортить списки
// графические дампы на списки
// жирным если неккоректно
static void generate_dot_file(const list* list, const char* dot_filename){
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
    fprintf(dot_file, " splines=ortho;\n");
    fprintf(dot_file, " graph [bgcolor=\"#FFFAFA\", nodesep = 0.8, ranksep=0.8];\n");
    // fprintf(dot_file, " graph [bgcolor=\"#FFFAFA\"];\n");

    /*
    fprintf(dot_file, " head_label [shape=box, style=\"filled\", fillcolor=\"#879eac\", color=\"#FFF4CC\", fontcolor=\"#FFF4CC\", fontsize=12, label=\"HEAD\"]\n");
    fprintf(dot_file, " tail_label [shape=box, style=\"filled\", fillcolor=\"#879eac\", color=\"#FFF4CC\", fontcolor=\"#FFF4CC\", fontsize=12, label=\"TAIL\"]\n");
    fprintf(dot_file, " free_label [shape=box, style=\"filled\", fillcolor=\"#879eac\", color=\"#FFF4CC\", fontcolor=\"#FFF4CC\", fontsize=12, label=\"FREE\"]\n");
    */
    fprintf(dot_file,"subgraph system {\n");
    // fprintf(dot_file, " rankdir=LR;\n");
    for(ssize_t idx = 0; idx < list->num_of_elem; idx++){
        if(idx == 0){
            fprintf(dot_file, " %d[shape=\"Mrecord\", style=\"filled\", fillcolor=\"#CD5C5C\", color = \"#000000\", penwidth=1.0, label=\"phys idx = %d | elem = %.2lf | {prev = %zd | next = %zd}\"];\n", 0, 0, list->data[0], list->prev[0], list->next[0]);
            continue;
        }
        if(list->data[idx]==POISON){
            fprintf(dot_file, " %zd[shape=\"Mrecord\", style=\"filled\", fillcolor=\"#82898F\", color = \"#000000\", penwidth=1.0, label=\"phys idx = %zd | elem = %.2lf | {prev = %zd | next = %zd} \"];\n", idx ,idx, list->data[idx], list->prev[idx], list->next[idx]);
            // continue;
        }
        // отрисовка ярко красным таких блоков
        if((list->prev[idx] >= list->num_of_elem || (list->prev[idx] < 0 && list->prev[idx]!=-1)) || (list->next[idx] >= list->num_of_elem || list->next[idx] < 0 )){
            // fprintf(dot_file, " %zd[shape=\"Mrecord\", style=\"filled\", fillcolor=\"#bb0d12\", color = \" #45322E\", penwidth=2.0, label=\"phys idx = %zd | elem = %.2lf | {prev = %zd | next = %zd}\"];\n", idx ,idx, list->data[idx], list->prev[idx], list->next[idx]);
            if(list->prev[idx] >= list->num_of_elem || list->prev[idx] < 0 ){
                fprintf(dot_file, " %zd[shape=\"octagon\", style=\"filled\", fillcolor=\"#bb0d12\", color = \"#000000\", penwidth=2.0, label=\"%zd\"];\n", list->prev[idx] ,list->prev[idx]);
            }
            if((list->next[idx] >= list->num_of_elem || list->next[idx] < 0 )){
                fprintf(dot_file, " %zd[shape=\"octagon\", style=\"filled\", fillcolor=\"#bb0d12\", color = \"#000000\", penwidth=2.0, label=\"%zd\"];\n", list->next[idx], list->next[idx]);
            }
            /*
            if((list->prev[idx] >= list->num_of_elem || list->prev[idx] < 0) && (list->next[idx] >= list->num_of_elem || list->next[idx] < 0 )){
                fprintf(dot_file, "{rank = below; %zd; %zd; %zd}", idx, list->prev[idx] ,list->next[idx]);
            }
            if(list->prev[idx] >= list->num_of_elem || list->prev[idx] < 0 ){
                fprintf(dot_file, "{rank = below; %zd; %zd}", idx, list->prev[idx]);
                // fprintf(dot_file, "%zd -> %zd [style=invis, weight=40, tailport=s, headport=n];\n", idx, list->prev[idx]);
            }
            if((list->next[idx] >= list->num_of_elem || list->next[idx] < 0 )){
                fprintf(dot_file, "{rank = below; %zd; %zd}", idx, list->next[idx]);
                // fprintf(dot_file, "%zd -> %zd [style=invis, weight=40, tailport=s, headport=n];\n", list->next[idx], idx);
            }
            */
            // { rank = below; 5; 75; 57; }
            // continue;
        }

        /*
        if(list->next[list->prev[idx]] != idx || list->prev[list->next[idx]] != idx){
            fprintf(dot_file, " %zd[shape=\"Mrecord\", style=\"filled\", fillcolor=\"#bb0d12\", color = \" #45322E\", penwidth=2.0, label=\"phys idx = %zd | elem = %.2lf | {prev = %zd | next = %zd}\"];\n", idx ,idx, list->data[idx], list->prev[idx], list->next[idx]);
            continue;
        }
        */
        if(list->data[idx]!=POISON){
        fprintf(dot_file, " %zd[shape=\"Mrecord\", style=\"filled\", fillcolor=\"#98FB98\", color = \"#000000\", penwidth=1.0, label=\"phys idx = %zd | elem = %.2lf | {prev = %zd | next = %zd}\"];\n", idx ,idx, list->data[idx], list->prev[idx], list->next[idx]);
        }
    }
    fprintf(dot_file, " head_label  [shape=box, style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"HEAD\"]\n");
    // fprintf(dot_file," spacer1 [style=invis, width=0, height=0, label=\"\"];\n");
    fprintf(dot_file, " tail_label [shape=box, style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"TAIL\"]\n");
    // fprintf(dot_file," spacer2 [style=invis, width=0, height=0, label=\"\"];\n");
    fprintf(dot_file, " free_label [shape=box, style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"FREE\"]\n");
    // fprintf(dot_file, "{rank=same; head_label tail_label free_label};\n");
    // fprintf(dot_file," head_label -> spacer1 -> tail_label -> spacer2 -> free_label [style=\"invis\", weight=1000]\n");

    // fprintf(dot_file, "{rank=same; %zd; head_label;}", list->next[0]);
    // fprintf(dot_file, "{rank=same; %zd; tail_label;}", list->prev[0]);
    // fprintf(dot_file, "{rank=same; %zd; free_label;}", list->free);
    // fprintf(dot_file, "{rank=same; head_label; tail_label; free_label;}\n");

    fprintf(dot_file, "head_label -> %zd [color=\"#20B2AA\", penwidth=1, arrowsize=0.85, style=\"solid\"]\n", list->next[0]);
    fprintf(dot_file, "tail_label -> %zd [color=\"#20B2AA\", penwidth=1, arrowsize=0.85, style=\"solid\"]\n", list->prev[0]);
    fprintf(dot_file, "free_label -> %zd [color=\"#20B2AA\", penwidth=1, arrowsize=0.85, style=\"solid\"]\n", list->free);


    // fprintf(dot_file, " head_label [shape=box, style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"HEAD\"]\n");
    // fprintf(dot_file, " tail_label [shape=box, style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"TAIL\"]\n");
    // fprintf(dot_file, " free_label [shape=box, style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"FREE\"]\n");
    /*
    for(ssize_t idx = 1; idx < list->num_of_elem; idx++){
        if(list->data[idx]!=POISON){
            continue;
        }
        fprintf(dot_file, " %zd[shape=\"Mrecord\", style=\"filled\", fillcolor=\"#82898F\", color = \"#000000\", penwidth=1.0, label=\"phys idx = %zd | elem = %.2lf | {prev = %zd | next = %zd} \"];\n", idx ,idx, list->data[idx], list->prev[idx], list->next[idx]);
    }
    */

    /*
    fprintf(dot_file, "{rank=same; %zd; head_label;}", list->next[0]);
    fprintf(dot_file, "{rank=same; %zd; tail_label;}", list->prev[0]);
    fprintf(dot_file, "{rank=same; %zd; free_label;}", list->free);

    fprintf(dot_file, "align_labels [shape=point, width=0, height=0, label=\"\", style=invis];\n");
    fprintf(dot_file, "align_labels -> head_label [style=invis, weight=200];\n");
    fprintf(dot_file, "align_labels -> tail_label [style=invis, weight=200];\n");
    fprintf(dot_file, "align_labels -> free_label [style=invis, weight=200];\n");

    fprintf(dot_file, "head_label -> %zd [color=\"#FFF4CC\", penwidth=1, arrowsize=0.85, style=\"solid\", constraint=false]\n", list->next[0]);
    fprintf(dot_file, "tail_label -> %zd [color=\"#FFF4CC\", penwidth=1, arrowsize=0.85, style=\"solid\", constraint=false]\n", list->prev[0]);
    fprintf(dot_file, "free_label -> %zd [color=\"#FFF4CC\", penwidth=1, arrowsize=0.85, style=\"solid\"]\n", list->free);
    */

    // connect normal elements
    ssize_t last_norm_idx = 0;
    for(ssize_t idx = 1; idx < list->num_of_elem; idx++){
        // if(list->data[idx]!=POISON){
            if(last_norm_idx == 0){
                fprintf(dot_file," %zd -> %zd [style=\"invis\", weight=1000]\n", last_norm_idx, idx);
                last_norm_idx = idx;
                // continue;
            }
            fprintf(dot_file," %zd -> %zd [style=\"invis\", weight=1000]\n", last_norm_idx, idx);
            last_norm_idx = idx;
        // }
    }

    // connect poison elements
    /*
    ssize_t poison_idx = 0;
    for(ssize_t idx = 1; idx < list->num_of_elem; idx++){
        if(list->data[idx]==POISON){
            if(poison_idx == 0){
                poison_idx = idx;
                fprintf(dot_file," %zd -> %zd [style=\"invis\", weight=10000]\n", last_norm_idx, poison_idx);
                continue;
            }
            fprintf(dot_file," %zd -> %zd [style=\"invis\", weight=10000]\n", poison_idx, idx);
            poison_idx = idx;
        }
    }
    */



    // connect next elements(двойная стрелка)
    // fprintf(dot_file, "head_label -> %zd [color=\"#20B2AA\", penwidth=1, arrowsize=0.85, style=\"solid\", constraint=false, weight=0]\n", list->next[0]);
    // fprintf(dot_file, "tail_label -> %zd [color=\"#20B2AA\", penwidth=1, arrowsize=0.85, style=\"solid\", constraint=false, weight=0]\n", list->prev[0]);
    // fprintf(dot_file, "free_label -> %zd [color=\"#20B2AA\", penwidth=1, arrowsize=0.85, style=\"solid\", constraint=false, weight=0]\n", list->free);
    for(ssize_t idx = 0; idx < list->num_of_elem; idx++){
        if((list->prev[idx] >= list->num_of_elem || (list->prev[idx] < 0 && list->prev[idx]!=-1)) || (list->next[idx] >= list->num_of_elem || list->next[idx] < 0 )){
            if((list->prev[idx] >= list->num_of_elem || (list->prev[idx] < 0 && list->prev[idx]!=-1)) && (list->next[idx] >= list->num_of_elem || list->next[idx] < 0 )){
                fprintf(dot_file, " %zd -> %zd [color = \"#FF4F00\", penwidth = 2, arrowsize = 1, constraint=false, weight=0]\n",  list->prev[idx], idx);
                fprintf(dot_file," %zd -> %zd [color = \"#0000FF\", penwidth = 2, arrowsize = 1, constraint=false, weight=0]\n", idx, list->next[idx]);
                continue;
            }
            if(list->prev[idx] >= list->num_of_elem || (list->prev[idx] < 0 && list->prev[idx]!=-1)){
                fprintf(dot_file, " %zd -> %zd [color = \"#FF4F00\", penwidth = 2, arrowsize = 1, constraint=false, weight=0]\n", list->prev[idx], idx);
                fprintf(dot_file," %zd -> %zd [color = \"#0000FF\", penwidth = 2, arrowsize = 1, constraint=false, weight=0]\n", idx, list->next[idx]);
            }
            if(list->next[idx] >= list->num_of_elem || list->next[idx] < 0){
                fprintf(dot_file, " %zd -> %zd [color = \"#FF4F00\", penwidth = 2, arrowsize = 1, constraint=false, weight=0]\n",idx, list->next[idx]);
                fprintf(dot_file," %zd -> %zd [color = \"#0000FF\", penwidth = 2, arrowsize = 1, constraint=false, weight=0]\n", list->prev[idx], idx);
            }
            continue;
        }

        
        if(list->data[idx]!=POISON && list->next[list->prev[idx]] != idx){
            fprintf(dot_file," %zd -> %zd [color = \"#FF4F00\", penwidth = 2, arrowsize = 1, dir=back, constraint=false, weight=0]\n", list->prev[idx], idx);
        }
        

        if(list->data[idx]!=POISON && list->prev[list->next[idx]] != idx){
            fprintf(dot_file," %zd -> %zd [color = \"#0000FF\", penwidth = 2, arrowsize = 1, constraint=false, weight=0]\n", idx, list->next[idx]);
            continue;
        }

        if(list->data[idx] == POISON && idx != 0){
            if((list->prev[list->next[idx]] >= list->num_of_elem || (list->prev[list->next[idx]] < 0 && list->prev[list->next[idx]]!=-1)) || (list->next[list->next[idx]] >= list->num_of_elem || list->next[list->next[idx]] < 0 )){
                continue;
            }
            fprintf(dot_file," %zd -> %zd [color = \"#44944A\", penwidth = 1, arrowsize = 0.85, constraint=false, weight=0]\n", idx, list->next[idx]);
        }
    
        if(list->data[idx] != POISON || idx == 0){
            if((list->prev[list->next[idx]] >= list->num_of_elem || (list->prev[list->next[idx]] < 0 && list->prev[list->next[idx]]!=-1)) || (list->next[list->next[idx]] >= list->num_of_elem || list->next[list->next[idx]] < 0 )){
                continue;
            }
            if(list->size == 1 && idx != 0){
                continue;
            }
            fprintf(dot_file," %zd -> %zd [color = \"#6A7075\", penwidth = 1, arrowsize = 0.85, dir=both, constraint=false, weight=0]\n", idx, list->next[idx]);
        }

    }
    fprintf(dot_file,"}\n");

    fprintf(dot_file,"subgraph gr1{\n");

    /*

    for(ssize_t idx = 0; idx < list->num_of_elem; idx++){
        if(idx==list->next[0]){
            fprintf(dot_file, " n%zd [shape=box, style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"HEAD\"]\n");
            fprintf(dot_file, "{rank=same; %zd; n%zd;}\n", list->next[0]);
            continue;
        }
        if(idx==list->prev[0]){
            fprintf(dot_file, " n%zd [shape=box, style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"TAIL\"]\n");
            fprintf(dot_file, "{rank=same; %zd; n%zd;}\n", list->prev[0]);
            continue;
        }
        if(idx==list->free){
            fprintf(dot_file, " n%zd [shape=box, style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"FREE\"]\n");
            fprintf(dot_file, "{rank=same; %zd; n%zd;}\n", list->free);
            continue;
        }
        fprintf(dot_file, " n%zd [style=\"invis\", width=0, height=0, label=\"\"]\n", idx);
    }

    ssize_t last_norm_idx1 = 0;
    for(ssize_t idx = 1; idx < list->num_of_elem; idx++){
        if(last_norm_idx1 == 0){
            fprintf(dot_file," n%zd -> n%zd [style=\"invis\", weight=1000]\n", last_norm_idx1, idx);
            last_norm_idx1 = idx;
        }
        fprintf(dot_file," n%zd -> n%zd [style=\"invis\", weight=1000]\n", last_norm_idx1, idx);
        last_norm_idx1 = idx;
    }
    */


    // fprintf(dot_file, " head_label  [shape=box, style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"HEAD\"]\n");
    // fprintf(dot_file," spacer1 [style=invis, width=0, height=0, label=\"\"];\n");
    // fprintf(dot_file, " tail_label [shape=box, style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"TAIL\"]\n");
    // fprintf(dot_file," spacer2 [style=invis, width=0, height=0, label=\"\"];\n");
    // fprintf(dot_file, " free_label [shape=box, style=\"filled\", fillcolor=\"#20B2AA\", color=\"#000000\", fontcolor=\"#000000\", fontsize=12, label=\"FREE\"]\n");
    // fprintf(dot_file, "{rank=same; head_label tail_label free_label};\n");
    // fprintf(dot_file," head_label -> spacer1 -> tail_label -> spacer2 -> free_label [style=\"invis\", weight=1000]\n");

    // fprintf(dot_file, "{rank=same; %zd; head_label;}", list->next[0]);
    // fprintf(dot_file, "{rank=same; %zd; tail_label;}", list->prev[0]);
    // fprintf(dot_file, "{rank=same; %zd; free_label;}", list->free);
    // fprintf(dot_file, "{rank=min; head_label; tail_label; free_label;}\n");

    // fprintf(dot_file, "head_label -> %zd [color=\"#20B2AA\", penwidth=1, arrowsize=0.85, style=\"solid\", constraint=false, weight=0]\n", list->next[0]);
    // fprintf(dot_file, "tail_label -> %zd [color=\"#20B2AA\", penwidth=1, arrowsize=0.85, style=\"solid\", constraint=false, weight=0]\n", list->prev[0]);
    // fprintf(dot_file, "free_label -> %zd [color=\"#20B2AA\", penwidth=1, arrowsize=0.85, style=\"solid\", constraint=false, weight=0]\n", list->free);

    fprintf(dot_file,"}\n");

    fprintf(dot_file,"}\n");
    fclose(dot_file);
}

static void generate_svg_file(const filenames_for_dump* dump){
    char command[200] = {0};

    if(snprintf(command, sizeof(command) ,"dot -Tsvg %s -o %s", dump->dot_filename, dump->svg_filename) == -1){
        fprintf(stderr, "Can't parse command to do\n");
        return;
    }
    int sys_res = system(command);
    check_and_dump_system_work(sys_res);
}

static int check_and_dump_system_work(int sys_res){
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

// list->prev[list->next[data_idx]] != data_idx
// list->next[list->prev[data_idx]] != data_idx