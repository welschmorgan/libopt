/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwelsch <mwelsch@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 17:14:43 by mwelsch           #+#    #+#             */
/*   Updated: 2016/05/05 18:19:50 by mwelsch          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libopt.h"
#include "libft.h"
#include <stdlib.h>
#include <stdarg.h>

static t_app		*g_libopt_app = NULL;

t_app				*init_app(char const *name,
							  char const *company,
							  char const *version,
							  char const *synopsis,
							  char const *executable)
{
	if (g_libopt_app)
		return (g_libopt_app);
	int				len;
	g_libopt_app = (t_app*)malloc(sizeof(t_app));

	len = name ? ft_strlen(name) : 0;
	g_libopt_app->name = ft_strndup(name, len);

	len = company ? ft_strlen(company) : 0;
	g_libopt_app->company = ft_strndup(company, len);

	len = version ? ft_strlen(version) : 0;
	g_libopt_app->version = ft_strndup(version, len);

	len = synopsis ? ft_strlen(synopsis) : 0;
	g_libopt_app->synopsis = ft_strndup(synopsis, len);

	len = executable ? ft_strlen(executable) : 0;
	g_libopt_app->executable = ft_strndup(executable, len);

	g_libopt_app->options = ft_dlist_new();
	g_libopt_app->params = ft_dlist_new();
	return (g_libopt_app);
}

int					parse_option_value(t_opt *opt, char const *arg)
{
	char const		*mid;
	char			**toks;
	int				n;

	if (!arg || !opt)
		return (EXIT_FAILURE);
	if (!ft_strnstr("--", arg, 2) && !ft_strnstr("-", arg, 1))
		return (EXIT_FAILURE);
	while (*arg == '-')
		arg++;
	mid = ft_strstr(arg, "=");
	if (opt->value.flags == OVF_NONE && mid)
	{
		ft_putstr("value given for option ");
		ft_putstr(opt->names[0] ? opt->names[0] : "undefined");
		ft_putendl(", but takes none!");
		return (EXIT_FAILURE);
	}
	mid = mid ? mid + 1 : mid;
	if (opt->value.flags & OVF_LIST)
	{
		toks = ft_strsplit(mid, ',');
		if (!toks)
			return (EXIT_FAILURE);
		n = 0;
		while (toks && toks[n])
			n++;
		init_option_value(&opt->value, OVF_LIST, n, toks);
	}
	else if (opt->value.flags & OVF_STRING)
		init_option_value(&opt->value, OVF_STRING, mid);
	else if (opt->value.flags & OVF_NUMBER)
		init_option_value(&opt->value, OVF_NUMBER, ft_atoi(mid));
	else if (opt->value.flags != OVF_NONE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int					error_app(int code, char *msg)
{
	ft_putstr("error: ");
	ft_putendl(msg);
	return (code);
}

int					raise_app(char const *arg)
{
	t_opt			*opt;
	char const		*pack;
	char			trigger[3];

	if (!arg)
		return (EXIT_FAILURE);
	ft_strncpy(trigger, "-v", 3);
	opt = get_app_option(arg);
	if (!opt)
	{
		pack = arg;
		while (*pack && *pack == '-')
			pack++;
		while (*pack)
		{
			trigger[1] = *pack;
			opt = get_app_option(trigger);
			if (!opt)
				return (EXIT_FAILURE);
			if (!parse_option_value(opt, arg))
				return (EXIT_FAILURE);
			if (trigger_option(opt))
				return (EXIT_FAILURE);
			pack++;
		}
	}
	else
	{
		if (!parse_option_value(opt, arg))
			return (EXIT_FAILURE);
		if (trigger_option(opt))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int					config_app(int argc, char *argv[])
{
	int				i;

	i = 0;
	while (i < argc && argv[i])
	{
		if (ft_strnstr(argv[i], "--", 2)
			|| ft_strnstr(argv[i], "-", 1))
		{
			raise_app(argv[i]);
		}
		else
		{
			ft_dlist_push_back(g_libopt_app->params,
							   ft_dnode_new(ft_strdup(argv[i]),
											ft_strlen(argv[i]),
											NF_DESTROY_ALL));
		}
		i++;
	}
	return (EXIT_FAILURE);
}

t_app				*get_app()
{
	return (g_libopt_app);
}

t_opt				*get_app_option(char const *name)
{
	t_dnode			*node;
	t_opt			*opt;
	unsigned		i;
	int				valpos;

	if (!name)
		return (NULL);
	valpos = 0;
	while (name[valpos] && name[valpos] != '=')
		valpos++;
	node = g_libopt_app->options->tail;
	while (node)
	{
		if ((opt = (t_opt*)node->data))
		{
			i = 0;
			while (i < opt->num_names && opt->names[i])
			{
				if (!ft_strncmp(name, opt->names[i], valpos))
					return (opt);
				i++;
			}
		}
		node = node->next;
	}
	return (NULL);
}

int					trigger_option(t_opt *opt)
{
	int				ret;

	if (!opt || !opt->callback)
		return (EXIT_FAILURE);
	if (opt->flags & OF_ONCE && opt->triggered)
	{
		ft_putstr("error: option ");
		ft_putstr(opt->names[0] ? opt->names[0] : "undefined");
		ft_putendl(" can only be used once!");
		return (EXIT_FAILURE);
	}
	ret = opt->callback(opt);
	opt->triggered += 1;
	return (ret);
}

t_opt				*init_option_names(t_opt *opt,
									   unsigned num_names,
									   char const **names)
{
	unsigned		i;

	if (opt && names)
	{
		opt->names = (char**)malloc(sizeof(char*) * (num_names + 1));
		i = 0;
		while (i < num_names && names[i])
		{
			opt->names[i] = ft_strdup(names[i]);
			i++;
		}
		opt->num_names = i;
		opt->names[i] = NULL;
	}
	return (opt);
}


t_opt				*init_option_descs(t_opt *opt,
									   unsigned num_descs,
									   char const **descs)
{
	unsigned		i;

	if (opt && descs)
	{
		opt->descs = (char**)malloc(sizeof(char*) * (num_descs + 1));
		i = 0;
		while (i < num_descs && descs[i])
		{
			opt->descs[i] = ft_strdup(descs[i]);
			i++;
		}
		opt->num_descs = i;
		opt->descs[i] = NULL;
	}
	return (opt);
}

t_opt				*init_option(t_opt *opt,
								 unsigned num_names,
								 char const **names,
								 unsigned num_descs,
								 char const **descs,
								 unsigned flags,
								 t_opt_func callback)
{
	if (!opt)
		return (opt);
	opt->flags = flags;
	opt->names = NULL;
	opt->num_names = 0;
	opt->num_descs = 0;
	opt->descs = NULL;
	opt->triggered = 0;
	opt->callback = callback;
	init_option_names(opt, num_names, names);
	init_option_descs(opt, num_descs, descs);
	init_option_value(&opt->value, OVF_NONE);
	return (opt);
}

static size_t		_app_usage_add(size_t n, char *dest, size_t i, char const *src)
{
	size_t			len;

	if (i >= n)
		return (0);
	len = ft_strlen(src);
	if (i + len > n)
		return (0);
	ft_strncpy(dest + i, src, len);
	return (len);
}

char				*get_app_usage(size_t n, char *buf, int *overflow)
{
	size_t			i;
	t_app			*app;
	t_dnode			*node;
	unsigned		j;
	t_opt			*opt;

	app = get_app();
	if (!app)
	{
		_app_usage_add(n, buf, 0, "No application defined, initialize it first.\n");
		return (buf);
	}
	i = 0;
	*overflow = 0;
	i += _app_usage_add(n, buf, i, app->name);
	i += _app_usage_add(n, buf, i, "\n");
	if (app->synopsis)
	{
		i += _app_usage_add(n, buf, i, "\n");
		i += _app_usage_add(n, buf, i, app->synopsis);
		i += _app_usage_add(n, buf, i, "\n");
		i += _app_usage_add(n, buf, i, "\n");
	}
	i += _app_usage_add(n, buf, i, "usage: ");
	i += _app_usage_add(n, buf, i, app->executable ? app->executable : app->name);
	i += _app_usage_add(n, buf, i, " <options> [...]\n\n");
	node = app->options->tail;
	while (node)
	{
		opt = (t_opt*)node->data;
		i += _app_usage_add(n, buf, i, "\t");
		j = 0;
		while (j < opt->num_names && opt->names[j])
		{
			if (j)
				i += _app_usage_add(n, buf, i, ", ");
			i += _app_usage_add(n, buf, i, opt->names[j]);
			j++;
		}
		i += _app_usage_add(n, buf, i, " :\n");
		j = 0;
		while (j < opt->num_descs && opt->descs[j])
		{
			i += _app_usage_add(n, buf, i, "\t\t");
			i += _app_usage_add(n, buf, i, opt->descs[j]);
			i += _app_usage_add(n, buf, i, "\n");
			j++;
		}
		i += _app_usage_add(n, buf, i, "\n");
		node = node->next;
	}
	*overflow = i >= n;
	return (buf);
}

void				print_app_usage()
{
	int				overflow;
	static char		buffer[256] = { 0 };

	overflow = 1;
	while (overflow)
	{
		get_app_usage(256, buffer, &overflow);
		ft_putstr(buffer);
	}
}

int					init_option_value(t_opt_val *val,
									  unsigned flags,
									  ...)
{
	char		*sdata;
	long double	ndata;
	va_list		args;

	va_start(args, flags);
	if (!val)
		return (EXIT_FAILURE);
	val->flags = flags;
	if (flags == OVF_NONE)
		val->content.number = 0;
	else if (flags & OVF_LIST)
	{
		ft_dlist_init(val->content.list);
		ft_dlist_init(val->content.list_sel);
		if (flags & OVF_STRING)
		{
			while ((sdata = va_arg(args, char*)))
			{
				ft_dlist_push_back(val->content.list,
								   ft_dnode_new(ft_strdup(sdata),
												ft_strlen(sdata),
												NF_DESTROY_ALL));
			}
		}
		else if (flags & OVF_NUMBER)
		{
			while ((ndata = va_arg(args, long double)))
			{
				ft_dlist_push_back(val->content.list,
								   ft_dnode_new(ft_memdup((void*)&ndata, sizeof(ndata)),
												sizeof(long double),
												NF_DESTROY_ALL));
			}
		}
	}
	else if (flags & OVF_STRING)
	{
		sdata = (char*)va_arg(args, char*);
		val->content.string = ft_strdup(sdata);
	}
	else if (flags & OVF_NUMBER)
	{
		ndata = (long double)va_arg(args, long double);
		val->content.number = ndata;
	}
	va_end(args);
	return (EXIT_SUCCESS);
}


int					decl_app_option(unsigned num_names,
									const char **names,
									unsigned num_descs,
									const char **descs,
									t_opt_val value,
									unsigned flags,
									t_opt_func callback)
{
	t_opt			*opt;

	unsigned		i;

	i = 0;
	while (i < num_names)
	{
		if ((opt = get_app_option(names[i])))
			return (EXIT_FAILURE);
		i++;
	}
	opt = (t_opt*)malloc(sizeof(t_opt));
	if (!opt)
		return (EXIT_FAILURE);
	init_option(opt, num_names, names, num_descs, descs, flags, callback);
	opt->value = value;
	ft_dlist_push_back(g_libopt_app->options,
					   ft_dnode_new(opt,
									sizeof(t_opt),
									NF_DESTROY_ALL));
	return (EXIT_SUCCESS);
}

int					reset_option(t_opt *opt)
{
	if (!opt)
		return (EXIT_FAILURE);
	unsigned		i;

	reset_option_value(&opt->value);
	i = 0;
	while (i < opt->num_names)
	{
		ft_strdel(&opt->names[i]);
		i++;
	}
	i = 0;
	while (i < opt->num_descs)
	{
		ft_strdel(&opt->descs[i]);
		i++;
	}
	ft_memdel((void**)&opt->names);
	opt->triggered = 0;
	return (EXIT_SUCCESS);
}



int					reset_option_value(t_opt_val *val)
{
	if (!val)
		return (EXIT_FAILURE);
	if (val->flags & OVF_LIST)
	{
		ft_dlist_destroy(&val->content.list, ft_dlist_deleter);
		ft_dlist_destroy(&val->content.list_sel, ft_dlist_deleter);
	}
	else if (val->flags & OVF_STRING)
	{
		ft_strdel(&val->content.string);
	}
	return (EXIT_SUCCESS);
}


int					reset_app()
{
	if (!g_libopt_app)
		return (EXIT_FAILURE);
	t_dnode			*node;

	node = g_libopt_app->options->tail;
	while (node)
	{
		reset_option((t_opt*)node->data);
		node = node->next;
	}
	ft_dlist_destroy(&g_libopt_app->options, ft_dlist_deleter);
	ft_dlist_destroy(&g_libopt_app->params, ft_dlist_deleter);
	ft_strdel(&g_libopt_app->name);
	ft_strdel(&g_libopt_app->company);
	ft_strdel(&g_libopt_app->version);
	ft_strdel(&g_libopt_app->executable);
	ft_strdel(&g_libopt_app->synopsis);
	ft_memdel((void**)&g_libopt_app);
	return (EXIT_SUCCESS);
}
