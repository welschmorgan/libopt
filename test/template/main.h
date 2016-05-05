/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dlist.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/30 14:04:19 by mwelsch           #+#    #+#             */
/*   Updated: 2016/03/31 11:01:54 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		DLIST_H
# define	DLIST_H

#include "libft.h"
#include "common.h"
#include <stdio.h>

int				test_dummy(t_test *test);

typedef enum	e_test_id
{
	TEST_DUMMY,
	TEST_COUNT
}				t_test_id;

#endif
