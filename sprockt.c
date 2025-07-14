
#define INCHTOMM 25.4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int dentes;//
    double d_prim;//mm
    double passo;//polegadas

    
} Sprocket;

double calc_efeito_polig();
Sprocket reg_sprocket();
void clear_stdin_buffer();
void listar_dados_sp(Sprocket sp);
void get_console_line(char* stor, size_t size);
int get_act();
double min_cent_dist(Sprocket sp1, Sprocket sp2);
double variacao_vcordal(Sprocket sp);
int min_comprim_corrente_passos(double d_entre_centros);
double comprim_corrente(double d_entre_centros, Sprocket sp1, Sprocket sp2);
int comprim_corrente_passos(double comp_poleg, double passo);
double distancia_entre_centros(int comp_corrente, Sprocket sp1, Sprocket sp2);

int main() {

    Sprocket* lista;
    lista = malloc(sizeof(Sprocket));
    size_t s_lista = 0;

    int act;

    do {
        act = get_act();

        if(act == 1) {
            s_lista++;
            lista = realloc(lista, s_lista * sizeof(Sprocket));
            lista[s_lista - 1] = reg_sprocket();
        }

        if(act == 2) {
            if(s_lista != 0) {
                for(int i = 0; i < s_lista; i++) {
                    printf("roda %d\n", i + 1);
                    listar_dados_sp(lista[i]);
                    printf("\n");
                }
            } else {
                printf("sem rodas registradas\n");
            }
        }

        if(act == 3) {
            printf("Escolha uma roda (numero): ");
            int t1, t2;
            double t3;
            scanf("%d", &t1);
            clear_stdin_buffer();
            printf("escolha outra roda (numero): ");
            scanf("%d", &t2);
            clear_stdin_buffer();
            
            if(t1 <= s_lista && t2 <= s_lista) {

                t3 = min_cent_dist(lista[t1-1], lista[t2-1]);
                if(t3 != 0) {
                    printf("A distancia minima e de %.3lf polegadas ou %.3lf mm\n", t3, t3 * INCHTOMM);
                    int t4 = comprim_corrente_passos(comprim_corrente(t3, lista[t1-1], lista[t2-1]), lista[t1-1].passo);
                    printf("O comprimento da corrente mais proxima e: %d passos\n", t4);
                    double t5 = distancia_entre_centros(t4, lista[t1-1], lista[t2-1]);
                    printf("A distancia final entre centros sera de: %.3lf polegadas, ou: %.3lfmm\n\n", t5, t5 * INCHTOMM);
                } else {
                    printf("passo nao compativel\n");
                }
            } else {
                printf("numeros digitidos sao invalidos\n");
            }
        }

        if(act == 4) {
            int t1;
            printf("escolha uma roda (numero): ");
            scanf("%d", &t1);
            clear_stdin_buffer();

            if(t1 <= s_lista) {
                printf("a variacao e de %.5lf%%\n", variacao_vcordal(lista[t1-1]) * 100);
            } else {
                printf("valor digitado invalido\n");
            }
        }

    } while (act != 0);


    return 0;
}

Sprocket reg_sprocket() {
    Sprocket sp;

    printf("registrando nova roda dentada\ndigite o num de dentes: ");
    scanf("%d", &sp.dentes);
    clear_stdin_buffer();

    printf("digite o passo (polegadas): ");
    scanf("%lf", &sp.passo);
    clear_stdin_buffer();

    sp.d_prim = sp.passo / (sin(M_PI/sp.dentes));

    return sp;
}

void clear_stdin_buffer() {//usado depois de cada scanf para retirar o \n que fica no buffer do stdin
    int c;
    do {
        c = getchar();
    } while(c != '\n' && c != EOF);
}

void listar_dados_sp(Sprocket sp) {
    printf("num de dentes: %d\npasso: %.3lf polegada(S)\ndiametro primitivo: %.3lf polegadas ou %.3lf mm\n", sp.dentes, sp.passo, sp.d_prim, sp.d_prim * INCHTOMM);
}

void get_console_line(char* stor, size_t size) {

    fgets(stor, size, stdin);
    stor[strcspn(stor, "\n")] = '\0';

}

int get_act() {
    int act;
    printf("Escolha uma acao:\n1.registrar uma nova roda dentada\n2.listar todos as rodas dentadas\n3.calcular a distancia minima entre os centros\n4.calcular a variacao da velocidade cordal da roda\n0.sair\n");
  /*   Escolha uma acao:
        1.registrar uma nova roda dentada
        2.listar todos as rodas dentadas
        3.calcular a distancia minima entre os centros
        4.calcular a variacao da velocidade cordal da roda
        0.sair */
    scanf("%d", &act);
    clear_stdin_buffer();

    return act;
}

double min_cent_dist(Sprocket sp1, Sprocket sp2) {

    if(sp1.passo != sp2.passo) {
        return 0;
    }

    return sp1.d_prim + sp2.d_prim;
}

double variacao_vcordal(Sprocket sp) {
    double v, t;
    t = M_PI / ((double) sp.dentes);
    v = t * ((1/sin(t)) - (1/tan(t)));

    return v;
}

double comprim_corrente(double d_entre_centros, Sprocket sp1, Sprocket sp2) {
    double comp;
    double d_passos = d_entre_centros/sp1.passo;

    comp = (2*d_passos) + ((sp1.dentes + sp2.dentes)/2) + (0.1013 * ((sp1.dentes - sp2.dentes)^2) / (4 * d_passos));

    return comp; //em polegadas
}

int comprim_corrente_passos(double comp_poleg, double passo) {
    int comp;

    comp = (int) comp_poleg/passo;
    if((comp) % 2 != 0) passo++; //a corrente deve ter um numero par de passos, senao seria necessario um link personalizado

    return comp;
}

double distancia_entre_centros(int comp_corrente, Sprocket sp1, Sprocket sp2) {
    double d;
    d = (sp1.passo/8) * ((2 * comp_corrente) - sp1.dentes - sp2.dentes + sqrt((((2 * comp_corrente) - sp1.dentes - sp2.dentes)^2) - (0.81 * ((sp1.dentes - sp2.dentes)^2))));

    return d; //em polegadas
}