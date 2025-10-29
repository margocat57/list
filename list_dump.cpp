#include "list_struct.h"
#include "list_dump.h"
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

static filenames_for_dump filename_ctor();

static int check_and_dump_system_work(int sys_res);

static void generate_dot_file(const list* list, const char* dot_filename);

static void generate_svg_file(const filenames_for_dump* dump);

static void draw_table(const list* list, FILE* html_output);

static void list_dump_html(const list* list, const char* output, const char* img, const char* debug_msg, const char *file, const char *func, int line);

void list_dump_func(const list* list, const char* output_file, const char* debug_msg, const char *file, const char *func,  int line){
    filenames_for_dump dump = filename_ctor();
    generate_dot_file(list, dump.dot_filename);
    generate_svg_file(&dump);
    if(dump.svg_filename){
        list_dump_html(list, output_file, dump.svg_filename, debug_msg, file, func, line);
    }
    free(dump.svg_filename);
    free(dump.dot_filename);
}

static void list_dump_html(const list* list, const char* output, const char* img, const char* debug_msg, const char *file, const char *func,  int line){
    static int launch_num = 0;
    FILE* html_output = NULL;
    if(launch_num == 0){
        html_output = fopen(output, "w");
        CHECK_AND_RET_DUMP(!html_output, "Can't open html file\n");
        launch_num++;
        fprintf(html_output, "<pre style=\"background-color: #1E2A36; color: #FFFFFF;\">");
        fprintf(html_output, "<p style=\"font-size: 50px; text-align: center;\"> LIST DUMP\n");
    }
    else{
        html_output = fopen(output, "a+");
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
    fprintf(html_output, "<table style=\"text-align: center; font-size: 15px; border: 2px solid #FFFFFF; border-collapse: collapse; width: 50%%; color: #FFFFFF;\">\n");

    fprintf(html_output, "<tr>\n");
    fprintf(html_output, "<td>Index</td>");
    for (int i = 0; i < (int)list->num_of_elem; i++) {
        fprintf(html_output, "<td style = \"border: 1px solid #FFFFFF\";>%d</td>\n", i);
    }
    fprintf(html_output, "</tr>\n");

    fprintf(html_output, "<tr>\n");
    fprintf(html_output, "<td>Data</td>");
    for (int i = 0; i < (int)list->num_of_elem; i++) {
        fprintf(html_output, "<td style = \"border: 1px solid #FFFFFF\";>%.2lf</td>\n", list->data[i]);
    }
    fprintf(html_output, "</tr>\n");

    fprintf(html_output, "<tr>\n");
    fprintf(html_output, "<td>Next</td>");
    for (int i = 0; i < (int)list->num_of_elem; i++) {
        fprintf(html_output, "<td style = \"border: 1px solid #FFFFFF\";>%d</td>\n", list->next[i]);
    }
    fprintf(html_output, "</tr>\n");

    fprintf(html_output, "<tr>\n");
    fprintf(html_output, "<td>Prev</td>");
    for (int i = 0; i < (int)list->num_of_elem; i++) {
        fprintf(html_output, "<td style = \"border: 1px solid #FFFFFF\";>%d</td>\n", list->pred[i]);
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
        fprintf(stderr, "Allocation error");
        return dump;
    }

    char* svg_filename = (char*)calloc(100, sizeof(char));
    if(!svg_filename){
        fprintf(stderr, "Allocation error");
        return dump;
    }

    dump.svg_filename = svg_filename;
    dump.dot_filename = dot_filename;

    time_t now = time(NULL); // количество секунд с 1970, не сохраняем никуда
    struct tm *t = localtime(&now); // преобразует в года/часы/... - поля структуры

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
        fprintf(stderr, "Cannot statr to do command\n");
        return -1;
    }
    if(WIFEXITED(sys_res)){
        int exit_stat = WEXITSTATUS(sys_res);
        fprintf(stderr, "Creating svg file finished with mistake %d\n", exit_stat);
        return exit_stat;
    }
    if(WIFSIGNALED(sys_res)){
        int sign_that_stop = WTERMSIG(sys_res);
        fprintf(stderr, "Coomand was finished by signal %d\n", sign_that_stop);
        return sign_that_stop;
    }
    return 0;
}