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
    char swap;
    
    i = 0;
    j = ft_strlen(str) - 1;
    while (i < j / 2)
    {
        swap = str[i];
        str[i] = str[j];
        str[j] = swap;
        i++;
        j--;
    }
    return(str);
}

int main()
{
    char c[] = "abbiamo l'esame";
    printf("Prima: %s\n", c);
    printf("Dopo: %s\n", ft_strrev(c));
}
