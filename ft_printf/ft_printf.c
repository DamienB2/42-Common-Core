/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 11:41:34 by dbaise            #+#    #+#             */
/*   Updated: 2026/04/28 16:05:54 by dbaise           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	process(int count, const char *pos, va_list arg)
{
	if (*pos == 'c')
		count += ft_putchar(va_arg(arg, int));
	else if (*pos == 's')
		count += ft_putstr(va_arg(arg, char *));
	else if (*pos == 'p')
		count += ft_putaddr(va_arg(arg, void *));
	else if (*pos == 'd' || *pos == 'i')
		count += ft_putnbr(va_arg(arg, int));
	else if (*pos == 'u')
		count += ft_unsigned_putnbr(va_arg(arg, unsigned int));
	else if (*pos == 'x')
		count += ft_putstr(convert(va_arg(arg, unsigned int), 16, 0));
	else if (*pos == 'X')
		count += ft_putstr(convert(va_arg(arg, unsigned int), 16, 1));
	else if (*pos == '%')
		count += ft_putchar('%');
	return (count);
}

int	ft_printf(const char *format, ...)
{
	int			count;
	const char	*pos;
	va_list		arg;

	va_start(arg, format);
	pos = format;
	count = 0;
	while (*pos)
	{
		if (*pos == '%')
		{
			pos++;
			count = process(count, pos, arg);
			if (*pos)
				pos++;
		}
		else
		{
			ft_putchar_fd(*pos, 1);
			pos++;
			count++;
		}
	}
	va_end(arg);
	return (count);
}

// #include <stdio.h>
// #include <limits.h>
// #include "ft_printf.h"

// // Couleurs ANSI
// #define RESET   "\033[0m"
// #define RED     "\033[31m"
// #define GREEN   "\033[32m"
// #define YELLOW  "\033[33m"
// #define CYAN    "\033[36m"
// #define BOLD    "\033[1m"

// static int  pass = 0;
// static int  fail = 0;

// static void check(const char *label, int ft, int std)
// {
//     printf("  %-35s : ", label);
//     if (ft == std)
//     {
//         printf("[" GREEN "OK" RESET "] (ret: " CYAN "%d" RESET ")\n", ft);
//         pass++;
//     }
//     else
//     {
//         printf("[" RED "KO" RESET "] ft=" RED "%-4d" RESET " | std=" GREEN "%d" RESET "\n", ft, std);
//         fail++;
//     }
// }

// static void section(const char *title)
// {
//     printf(BOLD CYAN "\n=== %s ===\n" RESET, title);
// }

// int main(void)
// {
//     int f, s;

//     /* ------------------------------------------------------------------ */
//     /*  %c — caractères                                                     */
//     /* ------------------------------------------------------------------ */
//     section("TEST DES CARACTÈRES (%c)");

//     printf("Sortie : ["); f = ft_printf("%c", 'A');
//     printf("] | [");      s =    printf("%c", 'A'); printf("]\n");
//     check("Lettre majuscule", f, s);

//     printf("Sortie : ["); f = ft_printf("%c", 'z');
//     printf("] | [");      s =    printf("%c", 'z'); printf("]\n");
//     check("Lettre minuscule", f, s);

//     printf("Sortie : ["); f = ft_printf("%c", '0');
//     printf("] | [");      s =    printf("%c", '0'); printf("]\n");
//     check("Chiffre '0'", f, s);

//     printf("Sortie : ["); f = ft_printf("%c", ' ');
//     printf("] | [");      s =    printf("%c", ' '); printf("]\n");
//     check("Espace", f, s);

//     printf("Sortie : ["); f = ft_printf("%c", '\n');
//     printf("] | [");      s =    printf("%c", '\n'); printf("]\n");
//     check("Newline (\\n)", f, s);

//     /* char 0 — cas piège : printf retourne 1 mais n'imprime rien de visible */
//     printf("Sortie : ["); f = ft_printf("%c", 0);
//     printf("] | [");      s =    printf("%c", 0); printf("]\n");
//     check("Char nul (valeur 0)", f, s);

//     /* ------------------------------------------------------------------ */
//     /*  %s — chaînes                                                        */
//     /* ------------------------------------------------------------------ */
//     section("TEST DES STRINGS (%s)");

//     printf("Sortie : ["); f = ft_printf("%s", "42");
//     printf("] | [");      s =    printf("%s", "42"); printf("]\n");
//     check("String classique", f, s);

//     printf("Sortie : ["); f = ft_printf("%s", "");
//     printf("] | [");      s =    printf("%s", ""); printf("]\n");
//     check("String vide", f, s);

//     printf("Sortie : ["); f = ft_printf("%s", "Hello, World!");
//     printf("] | [");      s =    printf("%s", "Hello, World!"); printf("]\n");
//     check("String longue", f, s);

//     printf("Sortie : ["); f = ft_printf("%s", "with spaces and\ttabs");
//     printf("] | [");      s =    printf("%s", "with spaces and\ttabs"); printf("]\n");
//     check("String avec espaces/tabs", f, s);

//     /* NULL — comportement défini par la norme : affiche "(null)" */
//     {
//         char *null_str = NULL;
//         printf("Sortie : ["); f = ft_printf("%s", null_str);
//         printf("] | [");      s =    printf("%s", null_str); printf("]\n");
//         check("String NULL", f, s);
//     }

//     /* ------------------------------------------------------------------ */
//     /*  %d / %i — entiers signés                                            */
//     /* ------------------------------------------------------------------ */
//     section("TEST DES ENTIERS (%d / %i)");

//     printf("Sortie : ["); f = ft_printf("%d", 0);
//     printf("] | [");      s =    printf("%d", 0); printf("]\n");
//     check("Zéro", f, s);

//     printf("Sortie : ["); f = ft_printf("%d", 1);
//     printf("] | [");      s =    printf("%d", 1); printf("]\n");
//     check("Un", f, s);

//     printf("Sortie : ["); f = ft_printf("%d", -1);
//     printf("] | [");      s =    printf("%d", -1); printf("]\n");
//     check("Moins un", f, s);

//     printf("Sortie : ["); f = ft_printf("%d", -100);
//     printf("] | [");      s =    printf("%d", -100); printf("]\n");
//     check("Négatif (-100)", f, s);

//     printf("Sortie : ["); f = ft_printf("%d", 42);
//     printf("] | [");      s =    printf("%d", 42); printf("]\n");
//     check("42", f, s);

//     printf("Sortie : ["); f = ft_printf("%d", INT_MAX);
//     printf("] | [");      s =    printf("%d", INT_MAX); printf("]\n");
//     check("INT_MAX", f, s);

//     printf("Sortie : ["); f = ft_printf("%d", INT_MIN);
//     printf("] | [");      s =    printf("%d", INT_MIN); printf("]\n");
//     check("INT_MIN", f, s);

//     printf("Sortie : ["); f = ft_printf("%i", 0);
//     printf("] | [");      s =    printf("%i", 0); printf("]\n");
//     check("%%i zéro", f, s);

//     printf("Sortie : ["); f = ft_printf("%i", INT_MIN);
//     printf("] | [");      s =    printf("%i", INT_MIN); printf("]\n");
//     check("%%i INT_MIN", f, s);

//     printf("Sortie : ["); f = ft_printf("%i", INT_MAX);
//     printf("] | [");      s =    printf("%i", INT_MAX); printf("]\n");
//     check("%%i INT_MAX", f, s);

//     /* ------------------------------------------------------------------ */
//     /*  %u — entiers non signés                                             */
//     /* ------------------------------------------------------------------ */
//     section("TEST DES UNSIGNED (%u)");

//     printf("Sortie : ["); f = ft_printf("%u", 0);
//     printf("] | [");      s =    printf("%u", 0); printf("]\n");
//     check("Zéro", f, s);

//     printf("Sortie : ["); f = ft_printf("%u", 42);
//     printf("] | [");      s =    printf("%u", 42); printf("]\n");
//     check("42", f, s);

//     printf("Sortie : ["); f = ft_printf("%u", UINT_MAX);
//     printf("] | [");      s =    printf("%u", UINT_MAX); printf("]\n");
//     check("UINT_MAX", f, s);

//     /* -1 casté en unsigned → UINT_MAX */
//     printf("Sortie : ["); f = ft_printf("%u", (unsigned int)-1);
//     printf("] | [");      s =    printf("%u", (unsigned int)-1); printf("]\n");
//     check("(unsigned)-1", f, s);

//     /* ------------------------------------------------------------------ */
//     /*  %x / %X — hexadécimal                                               */
//     /* ------------------------------------------------------------------ */
//     section("TEST HEXA (%x / %X)");

//     printf("Sortie : ["); f = ft_printf("%x", 0);
//     printf("] | [");      s =    printf("%x", 0); printf("]\n");
//     check("Zéro minuscule", f, s);

//     printf("Sortie : ["); f = ft_printf("%X", 0);
//     printf("] | [");      s =    printf("%X", 0); printf("]\n");
//     check("Zéro majuscule", f, s);

//     printf("Sortie : ["); f = ft_printf("%x", 255);
//     printf("] | [");      s =    printf("%x", 255); printf("]\n");
//     check("255 minuscule (ff)", f, s);

//     printf("Sortie : ["); f = ft_printf("%X", 255);
//     printf("] | [");      s =    printf("%X", 255); printf("]\n");
//     check("255 majuscule (FF)", f, s);

//     printf("Sortie : ["); f = ft_printf("%x", 1024);
//     printf("] | [");      s =    printf("%x", 1024); printf("]\n");
//     check("1024 minuscule", f, s);

//     printf("Sortie : ["); f = ft_printf("%X", 1024);
//     printf("] | [");      s =    printf("%X", 1024); printf("]\n");
//     check("1024 majuscule", f, s);

//     printf("Sortie : ["); f = ft_printf("%x", UINT_MAX);
//     printf("] | [");      s =    printf("%x", UINT_MAX); printf("]\n");
//     check("UINT_MAX minuscule", f, s);

//     printf("Sortie : ["); f = ft_printf("%X", UINT_MAX);
//     printf("] | [");      s =    printf("%X", UINT_MAX); printf("]\n");
//     check("UINT_MAX majuscule", f, s);

//     printf("Sortie : ["); f = ft_printf("%x", INT_MAX);
//     printf("] | [");      s =    printf("%x", INT_MAX); printf("]\n");
//     check("INT_MAX minuscule", f, s);

//     /* ------------------------------------------------------------------ */
//     /*  %p — pointeur                                                       */
//     /* ------------------------------------------------------------------ */
//     section("TEST POINTEUR (%p)");

//     void *ptr = (void *)0x42;
//     printf("Sortie : ["); f = ft_printf("%p", ptr);
//     printf("] | [");      s =    printf("%p", ptr); printf("]\n");
//     check("Adresse 0x42", f, s);

//     void *ptr2 = (void *)&f;
//     printf("Sortie : ["); f = ft_printf("%p", ptr2);
//     printf("] | [");      s =    printf("%p", ptr2); printf("]\n");
//     check("Adresse de variable", f, s);

//     /* NULL pointer */
//     printf("Sortie : ["); f = ft_printf("%p", (void *)NULL);
//     printf("] | [");      s =    printf("%p", (void *)NULL); printf("]\n");
//     check("Pointeur NULL", f, s);

//     /* ------------------------------------------------------------------ */
//     /*  %% — caractère pourcent                                             */
//     /* ------------------------------------------------------------------ */
//     section("TEST POURCENT (%%)");

//     printf("Sortie : ["); f = ft_printf("%%");
//     printf("] | [");      s =    printf("%%"); printf("]\n");
//     check("Double pourcent seul", f, s);

//     printf("Sortie : ["); f = ft_printf("100%%");
//     printf("] | [");      s =    printf("100%%"); printf("]\n");
//     check("100%%", f, s);

//     printf("Sortie : ["); f = ft_printf("%%%%");
//     printf("] | [");      s =    printf("%%%%"); printf("]\n");
//     check("Double double pourcent", f, s);

//     /* ------------------------------------------------------------------ */
//     /*  Chaînes sans format                                                 */
//     /* ------------------------------------------------------------------ */
//     section("TEST CHAÎNE SANS SPECIFIER");

//     printf("Sortie : ["); f = ft_printf("bonjour");
//     printf("] | [");      s =    printf("bonjour"); printf("]\n");
//     check("Texte sans format", f, s);

//     printf("Sortie : ["); f = ft_printf("%s", "");
//     printf("] | [");      s =    printf("%s", ""); printf("]\n");
//     check("Format vide", f, s);

//     /* ------------------------------------------------------------------ */
//     /*  Combinaisons / formats multiples                                    */
//     /* ------------------------------------------------------------------ */
//     section("COMBINAISONS DE SPECIFIERS");

//     printf("Sortie : ["); f = ft_printf("%d %d %d", 1, 2, 3);
//     printf("] | [");      s =    printf("%d %d %d", 1, 2, 3); printf("]\n");
//     check("Trois entiers", f, s);

//     printf("Sortie : ["); f = ft_printf("%s = %d", "answer", 42);
//     printf("] | [");      s =    printf("%s = %d", "answer", 42); printf("]\n");
//     check("String + entier", f, s);

//     printf("Sortie : ["); f = ft_printf("%c%c%c", 'a', 'b', 'c');
//     printf("] | [");      s =    printf("%c%c%c", 'a', 'b', 'c'); printf("]\n");
//     check("Trois chars", f, s);

//     printf("Sortie : ["); f = ft_printf("hex: %x | HEX: %X | dec: %d", 255, 255, 255);
//     printf("] | [");      s =    printf("hex: %x | HEX: %X | dec: %d", 255, 255, 255); printf("]\n");
//     check("Hexa+dec mélangés", f, s);

//     printf("Sortie : ["); f = ft_printf("%d %i %u %x %X %c %s %%",
//                                         42, -1, UINT_MAX, 0xff, 0xFF, 'Z', "ok");
//     printf("] | [");      s =    printf("%d %i %u %x %X %c %s %%",
//                                         42, -1, UINT_MAX, 0xff, 0xFF, 'Z', "ok"); printf("]\n");
//     check("Tous les specifiers", f, s);

//     /* ------------------------------------------------------------------ */
//     /*  Valeur de retour uniquement (pas d'affichage capturé ici)           */
//     /* ------------------------------------------------------------------ */
//     section("VALEUR DE RETOUR (ret count)");

//     f = ft_printf("%s", ""); s = printf("%s", "");
//     check("Retour string vide", f, s);

//     f = ft_printf("%d", INT_MIN); s = printf("%d", INT_MIN); printf("\n");
//     check("Retour INT_MIN", f, s);

//     f = ft_printf("%s", "abcde"); s = printf("%s", "abcde"); printf("\n");
//     check("Retour string 5 chars", f, s);

//     f = ft_printf("%c", 'X'); s = printf("%c", 'X'); printf("\n");
//     check("Retour char", f, s);

//     f = ft_printf("%%"); s = printf("%%"); printf("\n");
//     check("Retour %%", f, s);

//     /* ------------------------------------------------------------------ */
//     /*  Bilan                                                               */
//     /* ------------------------------------------------------------------ */
//     printf("\n" BOLD YELLOW "════════════════════════════════════════════════════" RESET "\n");
//     printf(BOLD "  BILAN : " RESET);
//     if (fail == 0)
//         printf(GREEN "%d/%d Tests réussis ! 🎉 (Parfait)\n" RESET, pass, pass + fail);
//     else
//         printf(RED "%d échec(s)" RESET " sur " BOLD "%d" RESET " tests.\n", fail, pass + fail);
//     printf(BOLD YELLOW "════════════════════════════════════════════════════" RESET "\n\n");

//     return (fail > 0);
// }