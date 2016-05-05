/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/30 12:17:08 by mwelsch           #+#    #+#             */
/*   Updated: 2016/04/01 13:04:29 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "main.h"
# include <time.h>

static t_test_suite	g_main_test_suite;
t_dlist				g_list;

int		test_dummy(t_test *test)
{
	(void)test;
	int		times = 5;
	t_dnode	*part;
	static unsigned const	ndata = 5;

	log_test(test, "im a dummy!");
	return (error_test(test, 1, "dummy error"));
}

int		init(int argc, char const *argv[])
{
	init_test_suite(&g_main_test_suite, "tests.log", argc, argv);
	push_test_suite(&g_main_test_suite, "dummy test, testing nothing", test_dummy);
}

int		reset(void)
{
	reset_test_suite(&g_main_test_suite);
	return (0);
}

int		run()
{
	return (run_test_suite(&g_main_test_suite));
}

int		main(void)
{
	int		code;

	code = init();
	code += run();
	code += reset();
	return (code);
}
