/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: srajaoui <srajaoui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 01:09:12 by srajaoui          #+#    #+#             */
/*   Updated: 2023/08/12 22:13:16 by srajaoui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_header.h"

void	go_sleep(t_struct *info, t_perso *perso)
{
	print(get_time(), perso->id, "is sleeping", info);
	usleep(info->time_to_sleep * 1000);
	print(get_time(), perso->id, "is thinking", info);
}

void	come_eat(t_struct *info, t_perso *perso)
{
	print(get_time(), perso->id, "is eating", info);
	perso->last_meal = get_time() - info->time_start;
	usleep(info->time_to_eat * 1000);
}

void	take_fork(t_struct *info, t_perso *perso)
{
	pthread_mutex_lock(&info->fork[perso->id - 1]);
	print(get_time(), perso->id, "has taken a fork", info);
	if (perso->id == info->number_of_philosopher)
		pthread_mutex_lock(&info->fork[0]);
	else
		pthread_mutex_lock(&info->fork[perso->id]);
	print(get_time(), perso->id, "has taken a fork", info);
	come_eat(info, perso);
	pthread_mutex_unlock(&info->fork[perso->id - 1]);
	if (perso->id == info->number_of_philosopher)
		pthread_mutex_unlock(&info->fork[0]);
	else
		pthread_mutex_unlock(&info->fork[perso->id]);
}

void	*routine(void *base)
{
	t_struct *info;
	t_perso perso;
	
	info = (t_struct *)base;
	perso_init(&perso, info);
	pthread_mutex_init(&(perso.run), NULL);
	pthread_mutex_init(&(perso.write), NULL);
	pthread_create(&(perso.philo), NULL, is_dead, &perso);
	while(perso.how_much_eat !=  0)
	{
		printf("%d   %d\n", perso.last_meal, perso.id);
		take_fork(info, &perso);
		perso.how_much_eat -= 1;
		go_sleep(info, &perso);	
	}
	pthread_join(perso.philo, NULL);
	pthread_mutex_destroy(&(perso.write));
	pthread_mutex_destroy(&(perso.run));
	return NULL;
}
