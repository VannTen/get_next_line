/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgautier <mgautier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 18:23:03 by mgautier          #+#    #+#             */
/*   Updated: 2017/01/23 18:07:21 by mgautier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "libft.h"

/*
** Constants definitions
*/

# define ONE_LINE_READ 1
# define READ_ERROR -1
# define FILE_IS_OVER 0
# define LINE_DELIMITER '\n'
# define BUF_SIZE 10

/*
** Types definitions
*/

typedef struct	s_file_cache
{
	t_lst	*lines;
	size_t	remain_length;
	int		fd;
	t_bool	is_over;
}				t_file_cache;

int				get_next_line(const int fd, char **line);
#endif
