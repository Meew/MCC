int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "Russian");
    if (strcmp(argv[1], "-l") || argc != 3) {
        printf("Davidyuk C Compiler\n");
        printf("����� ������� �8303� 2014\n");
        /*printf("�������������: %s [����] [����]\n", argv[0]);
        printf("\n");
    */  printf("  -l\t������ ������������ �����������");
        printf("\n");
        return 0;
    }

    FILE * f = fopen(argv[2], "r");
    fseek(f, 0, SEEK_END);
    int file_size = ftell(f) + 1; // +1 ��� \0
    fseek(f, 0, SEEK_SET);
    char * file = malloc(file_size * sizeof(char));
    fread(file, sizeof(char), file_size - 1, f);
    file[file_size - 1] = 0;
    fclose(f);

    LEXER_Handle lexer = LEXER_Create(file);

    return 0;
}