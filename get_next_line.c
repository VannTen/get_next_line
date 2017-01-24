/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgautier <mgautier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 10:36:07 by mgautier          #+#    #+#             */
/*   Updated: 2017/01/24 14:42:58 by mgautier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

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
	char	buf[BUFF_SIZE + 1];
	char	*completed_line;
	t_bool	is_complete;
	int		oct_read;
	int		length;

	is_complete = FALSE;
	length = (*line_to_complete == NULL) ? 0 : ft_strlen(*line_to_complete);
	while (!is_complete)
	{
		oct_read = read(fd, &buf, BUFF_SIZE);
		if (oct_read == READ_ERROR)
			return (READ_ERROR);
		buf[oct_read] = '\0';
		completed_line = f_strljoin(*line_to_complete, buf, length, oct_read);
		if (completed_line == NULL)
			return (READ_ERROR);
		length += oct_read;
		ft_strdel(line_to_complete);
		*line_to_complete = completed_line;
		is_complete = (ft_memchr(buf, LINE_DELIMITER, oct_read) != NULL
				|| oct_read != BUFF_SIZE);
	}
	return (oct_read);
}

/*
** ft_read_cache
**
** This function reads a file cache, and put the next line of it in line ;
** if necessary, it reads from the associated file, using ft_read_file.
*/

int				ft_read_cache(t_file_cache *file, char **line)
{
	int		read_result;
	char	*bufferized_line;

	bufferized_line = (char*)f_lstpop(&file->lines);
	if (file->lines == NULL)
	{
		read_result = ft_read_file(&bufferized_line, file->fd);
		file->lines = f_strsplit_lst(bufferized_line, LINE_DELIMITER);
		ft_strdel(&bufferized_line);
		if (read_result == READ_ERROR || file->lines == NULL)
			return (READ_ERROR);
		else if (read_result != BUFF_SIZE)
			file->is_over = TRUE;
		bufferized_line = (char*)f_lstpop(&file->lines);
	}
	if (file->is_over)
	{
		if (bufferized_line == NULL
				|| (file->lines == NULL && *bufferized_line == '\0'))
			return (FILE_IS_OVER);
	}
	*line = bufferized_line;
	return (ONE_LINE_READ);
}

void			cache_dtor(void *cache_)
{
	t_file_cache *cache;

	cache = (t_file_cache*)cache_;
	cache->fd = 0;
	f_lstdel(&cache->lines, ft_gen_strdel);
	cache->lines = NULL;
	free(cache);
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

	if (line == NULL)
		return (READ_ERROR);
	file_cache = (t_file_cache*)f_lstsearch(search_list, fd, &get_fd);
	if (file_cache == NULL)
	{
		file_cache = (t_file_cache*)malloc(sizeof(t_file_cache));
		file_cache->fd = fd;
		file_cache->lines = NULL;
		file_cache->is_over = FALSE;
	}
	if (file_cache == NULL)
		return (READ_ERROR);
	if (f_lstpush(file_cache, &search_list) == NULL)
	{
		cache_dtor(file_cache);
		return (READ_ERROR);
	}
	reading_result = ft_read_cache(file_cache, line);
	if (reading_result == FILE_IS_OVER)
		f_lstremoveif_one(&search_list, fd, &get_fd, &cache_dtor);
	return (reading_result);
}
