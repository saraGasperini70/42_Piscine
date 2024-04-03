#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>

int	ft_putnbr_base(int nb, int base)
{
	char	*map = "0123456789abcdef";
	int		i = 0;

	if (nb < 0)
	{
		nb *= -1;
		write (1, "-", 1);
	}

	if (nb >= base)
		i += ft_putnbr_base(nb / base, base);
	i += write (1, &map[nb % base], 1);
	return (i);
}

int	ft_putstr(char *str)
{
	int	i = 0;

	if (!str)
		str = "(null)";
	while (str[i])
		i += write(1, &str[i], 1);
	return (i);
}

int	ft_printf(const char *format, ...)
{
	va_list	start;
	va_start(start, format);
	int	i = 0;
	int	j = 0;

	while (format[i])
	{
		if (format[i] == '%' && (format[i + 1] == 's' || format[i + 1] == 'd' || format[i + 1] == 'x'))
		{
			if (format[i + 1] == 's')
				j += ft_putstr(va_arg(start, char *));
			if (format[i + 1] == 'd')
				j += ft_putnbr_base(va_arg(start, int), 10);
			if (format[i + 1] == 'x')
				j += ft_putnbr_base(va_arg(start, int), 16);
			i++;
		}
		else
			write (1, &format[i], 1);
		i++;
	}
	return (j);
}

int	main(void)
{
	ft_printf("Hello, %s, %d, %x\n", "World", 70, 70);
	printf("Hello, %s, %d, %x\n", "World", 70, 70);
	printf("%p\n", NULL);
	return (0);
}
