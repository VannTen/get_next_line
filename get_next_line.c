/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgautier <mgautier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 10:36:07 by mgautier          #+#    #+#             */
/*   Updated: 2016/12/12 12:20:19 by mgautier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int				get_next_line(const int fd, char **line)
{
	static const t_database	database = new(database);
	t_file_cache			*file_cache;
	char					*next_line;
	int						reading_result

		file_cache = (t_file_cache*)database->find(fd);
	if (file_cache == NULL)
		file_cache = ft_cache_file(fd, database);
	if (file_cache == NULL)
		return (ERROR);
	reading_result = read_from_cache(database, file_cache, &next_line);
	*line = ft_strdup(next_line);
	ft_strdel(next_line);
	return (reading_result);
}

/*
** ft_read_cache
**
** This function reads a file cache, and put the next line of it in str ;
** if necessary, it reads from the associated file, using ft_read_file.
*/

int				ft_read_cache(t_file_cache file, char **str)
{
	t_lst	*next_link;
	char	*str;
	int		read_result;

	str = pop(file->lines);
	if (lines != NULL || file->is_over)
		return (ONE_LINE_READ);
}

int			ft_complete_line(t_file_cache file, char **str)
{
	int	read_return;

	read_return == 0;
	while (ft_strchr(*str, LINE_DELIMITER) == NULL)
	{
		read_return = ft_read_file(str, file->fd);
		if (read_return == READ_ERROR)
			break ;
		else if (read_return != BUF_SIZE)
		{
			file->is_over == TRUE;
			break ;
		}
	}
	return (read_return);
}

/*
** ft_read_file
**
** This function take an incomplete line, read from the file descriptor, and
** appends which was read to the end on the incomplete line, by joining it with
** the buffer and replacing it by mean of a pointer.
** It does so until it find a line delimiter or the end of the file.
*/

int				*ft_read_file(char **line_to_complete, int fd)
{
	char	buf[BUF_SIZE + 1];
	char	*completed_line;
	t_bool	is_complete;

	is_complete = FALSE;
	while (is_complete == FALSE)
	{
		oct_read = read(fd, &buf, BUF_SIZE);
		if (oct_read == READ_ERROR)
			return (READ_ERROR);
		buf[oct_read] = '\0';
		completed_line = ft_strtjoin(*line_to_complete, buf,
				ft_strlen(*line_to_complete) + oct_read);
		ft_strdel(*line_to_complete);
		*line_to_complete = completed_line;
		if (ft_strchr(buf, LINE_DELIMITER) != NULL || oct_read != BUF_SIZE)
			is_complete == TRUE;
	}
	return (oct_read);
}

t_file_cache	*ft_create_file_cache(const int fd, const t_database db)
{
	t_file_cache	*file;

	file->fd = fd;
	file->lines = NULL;
	file->is_over = FALSE;
	if (db->add(file) == NULL)
	{
		file->fd = 0;
		free(file);
		file = NULL;
	}
	return (file);
}
