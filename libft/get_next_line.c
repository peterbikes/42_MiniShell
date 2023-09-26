/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heda-sil <heda-sil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 19:18:28 by heda-sil          #+#    #+#             */
/*   Updated: 2023/07/13 08:36:20 by psotto-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static char	*ft_read_file(int fd, char *r_line)
{
	char	*buff;
	char	*new_r_line;
	int		r;

	buff = malloc((BUFFER_SIZE + 1) * sizeof(*buff));
	if (!buff)
		return (NULL);
	r = 1;
	while (r != 0 && !ft_strchr(r_line, '\n'))
	{
		r = read(fd, buff, BUFFER_SIZE);
		if (r == -1)
		{
			free(buff);
			free(r_line);
			return (NULL);
		}
		buff[r] = 0;
		new_r_line = ft_strjoin(r_line, buff);
		free(r_line);
		r_line = new_r_line;
	}
	free(buff);
	return (r_line);
}

static char	*ft_get_line(char *r_line)
{
	char	*line;
	int		i;

	i = 0;
	if (!r_line[i])
		return (NULL);
	while (r_line[i] && r_line[i] != '\n')
		i++;
	line = ft_calloc(i + 2, sizeof(*line));
	i = 0;
	while (r_line[i] && r_line[i] != '\n')
	{
		line[i] = r_line[i];
		i++;
	}
	if (r_line[i] == '\n')
		line[i] = r_line[i];
	return (line);
}

static char	*ft_get_new_line(char *r_line)
{
	char	*new_r_line;
	int		i;
	int		j;

	i = 0;
	while (r_line[i] && r_line[i] != '\n')
		i++;
	if (r_line[i] == '\n')
		i++;
	if (!r_line[i])
	{
		free(r_line);
		return (NULL);
	}
	new_r_line = ft_calloc((ft_strlen(r_line) - i + 1), sizeof(*new_r_line));
	j = 0;
	while (r_line[i + j])
	{
		new_r_line[j] = r_line[i + j];
		j++;
	}
	free(r_line);
	return (new_r_line);
}

char	*get_next_line(int fd)
{
	static char	*r_line[4096];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!r_line[fd])
		r_line[fd] = ft_calloc(1, sizeof(char));
	r_line[fd] = ft_read_file(fd, r_line[fd]);
	if (!r_line[fd])
		return (NULL);
	line = ft_get_line(r_line[fd]);
	r_line[fd] = ft_get_new_line(r_line[fd]);
	return (line);
}

/*#include <stdio.h>
#include <fcntl.h>

int	main(void)
{
	int		fd1;
	char	*line1;
	int		fd2;
	char	*line2;
	int		fd3;
	char	*line3;
	int		i;

	fd1 = open("file1", O_RDONLY);
	close(fd1);
	fd2 = open("file2", O_RDONLY);
	fd3 = open("file3", O_RDONLY);
	if (fd1 == -1)
	{
		printf("File1 failed to open\n");
		return (0);
	}
	if (fd2 == -1)
	{
		printf("File2 failed to open\n");
		return (0);
	}
	if (fd3 == -1)
	{
		printf("File3 failed to open\n");
		return (0);
	}
	i = 1;
	while (fd1)
	{
		line1 = get_next_line(fd1);
		if (!line1)
			break ;
		printf("              <<<<<<<<<<<<File1 line %i>>>>>>>>>>>> \n", i++);
		printf("%s", line1);
		free(line1);
	}
	close(fd1);
	i = 1;
	while (fd2)
	{
		line2 = get_next_line(fd2);
		if (!line2)
			break ;
		printf("              <<<<<<<<<<<<File2 line %i>>>>>>>>>>>> \n", i++);
		printf("%s", line2);
		free(line2);
	}
	close(fd2);
	i = 1;
	while (fd3)
	{
		line3 = get_next_line(fd3);
		if (!line3)
			break ;
		printf("              <<<<<<<<<<<<File3 line %i>>>>>>>>>>>> \n", i++);
		printf("%s", line3);
		free(line3);
	}
	close(fd3);
	return (0);
}*/
