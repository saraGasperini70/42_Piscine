#include <unistd.h>
#include <stdio.h>

int ft_strlen(char *str)
{
    int count;
    
    count = 0;
    while (str[count])
        count++;
    return(count);
}

char    *ft_strrev(char *str)
{
    int i;
    int j;
    char    swap;

    i = - 1;
    j = ft_strlen(str);
    while (++i < j / 2)
    {
        swap = str[i];
        str[i] = str[j - 1 - i];
        str[j - 1 - i] = swap;
    }
    return (str);
}