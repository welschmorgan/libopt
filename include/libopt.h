/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libopt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 17:03:29 by mwelsch           #+#    #+#             */
/*   Updated: 2016/04/24 11:58:22 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef		LIBOPT_H
# define	LIBOPT_H

# include <stddef.h>

struct s_dlist;
struct s_dnode;
struct s_opt;

typedef int				(*t_opt_func)(struct s_opt *opt);

typedef enum			e_opt_val_flag
{
	OVF_NONE = 0,
	OVF_STRING = 1 << 1,
	OVF_LIST = 1 << 2,
	OVF_NUMBER = 1 << 3,
	OVF_MULTIPLE = 1 << 4,
	OVF_COUNT = 4
}						t_opt_val_flag;

typedef enum			e_opt_flag
{
	OF_NONE = 0,
	OF_ONCE = 1 << 1,
	OF_FLAG = 1 << 2,
	OF_COUNT = 1
}						t_opt_flag;

typedef union			u_opt_val_content
{
	char				*string;
	struct s_dlist		*list;
	struct s_dlist		*list_sel;
	long long			number;
}						t_opt_val_content;

typedef struct			s_opt_val
{
	unsigned			flags;
	t_opt_val_content	content;
}						t_opt_val;

typedef struct			s_opt
{
	unsigned			num_names;
	char				**names;
	unsigned			num_descs;
	char				**descs;
	unsigned			flags;
	t_opt_val			value;
	t_opt_func			callback;
	int					triggered;
}						t_opt;

typedef struct			t_app
{
	char				*name;
	char				*company;
	char				*version;
	char				*synopsis;
	char				*executable;
	struct s_dlist		*options;
	struct s_dlist		*params;
}						t_app;

t_app				*init_app(char const *name,
							  char const *company,
							  char const *version,
							  char const *synopsis,
							  char const *executable);
int					config_app(int argc, char *argv[]);
t_app				*get_app();
int					reset_app();
void				print_app_usage();
char				*get_app_usage(size_t n,
								   char *buf,
								   int *overflow);
t_opt				*init_option(t_opt *opt,
								 unsigned num_names,
								 char const **names,
								 unsigned num_descs,
								 char const **descs,
								 unsigned flags,
								 t_opt_func callback);
t_opt				*init_option_descs(t_opt *opt,
									   unsigned num_descs,
									   char const **descs);
t_opt				*init_option_names(t_opt *opt,
									   unsigned num_names,
									   char const **names);
int					trigger_option(t_opt *opt);
int					reset_option(t_opt *opt);
t_opt				*get_app_option(char const *name);
int					init_option_value(t_opt_val *val,
									   unsigned flags,
									   ...);
int					reset_option_value(t_opt_val *val);

int					decl_app_option(unsigned num_names,
									const char **names,
									unsigned num_descs,
									const char **descs,
									t_opt_val value,
									unsigned flags,
									t_opt_func callback);

int					error_app(int code, char *msg);

#endif
