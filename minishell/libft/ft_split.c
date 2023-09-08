/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:19:29 by vfuster-          #+#    #+#             */
/*   Updated: 2023/04/24 15:08:55 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Compte le nombre de mots dans la chaîne de caractères `str`,
 *  en utilisant le caractère de séparation `sep`.
 */
static int	ft_wordscount(char const *str, char sep)
{
	int	i;
	int	count;

    // Vérifie si la chaîne est nulle ou vide, et retourne 0 si
    // c'est le cas
	if (!str || !str[0])
        	return (0);
	i = 1;
	count = 0;
    	// Compte le premier mot s'il ne commence pas par le caractère 
    	// de séparation
    	if (str[0] != sep)
        	count++;
    	// Parcourt la chaîne et compte les mots
    	while (str[i])
    	{
        	// Si le caractère courant n'est pas le caractère de séparation 
		// et que le caractère précédent était le caractère de séparation,
		// alors on a trouvé un nouveau mot
        	if (str[i] != sep && str[i - 1] == sep)
            		count++;
        	i++;
    	}
    	return (count);
}

/**
 * Alloue un tableau de pointeurs de chaînes de caractères 
 * de taille `len + 1`.
 */
static char	**ft_malloc_words(char const *str, char sep)
{
	int		len;
    	char	**tab;

    	// Vérifie si la chaîne est nulle, et retourne NULL 
    	// si c'est le cas
    	if (!str)
        	return (0);
        // Alloue un tableau de pointeurs de chaînes de caractères 
    	// de taille `len + 1`
    	len = ft_wordscount(str, sep);
    	tab = (char **)malloc(sizeof(*tab) * (len + 1));
    	return (tab);
}

/**
 * Compte le nombre de caractères dans le prochain mot à partir
 * de l'indice `i` dans la chaîne de caractères `str`.
  */
static int	ft_next_word_count(char const *str, char sep, int i)
{
	int	count;

    	// Ignore les caractères de séparation au début du mot
    	count = 0;
    	while (str[i] == sep && str[i])
        	i++;
	// Compte les caractères du mot
    	while (str[i] && str[i] != sep)
    	{
        	count++;
        	i++;
    	}
    	return (count);
}

/** Cette fonction libère la mémoire allouée pour le tableau
 *  de pointeurs de chaînes de caractères str_tab. 
 * */
static char **ft_free(char **str_tab, int i)
{
	int j;

	j = 0;
	while (j < i && str_tab[j])
	{
		// on libere la memoire allouee a str_tab[j]
		free(str_tab[j]);
		j++;
	}
	// On libère la mémoire allouée pour chaque chaîne de 
	//caractères pointée par les pointeurs dans le tableau
	// de pointeurs, puis on libère la mémoire allouée pour
	// le tableau de pointeurs lui-même.
	free(str_tab);
	return (0);
}

// Cette fonction prend une chaîne de caractères 'str' et 
// un caractère 'c' et retourne un tableau de pointeurs de chaînes
// de caractères où chaque élément du tableau contient un 
// sous-ensemble de 'str' séparé par le caractère 'c'.
char	**ft_split(char const *str, char c)
{
	// l'index de début du mot actuel dans la chaîne 'str'
	int		word_start;
	// un compteur pour l'itération sur les mots de la chaîne 'str' 
	int		i;
	// un compteur pour l'itération sur les caractères du mot actuel
	int		j;
	// un pointeur vers le tableau à renvoyer 
	char	**tab;

	word_start = 0;
	// initialisé à -1 car i sera incrémenté avant l'accès 
	// aux éléments du tableau 'tab'
	i = -1;
	// alloue dynamiquement la mémoire pour le tableau 'tab'
	tab = ft_malloc_words(str, c);
	if (!tab)
		// retourne NULL si l'allocation de mémoire échoue
		return (0);
	// boucle sur chaque mot de la chaîne 'str'
	while (++i < ft_wordscount(str, c)) 
	{
		j = 0;
		// alloue dynamiquement la mémoire pour le mot actuel
		if (!tab[i])
		tab[i] = (char *)malloc(ft_next_word_count(str, c, word_start) + 1);
			// libère la mémoire allouée précédemment et retourne NULL 
			// si l'allocation de mémoire échoue
			return (ft_free(tab, i));
		// ignore les caractères de séparation avant le début du mot actuel	
		while (str[word_start] && str[word_start] == c) 
			word_start++;
		// copie chaque caractère du mot actuel dans le tableau 'tab'
		while (str[word_start] && str[word_start] != c) 
			tab[i][j++] = str[word_start++];
		// ajoute le caractère nul à la fin du mot actuel
		tab[i][j] = 0; 
	}
	// ajoute le pointeur NULL à la fin du tableau 'tab'
	tab[i] = 0;
	// retourne le tableau 'tab' contenant les mots séparés par le caractère 'c'
	return (tab); 
}
/*#include <stdio.h>
#include <stdlib.h>
#include "libft.h" 
int	main(void)
{
	char	**tab;
	char	*str = "Hello World! This is a test.";
	char	c = ' ';

	tab = ft_split(str, c);
	// vérifie si l'allocation de mémoire a échoué
	if (!tab) 
	{
		printf("Error: Memory allocation failed.\n");
		return (1);
	}
	for (int i = 0; tab[i]; i++)
		printf("%s\n", tab[i]); 
	free(tab);
	return (0);
}*/
