#ifndef INIT_FILE_H
# define INIT_FILE_H

#include "ft_nm.h"

void	init_secinfo32(int fd, t_secinfo32 *info,
		int (*dword)(int));

void	init_secinfo64(int fd, t_secinfo64 *info,
		int (*dword)(int), long (*qword)(int));

void	init_proginfo32(int fd, t_proginfo32 *info,
		int (*dword)(int));

void	init_proginfo64(int fd, t_proginfo64 *info,
		int (*dword)(int), long (*qword)(int));

void	init_fileinfo32(int fd, t_fileinfo32 *info,
		short (*word)(int), int (*dword)(int));

void	init_fileinfo64(int fd, t_fileinfo64 *info,
		short (*word)(int), int (*dword)(int), long (*qword)(int));

void	init_filestart(int fd, t_eident *e_ident);

#endif
