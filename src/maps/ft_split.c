#include <stddef.h>
#include <stdlib.h>

unsigned char	ft_isin_charset(char c, char *charset)
{
	while (*charset)
	{
		if (c == *charset)
			return (1);
		charset++;
	}
	return (0);
}

size_t	count_words(char *str, char *charset)
{
	size_t	count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (!ft_isin_charset(*str, charset))
		{
			if (!in_word)
			{
				count++;
				in_word = 1;
			}
		}
		else
			in_word = 0;
		str++;
	}
	return (count);
}

char	*split_nextword(char *str, char *charset)
{
	while (*str && !ft_isin_charset(*str, charset))
		str++;
	while (*str && ft_isin_charset(*str, charset))
		str++;
	return (str);
}

char	*ft_splitdup(char *src, char *charset)
{
	size_t	size;
	char	*str;
	size_t	i;

	size = 0;
	i = 0;
	while (src[size] && !ft_isin_charset(src[size], charset))
		size++;
	str = (char *)malloc(size + 1);
	if (!str)
		return (NULL);
	while (i < size)
	{
		str[i] = src[i];
		i++;
	}
	str[size] = '\0';
	return (str);
}

char	**ft_split(char *str, char *charset)
{
	size_t	i;
	size_t	totlen;
	char	**strs;

	totlen = count_words(str, charset);
	strs = (char **)malloc(sizeof(char *) * (totlen + 1));
	i = 0;
	if (!strs)
		return (NULL);
	while (*str && ft_isin_charset(*str, charset))
		str++;
	while (i < totlen)
	{
		strs[i] = ft_splitdup(str, charset);
		str = split_nextword(str, charset);
		i++;
	}
	strs[i] = NULL;
	return (strs);
}
