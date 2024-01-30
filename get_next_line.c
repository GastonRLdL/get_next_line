/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasroman <gasroman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 10:41:11 by gasroman          #+#    #+#             */
/*   Updated: 2024/01/30 18:51:52 by gasroman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "get_next_line.h"

static char	*next_buffer(char *buf)
{
	char	*new_buffer;
	int		i;
	int		j;

	i = 0;
	while (buf[i] && buf[i] != '\n')
		i++;
	if (!buf[i])
		return (double_free(&buf, NULL));
	new_buffer = ft_calloc(ft_strlen(buf) - i, sizeof(char));
	if (!new_buffer)
		return (double_free(&buf, NULL));
	i++;
	j = 0;
	while (buf[i])
		new_buffer[j++] = buf[i++];
	double_free(&buf, NULL);
	return (new_buffer);
}

static char	*ft_get_line(char *buf, char *line)
{
	int	i;

	i = 0;
	if (!buf[i])
		return (NULL);
	while (buf[i] && buf[i] != '\n')
		i++;
	i = i + (buf[i] == '\n');
	line = ft_calloc(i + 1, sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buf[i] && buf[i] != '\n')
	{
		line[i] = buf[i];
		i++;
	}
	if (buf[i] == '\n')
		line[i] = '\n';
	return (line);
}

static char	*ft_join(char *str, char *buf)
{
	char	*tmp;

	tmp = ft_strjoin(buf, str);
	if (!tmp)
		return (double_free(&str, &buf));
	double_free(&buf, NULL);
	return (tmp);
}

static char	*fd_reader(int fd, char *buf)
{
	char	*tmp;
	int		bytes;

	bytes = 1;
	tmp = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!tmp)
		return (double_free(&buf, NULL));
	while (bytes > 0)
	{
		bytes = read(fd, tmp, BUFFER_SIZE);
		if (bytes < 0)
			return (double_free(&tmp, &buf));
		tmp[bytes] = '\0';
		buf = ft_join(tmp, buf);
		if (!buf)
			return (NULL);
		if (ft_strchr(tmp, '\n'))
			break ;
	}
	double_free(&tmp, NULL);
	return (buf);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*buf = NULL;

	line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buf)
		buf = ft_calloc(1, 1);
	if (!buf)
		return (double_free(&buf, NULL));
	buf = fd_reader(fd, buf);
	if (!buf)
		return (NULL);
	line = ft_get_line(buf, line);
	if (!line || *line == '\0')
	{
		printf("%p\n", buf);
		return (double_free(&line, &buf));
	}
	buf = next_buffer(buf);
	return (line);
}

// int main()
// {
// 	int i = 0;
// 	int	fd = open("multiple_nl.txt", O_RDONLY);

// 	char *line;

// 	line = "";
// 	while (line)
// 	{
// 		line = get_next_line(fd);
// 		printf("line[%d]----->%s", i++, line);
// 		free(line);
// 	}
// 	close(fd);
// 	return(0);
// }