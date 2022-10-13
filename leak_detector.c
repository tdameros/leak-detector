/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leak_detector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 13:41:09 by tdameros          #+#    #+#             */
/*   Updated: 2022/10/13 12:11:24 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct s_list
{
	void			*malloc_ptr;
	char			*file;
	int				line;
	int				size;
	struct s_list	*next;
}	t_list;

static t_list	*g_mem_list = NULL;

void	*free_retnull(void *ptr)
{
	free(ptr);
	return (NULL);
}

int	free_retnegative(void *ptr)
{
	free(ptr);
	return (-1);
}

void	go_to_line(int fd, char *buf, int line)
{
	int	ret;
	int	count;

	count = 1;
	ret = read(fd, buf, 1);
	while (ret && count < line)
	{
		if (buf[0] == '\n')
			count++;
		if (count < line)
			ret = read(fd, buf, 1);
	}
}

int	get_line_size(char *file_name, int line)
{
	char	*buf;
	int		fd;
	int		count;
	int		ret;

	buf = malloc(sizeof(char));
	if (buf == NULL)
		return (-1);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (free_retnegative(buf));
	go_to_line(fd, buf, line);
	count = 0;
	ret = read(fd, buf, 1);
	while (ret && buf[0] != '\n')
	{
		count++;
		ret = read(fd, buf, 1);
	}
	if (close(fd) == -1)
		return (free_retnegative(buf));
	free(buf);
	return (count);
}

char	*get_line(char *file_name, int line)
{
	char	*buf;
	int		fd;
	int		size;

	size = get_line_size(file_name, line);
	if (size <= 0)
		return (NULL);
	buf = malloc(sizeof(char) * (size + 1));
	if (buf == NULL)
		return (NULL);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (free_retnull(buf));
	go_to_line(fd, buf, line);
	read(fd, buf, size);
	buf[size] = '\0';
	if (close(fd) == -1)
		return (free_retnull(buf));
	return (buf);
}

void	list_add_front(t_list **lst, t_list *new)
{
	new->next = *lst;
	*lst = new;
}

t_list	*list_new(void *malloc_ptr, int size, char *file, int line)
{
	t_list	*new_node;

	new_node = malloc(sizeof(t_list));
	if (new_node == NULL)
		return (NULL);
	new_node->malloc_ptr = malloc_ptr;
	new_node->size = size;
	new_node->line = line;
	new_node->file = strdup(file);
	new_node->next = NULL;
	return (new_node);
}

void	list_delone(t_list *node)
{
	free(node->malloc_ptr);
	free(node->file);
	free(node);
}

void	list_clear(t_list **lst)
{
	t_list	*current_node;
	t_list	*next_node;

	current_node = *lst;
	while (current_node != NULL)
	{
		next_node = current_node->next;
		list_delone(current_node);
		current_node = next_node;
	}
	*lst = NULL;
}

void	show_leaks(void)
{
	t_list	*node;
	char	*line;

	node = g_mem_list;
	if (node != NULL)
	{
		printf("=============== LEAKS DETECTED ===============\n");
		while (node != NULL)
		{
			printf("\n%d bytes leaks in %s", node->size, node->file);
			printf(" at line %d\n", node->line);
			line = get_line(node->file, node->line);
			printf("%s\n", line);
			free(line);
			node = node->next;
		}
		list_clear(&g_mem_list);
	}
}

void	ld_free(void *malloc_ptr)
{
	t_list	*current_node;
	t_list	*previous_node;	

	if (g_mem_list == NULL)
		return ;
	previous_node = g_mem_list;
	current_node = previous_node->next;
	if (previous_node->malloc_ptr == malloc_ptr)
	{
		g_mem_list = current_node;
		list_delone(previous_node);
	}
	while (current_node != NULL)
	{
		if (current_node->malloc_ptr == malloc_ptr)
		{
			previous_node->next = current_node->next;
			return (list_delone(current_node));
		}
		previous_node = current_node;
		current_node = current_node->next;
	}
}

void	*ld_malloc(size_t size, char *file, int line)
{
	void	*malloc_ptr;
	t_list	*new_node;
	char	*line_str;

	malloc_ptr = malloc(size);
	if (malloc_ptr == NULL)
	{
		printf("\n=============== MALLOC FAILED ================\n");
		printf("\nMalloc failed in %s at line %d\n", file, line);
		line_str = get_line(file, line);
		printf("%s\n\n", line_str);
		free(line_str);
		return (NULL);
	}
	new_node = list_new(malloc_ptr, size, file, line);
	list_add_front(&g_mem_list, new_node);
	return (malloc_ptr);
}
