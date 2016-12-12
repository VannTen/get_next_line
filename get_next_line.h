/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgautier <mgautier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/06 18:23:03 by mgautier          #+#    #+#             */
/*   Updated: 2016/12/12 13:59:28 by mgautier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

/*
** Constants definitions
*/

# define ONE_LINE_READ 1
# define READ_ERROR -1
# define FILE_IS_OVER 0
# define LINE_DELIMITER '\n'
# define BUF_SIZE 10

typedef struct	s_line
{
	char	*str;
	t_bool	is_complete;
}				t_line;

typedef struct	s_file_cache
{
	t_list	*lines;
	t_bool	file_is_over;
	int		key;
}				t_file_cache;

#endif
