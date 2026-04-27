/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 11:41:34 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/27 18:15:50 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	*convert(unsigned int num, int base, int isUpper)
{
	char		*tab;
	static char	buffer[50];
	char		*ptr;

	if (isUpper)
		tab = "0123456789ABCDEF";
	else
		tab = "0123456789abcdef";
	ptr = &buffer[49];
	*ptr = '\0';
	if (num == 0)
	{
		*--ptr = tab[0];
		return (ptr);
	}
	while (num != 0)
	{
		*--ptr = tab[num % base];
		num /= base;
	}
	return (ptr);
}

int ft_printf(const char *format, ...)
{
	const char	*pos;
	va_list		arg;

	va_start(arg, format);
	pos = format;
	while (*pos)
	{
		if (*pos == '%')
		{
			pos++;
			if (*pos == 'c')
				ft_putchar_fd(va_arg(arg, int), 1);
			else if (*pos == 's')
				ft_putstr_fd(va_arg(arg, char *), 1);
			else if (*pos == 'p')
				return (0);
			else if (*pos == 'd' || *pos == 'i')
				ft_putnbr_fd(va_arg(arg, int), 1);
			else if (*pos == 'u')
				return (0);
			else if (*pos == 'x')
				ft_putstr_fd(convert(va_arg(arg, unsigned int), 16, 0), 1);
			else if (*pos == 'X')
				ft_putstr_fd(convert(va_arg(arg, unsigned int), 16, 1), 1);
			else if (*pos == '%')
				ft_putchar_fd('%', 1);

			if (*pos)
				pos++;
		}
		else
		{
			ft_putchar_fd(*pos, 1);
			pos++;
		}
	}
	va_end(arg);
	return (0);
}
#include <stdio.h>
#include <limits.h>
#include "ft_printf.h"

static int	pass = 0;
static int	fail = 0;

static void	check(const char *label, int ft, int std)
{
	if (ft == std)
	{
		printf("  [OK] %s  (ret=%d)\n", label, ft);
		pass++;
	}
	else
	{
		printf("  [KO] %s  ft=%d | std=%d\n", label, ft, std);
		fail++;
	}
}

int	main(void)
{
	int			f;
	int			s;
	void		*ptr;

	/* ── %c ──────────────────────────────────────────────── */
	printf("\n=== %%c ===\n");
	f = ft_printf("%c", 'A');        s = printf("%c", 'A');        check("lettre majuscule", f, s); printf("\n");
	f = ft_printf("%c", 'z');        s = printf("%c", 'z');        check("lettre minuscule", f, s); printf("\n");
	f = ft_printf("%c", '0');        s = printf("%c", '0');        check("chiffre",          f, s); printf("\n");
	f = ft_printf("%c", ' ');        s = printf("%c", ' ');        check("espace",            f, s); printf("\n");
	f = ft_printf("%c", 1);          s = printf("%c", 1);          check("char=1",            f, s); printf("\n");

	/* ── %s ──────────────────────────────────────────────── */
	printf("\n=== %%s ===\n");
	f = ft_printf("%s", "hello");           s = printf("%s", "hello");           check("chaine simple",      f, s); printf("\n");
	f = ft_printf("%s", "");                s = printf("%s", "");                check("chaine vide",        f, s); printf("\n");
	f = ft_printf("%s", "un deux trois");   s = printf("%s", "un deux trois");   check("chaine avec espaces",f, s); printf("\n");
	/* NULL : printf standard est UB, on teste seulement ft_printf */
	f = ft_printf("%s", (char *)NULL);
	printf("  [INFO] %%s NULL -> ft_printf retourne %d (pas de comparaison std)\n", f);

	/* ── %d / %i ─────────────────────────────────────────── */
	printf("\n=== %%d / %%i ===\n");
	f = ft_printf("%d", 0);              s = printf("%d", 0);              check("zero",    f, s); printf("\n");
	f = ft_printf("%d", 42);             s = printf("%d", 42);             check("positif", f, s); printf("\n");
	f = ft_printf("%d", -42);            s = printf("%d", -42);            check("negatif", f, s); printf("\n");
	f = ft_printf("%d", INT_MAX);        s = printf("%d", INT_MAX);        check("INT_MAX", f, s); printf("\n");
	f = ft_printf("%d", INT_MIN);        s = printf("%d", INT_MIN);        check("INT_MIN", f, s); printf("\n");
	f = ft_printf("%i", -100);           s = printf("%i", -100);           check("%%i neg", f, s); printf("\n");

	/* ── %u ──────────────────────────────────────────────── */
	printf("\n=== %%u ===\n");
	f = ft_printf("%u", 0U);             s = printf("%u", 0U);             check("zero",          f, s); printf("\n");
	f = ft_printf("%u", 42U);            s = printf("%u", 42U);            check("positif",       f, s); printf("\n");
	f = ft_printf("%u", UINT_MAX);       s = printf("%u", UINT_MAX);       check("UINT_MAX",      f, s); printf("\n");
	f = ft_printf("%u", (unsigned int)-1);s= printf("%u", (unsigned int)-1);check("cast -1->uint",f, s); printf("\n");

	/* ── %x / %X ─────────────────────────────────────────── */
	printf("\n=== %%x / %%X ===\n");
	f = ft_printf("%x", 0U);             s = printf("%x", 0U);             check("zero",      f, s); printf("\n");
	f = ft_printf("%x", 255U);           s = printf("%x", 255U);           check("255->ff",   f, s); printf("\n");
	f = ft_printf("%X", 255U);           s = printf("%X", 255U);           check("255->FF",   f, s); printf("\n");
	f = ft_printf("%x", UINT_MAX);       s = printf("%x", UINT_MAX);       check("UINT_MAX",  f, s); printf("\n");
	f = ft_printf("%x", (unsigned int)-1);s= printf("%x",(unsigned int)-1);check("-1->hexa",  f, s); printf("\n");

	/* ── %p ──────────────────────────────────────────────── */
	printf("\n=== %%p ===\n");
	ptr = (void *)0x1234abcd;
	f = ft_printf("%p", ptr);            s = printf("%p", ptr);            check("pointeur fixe", f, s); printf("\n");
	f = ft_printf("%p", NULL);           s = printf("%p", NULL);           check("pointeur NULL", f, s); printf("\n");

	/* ── %% ──────────────────────────────────────────────── */
	printf("\n=== %%%% ===\n");
	f = ft_printf("100%%");              s = printf("100%%");              check("pourcent simple", f, s); printf("\n");
	f = ft_printf("%%%%");               s = printf("%%%%");               check("double pourcent", f, s); printf("\n");

	/* ── Valeurs de retour ───────────────────────────────── */
	printf("\n=== valeurs de retour ===\n");
	f = ft_printf("abc");                s = printf("abc");                check("ret 3 chars",  f, s); printf("\n");
	f = ft_printf("%s", "");             s = printf("%s", "");             check("ret vide",     f, s); printf("\n");
	f = ft_printf("%d", INT_MIN);        s = printf("%d", INT_MIN);        check("ret INT_MIN",  f, s); printf("\n");
	f = ft_printf("a%cb%sc%d", 'X', "YY", 99);
	s =    printf("a%cb%sc%d", 'X', "YY", 99);
	check("ret mixte", f, s); printf("\n");

	/* ── Formats mixtes ──────────────────────────────────── */
	printf("\n=== formats mixtes ===\n");
	f = ft_printf("%s a %d ans, code %X, ptr %p\n", "Bob", 30, 0xFFU, (void *)0xCAFE);
	s =    printf("%s a %d ans, code %X, ptr %p\n", "Bob", 30, 0xFFU, (void *)0xCAFE);
	check("mixte complet", f, s);

	f = ft_printf("%d %d %d %d %d\n", 1, -1, 0, INT_MAX, INT_MIN);
	s =    printf("%d %d %d %d %d\n", 1, -1, 0, INT_MAX, INT_MIN);
	check("cinq entiers",  f, s);

	/* ── Bilan ───────────────────────────────────────────── */
	printf("\n══════════════════════════════════\n");
	printf("  Résultat : %d/%d tests réussis\n", pass, pass + fail);
	if (fail == 0)
		printf("  Tout bon !\n");
	else
		printf("  %d échec(s) à corriger.\n", fail);
	printf("══════════════════════════════════\n\n");
	return (fail > 0 ? 1 : 0);
}
