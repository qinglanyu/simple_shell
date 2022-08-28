#include <stdio.h>

void shell_parse_cmd(const char *cmd)
{
    char major[32];
    char minor[32];

    sscanf(cmd, "%s %s", major, minor);
    printf("%s %s\n", major, minor);
}

int main(int argc, char *argv[])
{
    char cmd[64] = {0};

	printf("input cmd:");

    gets(cmd);

    shell_parse_cmd(cmd);

    return 0;
}




