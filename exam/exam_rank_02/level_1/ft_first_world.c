#include <unistd.h>

int main(int ac, char **av)
{
    int count;
    
    count = 0;
	if (ac == 2)
	{
		while (av[1][count] == '\0' || av[1][count] == ' ' || av[1][count] == '\t')
			count++;
        while (av[1][count] != '\0' && av[1][count] != ' ' && av[1][count] != '\t')
        {
            write (1, &av[1][count], 1);
            count++;	
		}
	}
	write (1, "\n", 1);
	return (0);
}
