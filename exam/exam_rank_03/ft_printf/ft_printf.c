#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
void	ft_putstr(char *str, int *len)
{
	if (!str)
		str = "(null)";
	while (*str)
	{
		*len += write (1, str, 1);
		str++;
	}
}

void	ft_putnbr_base(long long int nb, int base, int *len)
{
	char	*base_str = "0123456789abcdef";


	if (nb < 0)
	{
		nb *= -1;
		*len += write (1, "-", 1);
	}
	if (nb >= base)
		ft_putnbr_base(nb / base, base, len);
	*len += write (1, &base_str[nb % base], 1);
}

int	ft_printf(const char *prompt, ...)
{
	va_list	start;
	int	i = 0;
	int	result = 0;

	va_start(start, prompt);
	while (prompt[i])
	{
		if (prompt[i] == '%' && (prompt[i + 1] == 's' || prompt[i + 1] == 'd' || prompt[i + 1] == 'x'))
		{
			i++;
			if (prompt[i] == 's')
				ft_putstr(va_arg(start, char *), &result);
			if (prompt[i] == 'd')
				ft_putnbr_base((long long int)va_arg(start, int), 10, &result);
			if (prompt[i] == 'x')
				ft_putnbr_base((long long int)va_arg(start, unsigned int), 16, &result);
		}
		else
			result += write (1, &prompt[i], 1);
		i++;
	}
	return (va_end(start), result);
}

/*
#include <stdio.h>
#include <stdarg.h>
int
	main(void)
{
	int	r;

	r = 0;
	printf("%d\n", printf("Simple test\n"));
	ft_printf("%d\n", ft_printf("Simple test\n"));
	printf("%d\n", printf(""));
	ft_printf("%d\n", ft_printf(""));
	printf("%d\n", printf("--Format---"));
	ft_printf("%d\n", ft_printf("--Format---"));
	printf("%d\n", printf("\n"));
	ft_printf("%d\n", ft_printf("\n"));
	printf("%d\n", printf("%d\n", 0));
	ft_printf("%d\n", ft_printf("%d\n", 0));
	printf("%d\n", printf("%d\n", 42));
	ft_printf("%d\n", ft_printf("%d\n", 42));
	printf("%d\n", printf("%d\n", 1));
	ft_printf("%d\n", ft_printf("%d\n", 1));
	printf("%d\n", printf("%d\n", 5454));
	ft_printf("%d\n", ft_printf("%d\n", 5454));
	printf("%d\n", printf("%d\n", (int)2147483647));
	ft_printf("%d\n", ft_printf("%d\n", (int)2147483647));
	printf("%d\n", printf("%d\n", (int)2147483648));
	ft_printf("%d\n", ft_printf("%d\n", (int)2147483648));
	printf("%d\n", printf("%d\n", (int)-2147483648));
	ft_printf("%d\n", ft_printf("%d\n", (int)-2147483648));
	printf("%d\n", printf("%d\n", (int)-2147483649));
	ft_printf("%d\n", ft_printf("%d\n", (int)-2147483649));
	printf("%d\n", printf("\n"));
	ft_printf("%d\n", ft_printf("\n"));
	printf("%d\n", printf("%x\n", 0));
	ft_printf("%d\n", ft_printf("%x\n", 0));
	printf("%d\n", printf("%x\n", 42));
	ft_printf("%d\n", ft_printf("%x\n", 42));
	printf("%d\n", printf("%x\n", 1));
	ft_printf("%d\n", ft_printf("%x\n", 1));
	printf("%d\n", printf("%x\n", 5454));
	ft_printf("%d\n", ft_printf("%x\n", 5454));
	printf("%d\n", printf("%x\n", (int)2147483647));
	ft_printf("%d\n", ft_printf("%x\n", (int)2147483647));
	printf("%d\n", printf("%x\n", (int)2147483648));
	ft_printf("%d\n", ft_printf("%x\n", (int)2147483648));
	printf("%d\n", printf("%x\n", (int)-2147483648));
	ft_printf("%d\n", ft_printf("%x\n", (int)-2147483648));
	printf("%d\n", printf("%x\n", (int)-2147483649));
	ft_printf("%d\n", ft_printf("%x\n", (int)-2147483649));
	printf("%d\n", printf("%x\n", (int)0xFFFFFFFF));
	ft_printf("%d\n", ft_printf("%x\n", (int)0xFFFFFFFF));
	printf("%d\n", printf("\n"));
	ft_printf("%d\n", ft_printf("\n"));
	printf("%d\n", printf("%s\n", ""));
	ft_printf("%d\n", ft_printf("%s\n", ""));
	printf("%d\n", printf("%s\n", "toto"));
	ft_printf("%d\n", ft_printf("%s\n", "toto"));
	printf("%d\n", printf("%s\n", "wiurwuyrhwrywuier"));
	ft_printf("%d\n", ft_printf("%s\n", "wiurwuyrhwrywuier"));
	printf("%d\n", printf("%s\n", NULL));
	ft_printf("%d\n", ft_printf("%s\n", NULL));
	printf("%d\n", printf("-%s-%s-%s-%s-\n", "", "toto", "wiurwuyrhwrywuier", NULL));
	ft_printf("%d\n", ft_printf("-%s-%s-%s-%s-\n", "", "toto", "wiurwuyrhwrywuier", NULL));
	printf("%d\n", printf("\n--Mixed---\n"));
	ft_printf("%d\n", ft_printf("\n--Mixed---\n"));
	printf("%d\n", printf("%d%x%d%x%d%x%d%x\n", 0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648));
	ft_printf("%d\n", ft_printf("%d%x%d%x%d%x%d%x\n", 0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648));
	printf("%d\n", printf("-%d-%x-%d-%x-%d-%x-%d-%x-\n", 0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648));
	ft_printf("%d\n", ft_printf("-%d-%x-%d-%x-%d-%x-%d-%x-\n", 0, 0, 42, 42, 2147483647, 2147483647, (int)-2147483648, (int)-2147483648));
	printf("%d\n", printf("\n"));
	ft_printf("%d\n", ft_printf("\n"));
	printf("%d\n", printf("%s%s%s%s\n", "", "toto", "wiurwuyrhwrywuier", NULL));
	ft_printf("%d\n", ft_printf("%s%s%s%s\n", "", "toto", "wiurwuyrhwrywuier", NULL));
	printf("%d\n", printf("-%s-%s-%s-%s-\n", "", "toto", "wiurwuyrhwrywuier", NULL));
	ft_printf("%d\n", ft_printf("-%s-%s-%s-%s-\n", "", "toto", "wiurwuyrhwrywuier", NULL));
}*/
