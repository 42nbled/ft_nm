#include "ft_nm.h"


static void	main_32(int fd, t_flags flags) {
	t_fileinfo32	file_header;
	t_lst			name_tables = {.next=NULL, .data=NULL};

	read(fd, &file_header, sizeof(t_fileinfo32));
	for (int i=0; i < file_header.e_shnum; i++)
		run_32(fd, file_header, &name_tables);

	run_32(fd, file_header, &name_tables);
	close(fd);
	print_nm32(name_tables, flags);
}

static void	main_64(int fd, t_flags flags, char *executable_name) {
	t_fileinfo64	file_header;
	t_lst			name_tables = {.next=NULL, .data=NULL};

	read(fd, &file_header, sizeof(t_fileinfo64));
	for (int i=0; i < file_header.e_shnum; i++)
		run_64(fd, file_header, &name_tables);

	run_64(fd, file_header, &name_tables);
	close(fd);
	print_nm64(name_tables, flags, executable_name);
}

int main(int argc, char **argv) {
	int				fd;
	t_eident		identifiers;
	t_flags			flags = {0};
	t_files			*files;
	t_files			*tmp;

	if (argc == 1) {
		printf("Usage: %s [-agurp] [file]\n", argv[0]);
		return 1;
	}
	files = arg_check(&flags, argc, argv);
	if (!files)
		return 1;
	int nfiles = 0;
	for (t_files *tmp = files; tmp; tmp = tmp->next)
		nfiles++;
	while (files) {
		if (nfiles > 1)
			printf("\n%s:\n", files->name);
		if ((fd = open(files->name, O_RDONLY)) < 0) {
			fprintf(stderr, "Error while opening the file \'%s\'.", files->name);
			return (1);
		}
		read(fd, &identifiers, sizeof(identifiers));
		if (identifiers.EI_CLASS == 1)
			main_32(fd, flags);
		else
			main_64(fd, flags, files->name);
		tmp = files;
		files = files->next;
		free(tmp);
	}
	return (0);
}
