#include <stdio.h>
#include <ctype.h>

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
		...
	}
}

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


