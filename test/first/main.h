/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dlist.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/30 14:04:19 by mwelsch           #+#    #+#             */
/*   Updated: 2016/05/05 17:51:22 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		DLIST_H
# define	DLIST_H

#include "libft.h"
#include "libopt.h"
#include "common.h"
#include <stdio.h>

int				test_dummy(t_test *test);

typedef enum	e_test_id
{
	TEST_DUMMY,
	TEST_COUNT
}				t_test_id;

typedef enum	e_app_flag
{
	FLAG_VERBOSE,
	FLAG_HELP,
	FLAG_COUNT
}				t_app_flag;

int				test_opt_register(t_test *test);
int				test_opt_register_flag_v(t_opt *opt);
int				test_opt_register_flag_h(t_opt *_);
int				test_app_lifecycle(t_test *test);

#endif
