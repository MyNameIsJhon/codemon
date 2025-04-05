#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char *ft_strjoin(char const *s1, char const *s2) // alloue et crée une chaine de caractere
{
  size_t len1 = strlen((char *)s1);
  size_t len2 = strlen((char *)s2);
  char *new_str = (char *)malloc(
      (len1 + len2 + 1) *
      sizeof(char)); // Allouer de la mémoire pour la nouvelle chaîne

  if (!s1 || !s2 || !new_str)
    return NULL;
  memcpy((void *)new_str, (void *)s1, len1);
  memcpy((void *)new_str + len1, (void *)s2, len2 + 1);

  return new_str;
}
