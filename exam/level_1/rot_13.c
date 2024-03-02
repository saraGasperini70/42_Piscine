#include <unistd.h>
void    ft_rot13(char *str)
{
    int c;
    
    c = 0;
    while (str[c] != '\0')
    {
        if ((str[c] >= 'a' && str[c] <= 'm') || (str[c] >= 'A' && str[c] <= 'M'))
        {
            str[c] += 13;
        }
        else if ((str[c] >= 'n' && str[c] <= 'z') || (str[c] >= 'N' && str[c] <= 'Z'))
        {
            str[c] -= 13;
        }
        write (1, &str[c], 1);
        c++;
    }
}

int main(int ac, char **av)
{
    if (ac == 2)
    {
        ft_rot13(av[1]);
    }
    write (1, "\n", 1);
}
