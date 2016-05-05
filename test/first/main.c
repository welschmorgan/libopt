/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/30 12:17:08 by mwelsch           #+#    #+#             */
/*   Updated: 2016/04/24 12:45:34 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "main.h"
# include <libopt.h>
# include <time.h>

static t_test_suite	g_main_test_suite;
t_dlist				g_list;

int		test_app_lifecycle(t_test *test)
{
	//log_test(test, "im a dummy!");
	init_app("test application", "test inc.", "1.0-dev", "small unit test for libopt", "main.run");
	log_test(test, "app: %s ©%s v%s",
			 get_app()->name,
			 get_app()->company,
			 get_app()->version);
	reset_app();
	if (get_app())
		return (error_test(test, 1,
						   "global application pointer "
						   "persisted after reset"));
	return (EXIT_SUCCESS);
}

int		test_opt_register_flag_h(t_opt *_)
{
	(void)_;
	print_app_usage();
	return (EXIT_SUCCESS);
}

int		test_opt_register_flag_v(t_opt *opt)
{
	(void)opt;
	ft_putendl("Verbose option triggered!");
	return (0);
}

typedef enum	e_app_flag
{
	FLAG_VERBOSE,
	FLAG_HELP,
	FLAG_COUNT
}				t_app_flag;
int		test_opt_register(t_test *test)
{
	t_opt_val	val;
	char		*names[FLAG_COUNT][3];
	char		*helps[FLAG_COUNT][3];
	t_opt		*opt;
	t_app		*app;
	t_dnode		*node;
	t_dnode		*nval;

	names[FLAG_VERBOSE][0] = "-v";
	names[FLAG_VERBOSE][1] = "--verbose";
	names[FLAG_HELP][0] = "-h";
	names[FLAG_HELP][1] = "--help";
	helps[FLAG_HELP][0] = "shows this help screen.";
	helps[FLAG_VERBOSE][0] = "shows additionnal messages.";

	if (!g_main_test_suite.user_data[0])
		log_test(test, "no arguments provided!");
	//log_test(test, "im a dummy!");
	init_app("test application", "test inc.", "1.0-dev", "small unit test for libopt", "main.run");
	app = get_app();
	init_option_value(&val, OVF_NONE);
	decl_app_option(2, (const char **)names[FLAG_VERBOSE],
					1, (const char **)helps[FLAG_VERBOSE],
					val,
					OF_FLAG,
					test_opt_register_flag_v);
	decl_app_option(2, (const char **)names[FLAG_HELP],
					1, (const char **)helps[FLAG_HELP],
					val,
					OF_ONCE | OF_FLAG,
					test_opt_register_flag_h);
	if(config_app(TEST_DATA_SLOTS, (char **)g_main_test_suite.user_data))
		error_test(test, 1, "Failed to configure app!");
	node = app->options->tail;
	while (node)
	{
		opt = (t_opt*)node->data;
		log_test(test, "\toption: '%s', trigger:%d", opt->names && opt->names[0] ? opt->names[0] : "undefined", opt->triggered);
		if (opt->value.flags != OVF_NONE)
		{
			if (opt->value.flags & OVF_LIST)
			{
				nval = opt->value.content.list->tail;
				while (nval && nval->data)
				{
					log_test(test, "\t\tvalue: %s\n", (char*)nval->data);
					nval = nval->next;
				}
			}
			else if (opt->value.flags & OVF_STRING)
				log_test(test, "\t\tvalue: %s\n", opt->value.content.string);
			else if (opt->value.flags & OVF_NUMBER)
				log_test(test, "\t\tvalue: %lld\n", opt->value.content.number);
		}
		node = node->next;
	}
	node = app->params->tail;
	while (node && node->data)
	{
		log_test(test, "\targument: '%s'", (char*)node->data);
		node = node->next;
	}
	reset_app();
	return (0);
}

int		init(int argc, char const *argv[])
{
	init_test_suite(&g_main_test_suite, "tests.log", argc, argv);
	push_test_suite(&g_main_test_suite, "testing t_app lifecycle", test_app_lifecycle);
	push_test_suite(&g_main_test_suite, "testing registering of options", test_opt_register);
	return (0);
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

int		main(int argc, char const *argv[])
{
	int		code;

	code = init(argc, argv);
	code += run();
	code += reset();
	return (code);
}
