/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgautier <mgautier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 10:36:07 by mgautier          #+#    #+#             */
/*   Updated: 2016/12/06 15:58:30 by mgautier         ###   ########.fr       */
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
** Ft_cache_file
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

t_file_cache	ft_store_cache(const int fd, const t_database db,
		const char *buffer , const t_bool file_is_over);
{
	t_file_cache	file_cache;

	file_cache = (t_file_cache*)ft_memalloc(sizeof(t_file_cache));
	if (file_cache == NULL)
		return (NULL);
	file_cache->lines = ft_strsplit_empty(buffer, '\n');
	file_cache->end_is_line = (last_char(buffer) == '\n');
	file_cache->file_is_over = file_is_over;
	file_cache->key = fd;
	if (db->add(file_cache) == NULL)
	{
		ft_freetab(file_cache->lines);
		free(file_cache);
		file_cache = NULL;
	}
	return (file_cache);
}

