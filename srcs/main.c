#include "ft_nm.h"

static void	main_32(int fd, t_flags flags, char *executable_name) {
	t_fileinfo32	file_header;
	t_lst			name_tables = {.next=NULL, .data=NULL};

	read(fd, &file_header, sizeof(t_fileinfo32));


	struct stat st;
	if (fstat(fd, &st) == -1) {
		fprintf(stderr, "Error stat'ing the file \'%s\'.", executable_name);
	}

	if (file_header.e_shnum * file_header.e_shentsize + file_header.e_shoff > st.st_size ||
		  file_header.e_phnum * file_header.e_phentsize + file_header.e_phoff > st.st_size) {
		fprintf(stdout, "bfd plugin: %s: file too short\n", executable_name);
		fprintf(stderr, "nm: %s: file format not recognized\n", executable_name);
		return ;
	}

	for (int i=0; i < file_header.e_shnum; i++)
		if (run_32(fd, file_header, &name_tables)) {
			close(fd);
			return;
		};

	if (run_32(fd, file_header, &name_tables)) {
		close(fd);
		return;
	};

	close(fd);
	print_nm32(name_tables, flags, executable_name);
}

static void	main_64(int fd, t_flags flags, char *executable_name) {
	t_fileinfo64	file_header;
	t_lst			name_tables = {.next=NULL, .data=NULL};

	read(fd, &file_header, sizeof(t_fileinfo64));


	struct stat st;
	if (fstat(fd, &st) == -1) {
		fprintf(stderr, "Error stat'ing the file \'%s\'.", executable_name);
	}

	if (file_header.e_shnum * file_header.e_shentsize + file_header.e_shoff > st.st_size ||
		  file_header.e_phnum * file_header.e_phentsize + file_header.e_phoff > st.st_size) {
		fprintf(stdout, "bfd plugin: %s: file too short\n", executable_name);
		fprintf(stderr, "nm: %s: file format not recognized\n", executable_name);
		return ;
	}

	for (int i=0; i < file_header.e_shnum; i++)
		if (run_64(fd, file_header, &name_tables)) {
			close(fd);
			return;
		};

	if (run_64(fd, file_header, &name_tables)) {
		close(fd);
		return;
	};

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
			tmp = files;
			files = files->next;
			free(tmp);
			continue;
		}




		read(fd, &identifiers, sizeof(identifiers));
		if (identifiers.EI_MAG0 != 0x7F || identifiers.EI_MAG1 != 0x45 || identifiers.EI_MAG2 != 0x4c || identifiers.EI_MAG3 != 0x46) {
			fprintf(stderr, "%s: File format not recognised.", files->name);
			tmp = files;
			files = files->next;
			free(tmp);
			continue;
		}

		if (identifiers.EI_CLASS == 1)
			main_32(fd, flags, files->name);
		else
			main_64(fd, flags, files->name);
		tmp = files;
		files = files->next;
		free(tmp);
	}
	return (0);
}
