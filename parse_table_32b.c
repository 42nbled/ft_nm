#include "ft_nm.h"

t_secinfo32	*get_section_headers_32(int fd, t_fileinfo32 file_header)
{
	t_secinfo32	*lst = malloc(sizeof(t_secinfo32) * file_header.e_shnum);
	if (!lst)
		assert(0 && "TODO: free all and exit");
	lseek(fd, file_header.e_shoff, SEEK_SET);
	for (int i=0; i < file_header.e_shnum; i++)
		read(fd, &lst[i], sizeof(t_secinfo32));
	return (lst);
}

t_Elf32_Sym	*read_symtable_32(int fd, t_secinfo32 symh)
{
	t_Elf32_Sym	*symtable = malloc(symh.sh_size);
	if (!symtable)
		assert(0 && "TODO: free all and exit");
	lseek(fd, symh.sh_offset, SEEK_SET);
	read(fd, symtable, symh.sh_size);
	return (symtable);
}

char	*read_strtable_32(int fd, t_secinfo32 strh)
{
	char	*strtable = malloc(strh.sh_size);
	if (!strtable)
		assert(0 && "TODO: free all and exit");
	lseek(fd, strh.sh_offset, SEEK_SET);
	read(fd, strtable, strh.sh_size);
	return (strtable);
}

void parse_table_32(t_lst *root, t_Elf32_Sym *symbol_table, char *stringtable, size_t size, char *shstrtab, t_secinfo32 *sections)
{
	for (size_t i = 1; i < size; i++)
	{
		t_name_table *t = malloc(sizeof(t_name_table));
		if (!t)
			assert(0 && "TODO: free all and exit");
		t->name = strdup(&stringtable[symbol_table[i].st_name]);
		sprintf(t->value, "%08x", symbol_table[i].st_value);
		t->i_value = symbol_table[i].st_value;

		t->type = get_symbol_type(
				shstrtab + (symbol_table[i].st_shndx > 0 ? sections[symbol_table[i].st_shndx].sh_name : 0),
				ELF32_ST_BIND(symbol_table[i].st_info),
				ELF32_ST_TYPE(symbol_table[i].st_info),
				symbol_table[i].st_value,
				symbol_table[i].st_shndx);
		lst_append(root, t);
	}
}


void run_32(int fd, t_fileinfo32 fileh, t_lst *root)
{
	static char     *names[1024] = {0};
	static size_t   inames = 0;
	static size_t   i = 0;
	static char     *strtab = NULL;
	static t_secinfo32 *sections = NULL;

	if (!sections)
		sections = get_section_headers_32(fd, fileh);

	if (!strtab)
		strtab = read_strtable_32(fd, sections[fileh.e_shstrndx]);

	if (i < (size_t)fileh.e_shnum)
	{
		t_secinfo32 section = sections[i];
		if (section.sh_type == SHT_SYMTAB)
		{
			t_Elf32_Sym *symtable = read_symtable_32(fd, section);
			char        *strtable = read_strtable_32(fd, sections[section.sh_link]);
			if (symtable && strtable)
			{
				parse_table_32(root, symtable, strtable, section.sh_size / sizeof(t_Elf32_Sym), strtab, sections);
				free(symtable);
				if (inames < 1024)
					names[inames++] = strtable;
				else
					free(strtable);
			}
		}
		i++;
	}
	else if (i == (size_t)fileh.e_shnum)
	{
		// Cleanup
		free(strtab);
		free(sections);
		for (size_t j = 0; j < inames; j++)
			free(names[j]);
		strtab = NULL;
		sections = NULL;
		inames = 0;
		i = 0;
	}
}
