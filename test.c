#include <stdio.h>
#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*sandwich(char *token, char *env_value, int env_len)
{
	char	*res;
	int		len;
	int		i;
	int		flag;

	len = ft_strlen(token) + env_len - 1;
	res = (char *)malloc(sizeof(char) * (len + 1));
	i = -1;
	flag = 0;
	while (++i < len)
	{
		if (flag == 1)
			res[i] = token[i - env_len + 1];
		else if (token[i] == '$' && flag == 0)
		{
			while (*env_value)
				res[i++] = *(env_value++);
			flag = 1;
			--i;
		}
		else
			res[i] = token[i];
	}
	free(token);
	return (res);
}



int main(int argc, char const *argv[], char **envp)
{
	char *token = (char *)malloc(sizeof(char) * 6);
	token[0] = 's';
	token[1] = 's';
	token[2] = '$';
	token[3] = 's';
	token[4] = 's';
	token[5] = 0;
	char *value = "my";
	
	token = sandwich(token, value, ft_strlen(value));
	printf("%s\n", token);
	
	return 0;
}
