/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgautier <mgautier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 10:36:07 by mgautier          #+#    #+#             */
/*   Updated: 2017/01/03 16:11:34 by mgautier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "get_next_line.h"

/*
** ft_read_file
**
** This function take an incomplete line, read from the file descriptor, and
** appends which was read to the end on the incomplete line, by joining it with
** the buffer and replacing it by mean of a pointer.
** It does so until it find a line delimiter or the end of the file.
*/

int				ft_read_file(char **line_to_complete, int fd)
{
	char	buf[BUF_SIZE + 1];
	char	*completed_line;
	t_bool	is_complete;
	int		oct_read;

	is_complete = FALSE;
	while (is_complete == FALSE)
	{
		oct_read = read(fd, &buf, BUF_SIZE);
		if (oct_read == READ_ERROR)
			return (READ_ERROR);
		buf[oct_read] = '\0';
		completed_line = f_strljoin(*line_to_complete, buf,
				ft_strlen(*line_to_complete) + oct_read);
		if (completed_line == NULL)
			return (READ_ERROR);
		ft_strdel(line_to_complete);
		*line_to_complete = completed_line;
		if (ft_strchr(buf, LINE_DELIMITER) != NULL || oct_read != BUF_SIZE)
			is_complete = TRUE;
	}
	return (oct_read);
}

/*
** ft_read_cache
**
** This function reads a file cache, and put the next line of it in str ;
** if necessary, it reads from the associated file, using ft_read_file.
*/

int				ft_read_cache(t_file_cache *file, char **line)
{
	int		read_result;
	char	*bufferized_line;

	bufferized_line = (char*)f_lstpop(&file->lines);
	if (file->is_over)
	{
		if (bufferized_line == NULL)
			return (FILE_IS_OVER);
	}
	else if (file->lines == NULL)
	{
		read_result = ft_read_file(&bufferized_line, file->fd);
		file->lines = f_strsplit_lst(bufferized_line, LINE_DELIMITER);
		if (read_result == READ_ERROR || file->lines == NULL)
			return (READ_ERROR);
		else if (read_result != BUF_SIZE)
			file->is_over = TRUE;
	}
	*line = bufferized_line;
	return (ONE_LINE_READ);
}

/*
** ft_create_file_cache
**
** Allocate and initialize the data struct for file cache (which will keep
** a buffer for each file currently read);
*/

t_file_cache	*ft_create_file_cache(const int fd, t_lst *db)
{
	t_file_cache	*file;

	file = (t_file_cache*)malloc(sizeof(t_file_cache));
	if (file != NULL)
	{
		file->fd = fd;
		file->lines = NULL;
		file->is_over = FALSE;
		if (f_lstpush(file, &db) == NULL)
		{
			file->fd = 0;
			free(file);
			file = NULL;
		}
	}
	return (file);
}

int				get_fd(const t_lst *file_cache)
{
	return (((t_file_cache*)file_cache->content)->fd);
}

int				get_next_line(const int fd, char **line)
{
	static t_lst	*search_list = NULL;
	t_file_cache	*file_cache;
	int				reading_result;

	file_cache = (t_file_cache*)f_lstsearch(search_list, fd, &get_fd);
	if (file_cache == NULL)
		file_cache = ft_create_file_cache(fd, search_list);
	if (file_cache == NULL)
		return (READ_ERROR);
	reading_result = ft_read_cache(file_cache, line);
	return (reading_result);
}
