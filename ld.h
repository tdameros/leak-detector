/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ld.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 22:13:34 by tdameros          #+#    #+#             */
/*   Updated: 2022/10/12 23:57:59 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef LD_H
# define LD_H

void	*ld_malloc(size_t size, char *file, int line);
void	ld_free(void *malloc_ptr);
void	show_leaks(void);

# define malloc(size) ld_malloc(size, __BASE_FILE__, __LINE__)
# define free(ptr) ld_free(ptr)

#endif
