#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define MAX_VIDEOS 10

typedef struct {
    int id;
    char titulo[50];
    float duracao;
    char genero[30];
} Video;

typedef struct {
    int id;
    char nome[50];
    char email[50];
    int favoritos[MAX_VIDEOS];
    int qtd_favoritos;
} Usuario;

void cadastrar_video();
void cadastrar_usuario();
void listar_videos();
void listar_usuarios();
void atualizar_video();
void atualizar_usuario();
void remover_video();
void remover_usuario();
void relatorio_usuarios();

int main() {
    int opcao;
    do {
        printf("\n===== MENU STREAMING =====\n");
        printf("1. Cadastrar vídeo\n");
        printf("2. Cadastrar usuário\n");
        printf("3. Listar vídeos\n");
        printf("4. Listar usuários\n");
        printf("5. Atualizar vídeo\n");
        printf("6. Atualizar usuário\n");
        printf("7. Remover vídeo\n");
        printf("8. Remover usuário\n");
        printf("9. Relatório de usuários e vídeos favoritados\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();  // limpar buffer

        switch(opcao) {
            case 1: cadastrar_video(); break;
            case 2: cadastrar_usuario(); break;
            case 3: listar_videos(); break;
            case 4: listar_usuarios(); break;
            case 5: atualizar_video(); break;
            case 6: atualizar_usuario(); break;
            case 7: remover_video(); break;
            case 8: remover_usuario(); break;
            case 9: relatorio_usuarios(); break;
        }
    } while(opcao != 0);

    return 0;
}

void cadastrar_video() {
    FILE *f = fopen("videos.dat", "ab");
    Video v;
    printf("ID: "); scanf("%d", &v.id);
    getchar();
    printf("Título: "); fgets(v.titulo, 50, stdin); strtok(v.titulo, "\n");
    printf("Duração (min): "); scanf("%f", &v.duracao);
    getchar();
    printf("Gênero: "); fgets(v.genero, 30, stdin); strtok(v.genero, "\n");

    fwrite(&v, sizeof(Video), 1, f);
    fclose(f);
    printf("Vídeo cadastrado com sucesso!\n");
}

void cadastrar_usuario() {
    FILE *f = fopen("usuarios.dat", "ab");
    Usuario u;
    printf("ID: "); scanf("%d", &u.id);
    getchar();
    printf("Nome: "); fgets(u.nome, 50, stdin); strtok(u.nome, "\n");
    printf("Email: "); fgets(u.email, 50, stdin); strtok(u.email, "\n");

    printf("Quantos vídeos favoritos? (máx %d): ", MAX_VIDEOS);
    scanf("%d", &u.qtd_favoritos);
    for(int i = 0; i < u.qtd_favoritos; i++) {
        printf("ID do vídeo %d: ", i+1);
        scanf("%d", &u.favoritos[i]);
    }

    fwrite(&u, sizeof(Usuario), 1, f);
    fclose(f);
    printf("Usuário cadastrado com sucesso!\n");
}

void listar_videos() {
    FILE *f = fopen("videos.dat", "rb");
    Video v;
    printf("\n--- Lista de Vídeos ---\n");
    while(fread(&v, sizeof(Video), 1, f)) {
        printf("ID: %d | Título: %s | Duração: %.2f min | Gênero: %s\n", v.id, v.titulo, v.duracao, v.genero);
    }
    fclose(f);
}

void listar_usuarios() {
    FILE *f = fopen("usuarios.dat", "rb");
    Usuario u;
    printf("\n--- Lista de Usuários ---\n");
    while(fread(&u, sizeof(Usuario), 1, f)) {
        printf("ID: %d | Nome: %s | Email: %s | Favoritos: ", u.id, u.nome, u.email);
        for(int i = 0; i < u.qtd_favoritos; i++) {
            printf("%d ", u.favoritos[i]);
        }
        printf("\n");
    }
    fclose(f);
}

void atualizar_video() {
    FILE *f = fopen("videos.dat", "rb+");
    Video v;
    int id, achou = 0;
    printf("ID do vídeo para atualizar: ");
    scanf("%d", &id);
    while(fread(&v, sizeof(Video), 1, f)) {
        if(v.id == id) {
            fseek(f, -sizeof(Video), SEEK_CUR);
            printf("Novo título: "); getchar(); fgets(v.titulo, 50, stdin); strtok(v.titulo, "\n");
            printf("Nova duração: "); scanf("%f", &v.duracao); getchar();
            printf("Novo gênero: "); fgets(v.genero, 30, stdin); strtok(v.genero, "\n");
            fwrite(&v, sizeof(Video), 1, f);
            achou = 1;
            break;
        }
    }
    fclose(f);
    if(achou) printf("Vídeo atualizado!\n");
    else printf("Vídeo não encontrado.\n");
}

void atualizar_usuario() {
    FILE *f = fopen("usuarios.dat", "rb+");
    Usuario u;
    int id, achou = 0;
    printf("ID do usuário para atualizar: ");
    scanf("%d", &id);
    while(fread(&u, sizeof(Usuario), 1, f)) {
        if(u.id == id) {
            fseek(f, -sizeof(Usuario), SEEK_CUR);
            printf("Novo nome: "); getchar(); fgets(u.nome, 50, stdin); strtok(u.nome, "\n");
            printf("Novo email: "); fgets(u.email, 50, stdin); strtok(u.email, "\n");
            printf("Nova quantidade de favoritos: "); scanf("%d", &u.qtd_favoritos);
            for(int i = 0; i < u.qtd_favoritos; i++) {
                printf("ID do vídeo %d: ", i+1);
                scanf("%d", &u.favoritos[i]);
            }
            fwrite(&u, sizeof(Usuario), 1, f);
            achou = 1;
            break;
        }
    }
    fclose(f);
    if(achou) printf("Usuário atualizado!\n");
    else printf("Usuário não encontrado.\n");
}

void remover_video() {
    FILE *orig = fopen("videos.dat", "rb");
    FILE *novo = fopen("tmp.dat", "wb");
    Video v;
    int id;
    printf("ID do vídeo a remover: ");
    scanf("%d", &id);
    while(fread(&v, sizeof(Video), 1, orig)) {
        if(v.id != id) fwrite(&v, sizeof(Video), 1, novo);
    }
    fclose(orig);
    fclose(novo);
    remove("videos.dat");
    rename("tmp.dat", "videos.dat");
    printf("Vídeo removido!\n");
}

void remover_usuario() {
    FILE *orig = fopen("usuarios.dat", "rb");
    FILE *novo = fopen("tmp.dat", "wb");
    Usuario u;
    int id;
    printf("ID do usuário a remover: ");
    scanf("%d", &id);
    while(fread(&u, sizeof(Usuario), 1, orig)) {
        if(u.id != id) fwrite(&u, sizeof(Usuario), 1, novo);
    }
    fclose(orig);
    fclose(novo);
    remove("usuarios.dat");
    rename("tmp.dat", "usuarios.dat");
    printf("Usuário removido!\n");
}

void relatorio_usuarios() {
    FILE *fu = fopen("usuarios.dat", "rb");
    FILE *fv = fopen("videos.dat", "rb");
    Usuario u;
    Video v;

    printf("\n--- Relatório: Usuários e Vídeos Favoritados ---\n");
    while(fread(&u, sizeof(Usuario), 1, fu)) {
        printf("\nUsuário: %s (%s)\nFavoritos:\n", u.nome, u.email);
        for(int i = 0; i < u.qtd_favoritos; i++) {
            rewind(fv); // reinicia leitura dos vídeos
            while(fread(&v, sizeof(Video), 1, fv)) {
                if(v.id == u.favoritos[i]) {
                    printf("  - %s\n", v.titulo);
                    break;
                }
            }
        }
    }
    fclose(fu);
    fclose(fv);
}
