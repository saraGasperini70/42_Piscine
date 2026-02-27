#include <unistd.h>
int main(int ac, char **av)
{
    int count;
    
    count = 0;
    if (ac == 2)
    {
        while (av[1][count])
        {
            if (av[1][count] == 'a')
            {
                write (1, "a", 1);
				break ;
            }
			count++;
        }
		write (1, "\n", 1);
	}
    else
    {
        write (1, "a\n", 1);
    }
	return (0);
}
