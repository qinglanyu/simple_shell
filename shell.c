#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define foo(x) _Generic(x, int : foo_int,\
                       double : foo_double)(x)

typedef enum {
    DT_S08,
    DT_S16,
    DT_S32,
    DT_U08,
    DT_U16,
    DT_U32,
    DT_STR,
    DT_FLT,
    DT_ULL,
    DT_SLL
} emDT;

#define typename(x) _Generic(x, int : DT_S32,\
    short : DT_S16,\
    char  : DT_S08,\
    unsigned int : DT_U32,\
    unsigned short: DT_U16,\
    unsigned char : DT_U08,\
    char * : DT_STR,\
    float : DT_FLT)

typedef struct variable_info_item{
    const char *var_name;
    void *var_addr;
    emDT var_type;
} variable_info_item;

static variable_info_item global_variable_list[30];
static int global_variable_count = 0;

#define COLLECT_GLOBAL_VARIABLE(var) \
    global_variable_list[global_variable_count].var_name = #var;\
    global_variable_list[global_variable_count].var_addr = (void *)&var;\
    global_variable_list[global_variable_count].var_type = typename(var);\
    global_variable_count++;

/* global variables definition */
int age = 20;
char *name = "Jack";
float pi = 3.1415926;
char gender = 'f';

void init_global_variable_list(void)
{
    COLLECT_GLOBAL_VARIABLE(age);
    COLLECT_GLOBAL_VARIABLE(name);
    COLLECT_GLOBAL_VARIABLE(pi);
    COLLECT_GLOBAL_VARIABLE(gender);
}

char *skip_spaces(const char *str)
{
    while (isspace(*str)){
        ++str;
    }
    return (char*) str;
}

char *strim(char *s)
{
    size_t size;
    char *end;

    size = strlen(s);
    if (!size)
        return s;

    end = s + size - 1;
    while (end >= s && isspace(*end)){
        end--;
    }
    *(end+1) = '\0';

    return skip_spaces(s);
}

void shell_parse_cmd(const char *cmd)
{
    char major[32];
    char minor[32];
    int idx = 0;

    sscanf(cmd, "%s %s", major, minor);
    printf("%s %s\n", major, minor);

    for(; idx < global_variable_count; ++idx){
        if (0 == strcmp(major, global_variable_list[idx].var_name)) {
            switch(global_variable_list[idx].var_type){
                case DT_S08:
                    printf("%s = %c, of type char.\n", major, *(char *)global_variable_list[idx].var_addr);
                    break;
            }
            break;
        }
    }

    if (idx == global_variable_count) {
        printf("unknown variable: %s\n", major);
    }
}

void foo_int(int x)
{
    printf("%s is called for x:%d.\n", __FUNCTION__, x);
}

void foo_double(double x)
{
    printf("%s is called for x:%f.\n", __FUNCTION__, x);
}

int main(int argc, char *argv[])
{
    char cmd[64] = {0};

    foo(3.1415926);
    foo(1024);

    typeof(cmd) p = {'@'};
    printf("%c\n", p[0]);

    init_global_variable_list();

    printf("input cmd:");

    gets(cmd);

    shell_parse_cmd(cmd);

    return 0;
}


