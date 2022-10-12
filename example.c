/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   example.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 22:15:02 by tdameros          #+#    #+#             */
/*   Updated: 2022/10/12 22:16:20 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <ld.h>

int	main(void)
{
	void	*test1;
	void	*test2;

	test1 = malloc(sizeof(int) * 20);
	test2 = malloc(sizeof(char *) * 13);

	show_leaks();
	return (0);
}
