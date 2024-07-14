#include "init_file.h"
#include "read_unit.h"


void	init_secinfo32(int fd, t_secinfo32 *info,
		int (*dword)(int))
{
	info->sh_name		= dword(fd);
	info->sh_type		= dword(fd);
	info->sh_flags		= dword(fd);
	info->sh_addr		= dword(fd);
	info->sh_offset		= dword(fd);
	info->sh_size		= dword(fd);
	info->sh_link		= dword(fd);
	info->sh_info		= dword(fd);
	info->sh_addralign	= dword(fd);
	info->sh_entsize	= dword(fd);
}

void	init_secinfo64(int fd, t_secinfo64 *info,
		int (*dword)(int), long (*qword)(int))
{
	info->sh_name		= dword(fd);
	info->sh_type		= dword(fd);
	info->sh_flags		= qword(fd);
	info->sh_addr		= qword(fd);
	info->sh_offset		= qword(fd);
	info->sh_size		= qword(fd);
	info->sh_link		= dword(fd);
	info->sh_info		= dword(fd);
	info->sh_addralign	= qword(fd);
	info->sh_entsize	= qword(fd);
}

void	init_proginfo32(int fd, t_proginfo32 *info,
		int (*dword)(int))
{
	info->p_type		= dword(fd);
	info->p_offset		= dword(fd);
	info->p_vaddr		= dword(fd);
	info->p_paddr		= dword(fd);
	info->p_filesz		= dword(fd);
	info->p_memsz		= dword(fd);
	info->p_flags		= dword(fd);
	info->p_align		= dword(fd);
}

void	init_proginfo64(int fd, t_proginfo64 *info,
		int (*dword)(int), long (*qword)(int))
{
	info->p_type		= dword(fd);
	info->p_offset		= dword(fd);
	info->p_vaddr		= qword(fd);
	info->p_paddr		= qword(fd);
	info->p_filesz		= qword(fd);
	info->p_memsz		= qword(fd);
	info->p_flags		= qword(fd);
	info->p_align		= qword(fd);
}

void	init_fileinfo32(int fd, t_fileinfo32 *info,
		short (*word)(int), int (*dword)(int))
{
	info->e_type		= word(fd);
	info->e_machine		= word(fd);
	info->e_version		= dword(fd);
	info->e_entry		= dword(fd);
	info->e_phoff		= dword(fd);
	info->e_shoff		= dword(fd);
	info->e_flags		= dword(fd);
	info->e_ehsize		= word(fd);
	info->e_phentsize	= word(fd);
	info->e_phnum		= word(fd);
	info->e_shentsize	= word(fd);
	info->e_shnum		= word(fd);
	info->e_shstrndx	= word(fd);
}

void	init_fileinfo64(int fd, t_fileinfo64 *info,
		short (*word)(int), int (*dword)(int), long (*qword)(int))
{
	info->e_type		= word(fd);
	info->e_machine		= word(fd);
	info->e_version		= dword(fd);
	info->e_entry		= qword(fd);
	info->e_phoff		= qword(fd);
	info->e_shoff		= qword(fd);
	info->e_flags		= dword(fd);
	info->e_ehsize		= word(fd);
	info->e_phentsize	= word(fd);
	info->e_phnum		= word(fd);
	info->e_shentsize	= word(fd);
	info->e_shnum		= word(fd);
	info->e_shstrndx	= word(fd);
}

void	init_filestart(int fd, t_eident *e_ident)
{
	e_ident->EI_MAG0		= read_byte(fd);
	e_ident->EI_MAG1		= read_byte(fd);
	e_ident->EI_MAG2		= read_byte(fd);
	e_ident->EI_MAG3		= read_byte(fd);
	e_ident->EI_CLASS		= read_byte(fd);
	e_ident->EI_DATA		= read_byte(fd);
	e_ident->EI_VERSION		= read_byte(fd);
	e_ident->EI_OSABI		= read_byte(fd);
	e_ident->EI_ABIVERSION	= read_byte(fd);

	for (int i=0; i<7; i++) read_byte(fd);
}
