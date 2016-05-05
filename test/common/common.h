/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/30 13:37:45 by mwelsch           #+#    #+#             */
/*   Updated: 2016/04/02 12:16:10 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		COMMON_H
# define	COMMON_H

# include <stdlib.h>
# include <signal.h>
# include <time.h>
# include <string.h>

# include "libft.h"

# define TEST_NAME_SIZE		50
# define TEST_ERROR_SIZE	180
# define TEST_SIGC_NUM		31
# define TEST_DATA_SLOTS	10

# define ASSERT_TEST(TEST, COND, FMT, ...) assert_test(TEST, COND, #COND, FMT, __VA_ARGS__)


struct					s_test;
struct					s_test_suite;
typedef int				(*t_test_fn)(struct s_test *);

typedef struct			s_test
{
	struct s_test_suite	*suite;
	char				name[TEST_NAME_SIZE];
	char				error[TEST_ERROR_SIZE];
	t_test_fn			func;
	int					code;
	int					signal;
	void				*user_data[TEST_DATA_SLOTS];
	int					log_fd;
}						t_test;

int						init_test(t_test *test, struct s_test_suite *suite, char const *name, t_test_fn fn);
int						reset_test(t_test *test);
int						run_test(t_test *test);
t_test					*current_test();
int						error_test(t_test *test, int code, char const *fmt, ...);
int						log_test(t_test *test, char const *fmt, ...);
int						assert_test(t_test *test,
									int cond,
									char const *condstr,
									char const *fmt,
									...);

int						write_pipe(char const *str, size_t const n);
int						read_pipe(char **line, int(*fn)(int, char **));
int						open_pipe();

int						write_pipe_strings(size_t const n, int const nstrs, char const **strs);
int						read_pipe_strings(int const nstrs, char **gots, int *rets, int(*reader)(int, char**));
int						delete_piped_strings(int const nstrs, char **gots, int *rets);
int						test_strings(size_t const n, int const nstrs, char const **strs,
									 int (*reader)(int, char **),
									 int (*match_str)(size_t const n, char const *wanted, char const *got),
									 int (*match_return)(int wanted, int got));
int						validate_piped_strings(size_t const n,
											   int const nstrs,
											   char const **wanted,
											   char const **got,
											   int const *rets,
											   int (*match_str)(size_t const n, char const *wanted, char const *got),
											   int (*match_return)(int wanted, int got));

typedef struct			s_test_suite
{
	t_dlist				tests;
	t_test				*current;
	int					code;
	int					log_fd;
	time_t				start_time;
	time_t				end_time;
	sig_t				signal_handler[TEST_SIGC_NUM + 1];
	void				*user_data[TEST_DATA_SLOTS];
}						t_test_suite;

t_test_suite			*current_test_suite();
int						init_test_suite(t_test_suite *suite, char const *log_name, int argc, char const *argv[]);
t_test					*push_test_suite(t_test_suite *suite, char const *name, t_test_fn func);
int						run_test_suite(t_test_suite *suite);
void					reset_test_suite(t_test_suite *suite);
void					signal_test_suite(int sigc);
void					install_test_suite_sighandler(t_test_suite *test, int sigc, sig_t func);
void					uninstall_test_suite_sighandler(t_test_suite *test, int sigc);

extern int				g_pipe_fd[2];

#endif
