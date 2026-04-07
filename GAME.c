#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TOTAL_PAISES 25
#define MAX_JOGADORES 5

typedef struct {
    char nome[30];
    int dono;
    int tropas;
    SDL_Rect retangulo;
} Pais;

typedef struct {
    char nome[30];
    SDL_Color cor;
} Jogador;

void escreverTexto(SDL_Renderer* r, TTF_Font* f, const char* t, int x, int y, SDL_Color c){
    SDL_Surface* s = TTF_RenderText_Blended(f,t,c);
    if (!s) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(r,s);
    SDL_Rect dst = {x,y,s->w,s->h};
    SDL_RenderCopy(r,tex,NULL,&dst);
    SDL_FreeSurface(s);
    SDL_DestroyTexture(tex);
}

int mouseSobre(SDL_Rect r,int mx,int my){
    return mx>=r.x && mx<=r.x+r.w && my>=r.y && my<=r.y+r.h;
}

void rolarDados(int dados[], int qtd){
    for(int i=0;i<qtd;i++) dados[i]=rand()%6+1;
}

void mostrarDados(SDL_Renderer* r, TTF_Font* f, int atk[], int nAtk, int def[], int nDef){
    SDL_Rect fundo = {0,500,800,100};
    SDL_SetRenderDrawColor(r,10,10,10,255);
    SDL_RenderFillRect(r,&fundo);

    char a[100]="ATK: ";
    char d[100]="DEF: ";

    for(int i=0;i<nAtk;i++){ char t[10]; sprintf(t,"%d ",atk[i]); strcat(a,t); }
    for(int i=0;i<nDef;i++){ char t[10]; sprintf(t,"%d ",def[i]); strcat(d,t); }

    escreverTexto(r,f,a,50,520,(SDL_Color){255,80,80,255});
    escreverTexto(r,f,d,50,550,(SDL_Color){80,150,255,255});
}

int main(){
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* w = SDL_CreateWindow("WAR SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800,600,0);
    SDL_Renderer* r = SDL_CreateRenderer(w,-1,SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("arial.ttf",18);

    int rodando=1, menu=1, numJogadores=2;

    SDL_Rect btnJogar={300,200,200,50};
    SDL_Rect btnSairMenu={300,300,200,50};
    SDL_Rect btnMais={520,200,40,40};
    SDL_Rect btnMenos={240,200,40,40};

    // 🔥 CONFIGURAÇÃO ALINHADA DOS BOTÕES LADO DIREITO
    int larguraBtn = 150;
    int alturaBtn = 50;
    int xBtn = 640; // Ajustado para centralizar melhor no painel de 620 a 800
    
    SDL_Rect btnAtacar   = {xBtn, 100, larguraBtn, alturaBtn};
    SDL_Rect btnPular    = {xBtn, 170, larguraBtn, alturaBtn}; // 100 + 50 + 20 de espaço
    SDL_Rect btnSairJogo = {xBtn, 240, larguraBtn, alturaBtn}; // 170 + 50 + 20 de espaço

    Jogador jogadores[MAX_JOGADORES];
    SDL_Color cores[]={{255,50,50,255},{50,50,255,255},{50,255,50,255},{255,255,50,255},{200,50,250,255}};

    Pais mapa[TOTAL_PAISES];
    const char* nomes[]={"Brasil","EUA","China","Russia","Alemanha","Japao","India","Egito","Franca","Italia",
    "Canada","Mexico","Australia","Argentina","Chile","Peru","Angola","Grecia","Turquia","Ira",
    "Iraque","Coreia","Espanha","Suecia","Congo"};

    Pais* selecionado=NULL;
    int modoAtaque=0, turno=0;

    int dadosAtk[3], dadosDef[3], nAtk=0, nDef=0;

    int tropasDisponiveis=0;
    int tropasCalculadas=0;

    while(rodando){
        SDL_Event ev;
        while(SDL_PollEvent(&ev)){
            if(ev.type==SDL_QUIT) rodando=0;

            if(ev.type==SDL_MOUSEBUTTONDOWN){
                int mx=ev.button.x,my=ev.button.y;

                if(menu){
                    if(mouseSobre(btnJogar,mx,my)){
                        for(int i=0;i<numJogadores;i++){
                            sprintf(jogadores[i].nome,"Jogador %d",i+1);
                            jogadores[i].cor=cores[i];
                        }
                        for(int i=0;i<TOTAL_PAISES;i++){
                            strcpy(mapa[i].nome,nomes[i]);
                            mapa[i].dono=i%numJogadores;
                            mapa[i].tropas=3;
                            mapa[i].retangulo=(SDL_Rect){20+(i%5)*120,20+(i/5)*80,120,60};
                        }
                        turno=0; menu=0; tropasCalculadas=0;
                    }
                    if(mouseSobre(btnSairMenu,mx,my)) rodando=0;
                    if(mouseSobre(btnMais,mx,my) && numJogadores<5) numJogadores++;
                    if(mouseSobre(btnMenos,mx,my) && numJogadores>2) numJogadores--;
                }
                else{
                    int id=turno%numJogadores;
                    if(mouseSobre(btnAtacar,mx,my)){modoAtaque=1; selecionado=NULL;}
                    if(mouseSobre(btnPular,mx,my)){
                        modoAtaque=0; turno++; selecionado=NULL; tropasCalculadas=0;
                    }
                    if(mouseSobre(btnSairJogo,mx,my)){menu=1;}

                    for(int i=0;i<TOTAL_PAISES;i++){
                        Pais* p=&mapa[i];
                        if(mouseSobre(p->retangulo,mx,my)){
                            if(modoAtaque){
                                if(!selecionado && p->dono==id && p->tropas>1) selecionado=p;
                                else if(selecionado && p->dono!=id){
                                    nAtk = selecionado->tropas>3?3:selecionado->tropas-1;
                                    nDef = p->tropas>2?3:p->tropas;
                                    rolarDados(dadosAtk,nAtk);
                                    rolarDados(dadosDef,nDef);

                                    for(int a=0;a<nAtk-1;a++)
                                        for(int b=a+1;b<nAtk;b++)
                                            if(dadosAtk[a]<dadosAtk[b]){ int t=dadosAtk[a]; dadosAtk[a]=dadosAtk[b]; dadosAtk[b]=t; }

                                    for(int a=0;a<nDef-1;a++)
                                        for(int b=a+1;b<nDef;b++)
                                            if(dadosDef[a]<dadosDef[b]){ int t=dadosDef[a]; dadosDef[a]=dadosDef[b]; dadosDef[b]=t; }

                                    int min = nAtk<nDef?nAtk:nDef;
                                    for(int d=0;d<min;d++){
                                        if(dadosAtk[d]>dadosDef[d]) p->tropas--;
                                        else selecionado->tropas--;
                                    }
                                    if(p->tropas<=0){ p->dono=id; p->tropas=1; selecionado->tropas--; }
                                    selecionado=NULL;
                                }
                            }
                            else if(p->dono==id && tropasDisponiveis>0){
                                p->tropas++; tropasDisponiveis--;
                            }
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(r,20,20,20,255);
        SDL_RenderClear(r);

        int mx,my;
        SDL_GetMouseState(&mx,&my);

        if(menu){
            SDL_SetRenderDrawColor(r,0,200,0,255);
            SDL_RenderFillRect(r,&btnJogar);
            SDL_SetRenderDrawColor(r,200,0,0,255);
            SDL_RenderFillRect(r,&btnSairMenu);

            SDL_SetRenderDrawColor(r, mouseSobre(btnMais,mx,my)?255:200,255,100,255);
            SDL_RenderFillRect(r,&btnMais);
            SDL_SetRenderDrawColor(r, mouseSobre(btnMenos,mx,my)?255:200,255,100,255);
            SDL_RenderFillRect(r,&btnMenos);

            escreverTexto(r,font,"JOGAR",360,215,(SDL_Color){255,255,255,255});
            escreverTexto(r,font,"SAIR",370,315,(SDL_Color){255,255,255,255});
            escreverTexto(r,font,"+",btnMais.x+10,btnMais.y,(SDL_Color){0,0,0,255});
            escreverTexto(r,font,"-",btnMenos.x+12,btnMenos.y,(SDL_Color){0,0,0,255});

            char buf[50];
            sprintf(buf,"Jogadores: %d",numJogadores);
            escreverTexto(r,font,buf,330,150,(SDL_Color){255,255,0,255});
        }
        else{
            int id=turno%numJogadores;
            if(!tropasCalculadas){
                int territ=0;
                for(int i=0;i<TOTAL_PAISES;i++) if(mapa[i].dono==id) territ++;
                tropasDisponiveis = territ/2;
                if(tropasDisponiveis<1) tropasDisponiveis=1;
                tropasCalculadas=1;
            }

            SDL_Rect painel = {620,0,180,600};
            SDL_SetRenderDrawColor(r,30,30,30,255);
            SDL_RenderFillRect(r,&painel);

            for(int i=0;i<TOTAL_PAISES;i++){
                Pais* p=&mapa[i];
                SDL_SetRenderDrawColor(r, jogadores[p->dono].cor.r, jogadores[p->dono].cor.g, jogadores[p->dono].cor.b,255);
                SDL_RenderFillRect(r,&p->retangulo);
                SDL_SetRenderDrawColor(r,255,255,255,255);
                SDL_RenderDrawRect(r,&p->retangulo);
                char txt[40];
                sprintf(txt,"%s (%d)",p->nome,p->tropas);
                escreverTexto(r,font,txt,p->retangulo.x+5,p->retangulo.y+5,(SDL_Color){255,255,255,255});
            }

            char hud[80];
            sprintf(hud,"Vez: %s | Tropas: %d | %s", jogadores[id].nome, tropasDisponiveis, modoAtaque?"ATAQUE":"REFORCO");
            escreverTexto(r,font,hud,20,460,(SDL_Color){255,255,255,255});

            // 🔥 DESENHO DOS BOTÕES LADO DIREITO COM HOVER
            SDL_SetRenderDrawColor(r, mouseSobre(btnAtacar,mx,my)?150:100, 100, 100, 255);
            SDL_RenderFillRect(r,&btnAtacar);
            
            SDL_SetRenderDrawColor(r, mouseSobre(btnPular,mx,my)?150:100, 100, 100, 255);
            SDL_RenderFillRect(r,&btnPular);
            
            SDL_SetRenderDrawColor(r, mouseSobre(btnSairJogo,mx,my)?150:100, 100, 100, 255);
            SDL_RenderFillRect(r,&btnSairJogo);

            // TEXTOS DOS BOTÕES (Ajustados para o novo tamanho)
            escreverTexto(r,font,"Atacar",btnAtacar.x+45,btnAtacar.y+15,(SDL_Color){255,255,255,255});
            escreverTexto(r,font,"Pular",btnPular.x+50,btnPular.y+15,(SDL_Color){255,255,255,255});
            escreverTexto(r,font,"Sair",btnSairJogo.x+55,btnSairJogo.y+15,(SDL_Color){255,255,255,255});

            if(nAtk>0) mostrarDados(r,font,dadosAtk,nAtk,dadosDef,nDef);
        }
        SDL_RenderPresent(r);
    }
    SDL_Quit();
    return 0;
}
