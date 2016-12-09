/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgautier <mgautier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 10:36:07 by mgautier          #+#    #+#             */
/*   Updated: 2016/12/09 18:49:56 by mgautier         ###   ########.fr       */
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
** ft_cache_file
**
** This function read from a file descriptor (usigne the syscall 'read'
** until is has at least one valid line (depending on BUF_SIZE and the lenght
** of the line in the file, that can take several calls.
** If, on the contrary, multiples lines are read, they are split into a string
** array.
*/

t_file_cache	ft_cache_file(const int fd, const t_database db)
{
	char			buffer[BUF_SIZE + 1];
	int				oct_read;
	char			**lines;

	oct_read = 1;
	buffer[0] = '\0';
	lines = (char**)ft_memalloc(sizeof(char*) * 2);
	lines[0] = NULL;
	lines[1] = NULL;
	while ((buffer[oct_read - 1] != '\n' || oct_read == BUF_SIZE)
			&& lines[1] == NULL)
	{
		ft_tabdel_not_content(lines);
		oct_read = read(fd, &buffer, BUF_SIZE);
		if (oct_read == -1)
			return (NULL);
		buffer[oct_read] = '\0';
		lines = ft_strplit_empty(buffer, '\n');
		lines[0] = ft_strjoin(incomplete_line, lines[0]);
	}
	return (ft_store_cache(fd, db, incomplete_line));
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

	if (file->lines == NULL)
	{
		if (file->is_over)
			return (FILE_IS_OVER);
		else
			file->lines = ft_lstnew_nocpy(NULL);
	}
	while (file->lines->next == NULL && file->is_over == FALSE)
	{
		str = (char*)lines->content;
		free(lines);
		lines = NULL;
		read_result = ft_read_file(&str, file->fd);
		if (read_result == FILE_IS_OVER)
			file->is_over = TRUE;
		else if (read_result == READ_ERROR)
			return (READ_ERROR);
		lines = ft_strplit_lst(str);
	}
	*str = (char*)file->lines->content;
	next_link = file->lines->next;
	free(file->lines);
	file->lines = next_link;
	return (ONE_LINE_READ);
}


/*
** ft_read_file
**
** This function take an incomplete line, read from the file descriptor, and
** appends which was read to the end on the incomplete line, by joining it with
** the buffer and replacing it by mean of a pointer.
*/

int				*ft_read_file(char **line_to_complete, int fd)
{
	char	buf[BUF_SIZE + 1];
	char	*completed_line;

	oct_read = read(fd, &buf, BUF_SIZE);
	if (oct_read == READ_ERROR)
		return (READ_ERROR);
	buf[oct_read] = '\0';
	completed_line = ft_strtjoin(*line_to_complete, buf,
			ft_strlen(*line_to_complete) + oct_read);
	ft_strdel(*line_to_complete);
	*line_to_complete = completed_line;
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
