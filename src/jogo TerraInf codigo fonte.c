#include "raylib.h"
#include<unistd.h>
#include <time.h>
#include <math.h>
#include<string.h>
#include <stdio.h>
#include<ctype.h>
typedef struct   //struct para armazenar os nomes e pontuações do ranking
{
    char nome[50];
    int pontuacao;

} jogadorRanking_t;
typedef struct   //stuct para as informações do jogador
{
    char nome[50];
    char ultimoItem[20];
    int pontuacao,x,y,vidas,energia,escadas;
} jogador_t;

int main(void)
{   //variaveis uteis como condição para habilitar/desabilitar determinada açao,
    //preferiu declará-las fora do while da tela para não gerar nenhum conflito ou mudança indevida dessas variaves
    // O mesmo vale para algumas outras variaves declaradas na main
    int flagFimJogo=0;
    int CapsLock=0;
    int flagSemNome=0;
    int delayVoltarMenu=0;
    int flagInseNome=0;
    int flagGameOver=0;
    int flagMudaNivel=0;
    int pontuacaoAnterior=0;
    int contagemQueda=0;
    int flag=0;
    int modoMin=0;
    int escada=0;
    int playingSomInicio = 0;
    int playingSomDoJogo = 0;
    int mouseRanking=0;
    int mouseSair=0;
    int mouseNovoJogo=0;
    int voltarMenu=0;
    int sair=0;
    //definindo algumas informações do jogo e do jogador
    int nivel=1;
    int pontuacaoNivel=1000;

    jogador_t jogador;
    jogador.vidas=3;
    jogador.pontuacao=0;
    jogador.energia=100;
    jogador.escadas=20;
    strcpy(jogador.ultimoItem,"ainda nao minerou");

    //preparando a matriz da fase 1
    FILE *matrizMapa=fopen("nivel1.txt","r");



        char matriz1[20][30];
        int i,j,aux3[5]= {0};
        for(i=0; i<20; i++)
        {
            for(j=0; j<30; j++)
            {

                matriz1[i][j]=fgetc(matrizMapa);
            }

            fgetc(matrizMapa);
        }
        for(i=0; i<20; i++)
        {
            for(j=0; j<30; j++)
            {

                if(matriz1[i][j]=='J')
                {
                    jogador.x=j+1;
                    jogador.y=i+1;
                }
            }


        }



        // remove("ranking.bin");
        jogadorRanking_t jogadorRanking[5];
        //preparando o arquivo do ranking
        FILE *arquivoRanking=fopen("ranking.bin", "rb+");
        if(arquivoRanking==NULL)
        {
            arquivoRanking=fopen("ranking.bin", "wb+");
            strcpy(jogadorRanking[0].nome, "Pedro");
            strcpy(jogadorRanking[1].nome, "Andre");
            strcpy(jogadorRanking[2].nome, "Bruna");
            strcpy(jogadorRanking[3].nome, "Renata");
            strcpy(jogadorRanking[4].nome, "Felipe");
            jogadorRanking[0].pontuacao=0;
            jogadorRanking[1].pontuacao=0;
            jogadorRanking[2].pontuacao=0;
            jogadorRanking[3].pontuacao=0;
            jogadorRanking[4].pontuacao=0;
            for(i=0; i<5; i++)
            {
                fwrite(jogadorRanking[i].nome, sizeof(char), 50, arquivoRanking);
                fwrite(&(jogadorRanking[i].pontuacao), sizeof(int), 1, arquivoRanking);
            }
            rewind(arquivoRanking);
        }
        else
        {

            int i;
            for(i=0; i<5; i++)
            {
                fread(jogadorRanking[i].nome, sizeof(char), 50, arquivoRanking);
                fread(&(jogadorRanking[i].pontuacao), sizeof(int), 1, arquivoRanking);
            }
            rewind(arquivoRanking);
        }


        //variaveis uteis para ajeitar a posicao dos nomes e pontuaçoes do ranking
        char aux2[100]=" ";
        int tamanhoNome[5],ajeitaNome=0,aux;
        char rankingText[5][60];
        //coordenadas do mouse
        int x,y;



        // Screen width and height
        const int screenWidth = 1200;
        const int screenHeight = 800;
        Sound coinSound = LoadSound("coin.wav");
        InitWindow(screenWidth, screenHeight, "TerraINF");
        SetTargetFPS(60);
        Image capa = LoadImage("capa.png");
        Texture2D textureCapa = LoadTextureFromImage(capa);
        UnloadImage(capa);
        Image Ranking = LoadImage("ranking(6).png");
        Texture2D textureRanking = LoadTextureFromImage(Ranking);
        UnloadImage(Ranking);
        Image fundoHud=LoadImage("fundo_hud.png");
        Texture2D textureFundoHud = LoadTextureFromImage(fundoHud);
        UnloadImage(fundoHud);
        Image Terra=LoadImage("terra.png");
        Texture2D textureTerra = LoadTextureFromImage(Terra);
        UnloadImage(Terra);
        Image fundoTerra=LoadImage("fundo_terra.png");
        Texture2D textureFundoTerra = LoadTextureFromImage(fundoTerra);
        UnloadImage(fundoTerra);
        Image borda=LoadImage("borda.png");
        Texture2D textureBorda = LoadTextureFromImage(borda);
        UnloadImage(borda);
        Image minerio=LoadImage("minerio.png");
        Texture2D textureMinerio = LoadTextureFromImage(minerio);
        UnloadImage(minerio);
        Image jogadorImg=LoadImage("jogador.png");
        Texture2D textureJogador = LoadTextureFromImage(jogadorImg);
        UnloadImage(jogadorImg);
        Image jogadorEscadaImg=LoadImage("jogador_escada.png");
        Texture2D textureJogadorEscada = LoadTextureFromImage(jogadorEscadaImg);
        UnloadImage(jogadorEscadaImg);
        Image jogadorPicareta=LoadImage("jogador_picareta.png");
        Texture2D texturejogadorPicareta = LoadTextureFromImage(jogadorPicareta);
        UnloadImage(jogadorPicareta);
        Image jogadorPicaretaEscada=LoadImage("jogador_picareta_escada.png");
        Texture2D texturejogadorPicaretaEscada = LoadTextureFromImage(jogadorPicaretaEscada);
        UnloadImage(jogadorPicaretaEscada);
        Image EscadaImg=LoadImage("escada.png");
        Texture2D textureEscada = LoadTextureFromImage(EscadaImg);
        UnloadImage(EscadaImg);

        InitAudioDevice();
         //audio somInicio desconsiderado
        Sound clickSound = LoadSound ("mixkit_game_click.wav");
        Sound somInicio = LoadSound ("Bem-vindos-ao-terra-INF.wav");
        Sound somDoJogo = LoadSound ("terrariaMusicDay.wav");






        while ((!WindowShouldClose())&&(mouseSair!=20))
        {
            x=GetMouseX();
            y=GetMouseY();
            if(mouseRanking!=20&&mouseNovoJogo!=20) //dentro desse if está o menu
            {


                BeginDrawing();
                ClearBackground(RAYWHITE);

                // Draw the loaded texture
                DrawTexture(textureCapa, 0, 0, WHITE);
                DrawRectangle(495, 400, 215, 60, DARKBROWN);
                DrawText("Novo Jogo", 528, 420, 30, LIGHTGRAY);
                DrawRectangle(445, 490, 315, 60, DARKBROWN);
                DrawText("Ranking de Pontos", 463, 510, 30, LIGHTGRAY);
                DrawRectangle(550, 580, 105, 60, DARKBROWN);
                DrawText("Sair", 574, 600, 30, LIGHTGRAY);

                if (playingSomInicio==0 && !IsSoundPlaying(somInicio)) //os dois ifs a seguir serviam para iniciar o Som do Inicio e depois o som do jogo porem
                {                                                      //foi decidido desabilitar o som do inicio
                    // PlaySound(somInicio);
                    playingSomInicio = 1;
                }
                //para cada opçao do menu, há uma logica para aumentar o barra da opção ao passar proximo dela, e, ao clica, mudar de cor, gerar um barulho
                //de click, e um delay para tenha tempo para que isso seja percebido
                if (playingSomInicio==1 && !IsSoundPlaying(somInicio) && playingSomDoJogo ==0)
                {
                    PlaySound(somDoJogo);
                    playingSomDoJogo = 1;
                }
                if(x>465&&x<740&&y>390&&y<470)
                {

                    DrawRectangle(465, 390, 275, 80, DARKBROWN);
                    DrawText("Novo Jogo", 487, 412, 46, LIGHTGRAY);


                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        mouseNovoJogo=1;
                        PlaySound(clickSound);
                    }

                }
                if(mouseNovoJogo>0&&mouseNovoJogo<10)
                {
                    DrawRectangle(465, 390, 275, 80, BROWN);
                    DrawText("Novo Jogo", 487, 412, 46, GRAY);
                    mouseNovoJogo++;
                }
                if(mouseNovoJogo>=10&&mouseNovoJogo<20)
                {
                    mouseNovoJogo++;

                }


                if(x>370&&x<835&&y>480&&y<560)
                {

                    DrawRectangle(370, 480, 465, 80, DARKBROWN);
                    DrawText("Ranking de Pontos", 390, 502, 46, LIGHTGRAY);
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        mouseRanking=1;
                        PlaySound(clickSound);
                    }

                }
                if(mouseRanking>0&&mouseRanking<10)
                {
                    DrawRectangle(370, 480, 465, 80, BROWN);
                    DrawText("Ranking de Pontos", 390, 502, 46, GRAY);
                    mouseRanking++;
                }
                if(mouseRanking>=10&&mouseRanking<20)
                {
                    mouseRanking++;

                }




                if(x>520&&x<685&&y>570&&y<650)
                {

                    DrawRectangle(520, 570, 165, 80, DARKBROWN);
                    DrawText("Sair", 560, 592, 46, LIGHTGRAY);
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        mouseSair=1;
                        PlaySound(clickSound);
                    }

                }
                if(mouseSair>0&&mouseSair<10)
                {
                    DrawRectangle(520, 570, 165, 80, BROWN);
                    DrawText("Sair", 560, 592, 46, GRAY);
                    mouseSair++;
                }
                if(mouseSair>=10&&mouseSair<20)
                {
                    mouseSair++;

                }
            }
            else if(mouseNovoJogo!=20)// nesse if entramos no ranking
            {
                StopSound(somInicio);
                ClearBackground(RAYWHITE);
                DrawTexture(textureRanking, 0, 0, WHITE);
                if (playingSomInicio==1 && !IsSoundPlaying(somInicio) && playingSomDoJogo ==0)
                {
                    PlaySound(somDoJogo);
                    playingSomDoJogo = 1;
                }
                for(i=0; i<5; i++)
                {
                    fread(jogadorRanking[i].nome, sizeof(char), 50, arquivoRanking);
                    fread(&(jogadorRanking[i].pontuacao), sizeof(int), 1, arquivoRanking);
                }
                rewind(arquivoRanking);
                //codigo para ajeitar a exibição do ranking:
                for(i=0; i<5; i++)
                {
                    tamanhoNome[i]=strlen(jogadorRanking[i].nome);
                    if(tamanhoNome[i]>ajeitaNome)
                    {
                        ajeitaNome=tamanhoNome[i];
                    }
                }
                for(i=0; i<5; i++)
                {
                    sprintf(rankingText[i], " : %d", jogadorRanking[i].pontuacao);
                    aux=ajeitaNome-tamanhoNome[i];
                    if(aux>0)
                    {
                        for(j=1; j<aux; j++)
                        {
                            strcat(aux2," ");
                        }
                        strcat(jogadorRanking[i].nome,aux2);
                        strcpy(aux2," ");
                    }

                }
                int ajeitaPosicao=0;
                for(i=0; i<5; i++)
                {
                    if(ajeitaPosicao<strlen(jogadorRanking[i].nome))
                    {
                        ajeitaPosicao=strlen(jogadorRanking[i].nome);
                    }
                }
                if(ajeitaPosicao>9)
                {
                    ajeitaPosicao=ajeitaPosicao-9;
                }
                else
                {
                    ajeitaPosicao=0;
                }
                for(i=0; i<5; i++)
                {
                    DrawText(jogadorRanking[i].nome,  670-30*ajeitaPosicao, (220+70*i), 50, GRAY);
                    DrawText(rankingText[i], 930, (220+70*i), 50, GRAY);
                }

                DrawRectangle(762, 570, 135, 60, DARKBROWN);
                DrawText("Menu", 793, 585, 30, LIGHTGRAY);
                if(x>732&&x<922&&y>560&&y<640)
                {

                    DrawRectangle(732, 560, 190, 80, DARKBROWN);
                    DrawText("Menu", 772, 577, 46, LIGHTGRAY);
                    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        voltarMenu=1;
                        PlaySound(clickSound);
                    }

                }
                if(voltarMenu>0&&voltarMenu<10)
                {
                    DrawRectangle(732, 560, 190, 80, BROWN);
                    DrawText("Menu", 772, 577, 46, GRAY);
                    voltarMenu++;
                }
                if(voltarMenu>=10&&voltarMenu<20)
                {
                    voltarMenu++;

                }
                if(voltarMenu==20)
                {
                    mouseRanking=0;
                    voltarMenu=0;
                }
            }
            else if(flagMudaNivel==0&&flagGameOver==0&&flagFimJogo==0)//nesse if começamos uma nova fase
            {   //transfomando informações em char para exibir na tela
                char score[50];
                char vidas[3];
                char energia[5];
                char escadas[4];
                char fase[3];
                energia[0]='\0';
                StopSound(somInicio);
                ClearBackground(RAYWHITE);
                sprintf(fase, " %d", nivel);
                sprintf(escadas, " %d", jogador.escadas);
                sprintf(energia, " %d", jogador.energia);
                sprintf(vidas, " %d", jogador.vidas);
                sprintf(score, " %d pontos", jogador.pontuacao);
                DrawTexture(textureFundoHud, 0, 0, WHITE);
                DrawText("SCORE:", 10, 10, 20, YELLOW);
                DrawText(score, 90, 10, 20, RED);
                DrawText("VIDAS:", 250, 10, 20, YELLOW);
                DrawText(vidas, 320, 10, 20, RED);
                DrawText("ENERGIA:", 400, 10, 20, YELLOW);
                DrawText(energia, 500, 10, 20, RED);
                DrawText("ESCADAS:", 600, 10, 20, YELLOW);
                DrawText(escadas, 700, 10, 20, RED);
                DrawText("FASE:", 800, 10, 20, YELLOW);
                DrawText(fase, 860, 10, 20, RED);
                DrawText("ITEM:", 930, 10, 20, YELLOW);
                DrawText(jogador.ultimoItem, 1000, 10, 20, RED);
                if (playingSomInicio==1 && !IsSoundPlaying(somInicio) && playingSomDoJogo ==0)
                {
                    PlaySound(somDoJogo);
                    playingSomDoJogo = 1;
                }
                //exibindo os espaços na tela de acordo com o que esta atualmente na matriz
                for(i=1; i<20; i++)
                {
                    for(j=0; j<30; j++)
                    {
                        switch (matriz1[i][j])
                        {
                        case 'B':
                            DrawTexture(textureBorda, j*40, i*40, WHITE);
                            break;
                        case 'X':
                            DrawTexture(textureTerra, j*40, i*40, WHITE);
                            break;
                        case ' ':
                            DrawTexture(textureFundoTerra, j*40, i*40, WHITE);
                            break;
                        case 'J':
                        {    //logica para considerar as varias possibilidades de estado do jogador:
                            //modo mineração ligado/desligado, e se esta ou nao em uma escada
                            if(modoMin==0)
                            {
                                if(escada==0)
                                {
                                    DrawTexture(textureJogador, j*40, i*40, WHITE);
                                }
                                else if(escada==1)
                                {
                                    DrawTexture(textureJogadorEscada, j*40, i*40, WHITE);
                                }
                            }
                            else if(modoMin==1)
                            {
                                if(escada==0)
                                {
                                    DrawTexture(texturejogadorPicareta, j*40, i*40, WHITE);
                                }
                                else if(escada==1)
                                {
                                    DrawTexture(texturejogadorPicaretaEscada, j*40, i*40, WHITE);
                                }
                            }
                        }
                        break;
                        case 'H':
                            DrawTexture(textureEscada, j*40, i*40, WHITE);
                            break;
                        default:
                            DrawTexture(textureMinerio, j*40, i*40, WHITE);
                            break;
                        }

                    }

                }
                int flagCair=0;
                //codigos para a movimentaçao do jogador, considerando se ele estava ou nao em uma escada e se ele vai estar ou não em uma
                if(IsKeyPressed(KEY_A)&&modoMin==0)
                {
                    if(matriz1[jogador.y-1][jogador.x-2]==' ')
                    {
                        if(escada==1)
                        {
                            matriz1[jogador.y-1][jogador.x-2]=matriz1[jogador.y-1][jogador.x-1];
                            matriz1[jogador.y-1][jogador.x-1]='H';
                            jogador.x=jogador.x-1;
                            escada=0;
                        }
                        else if(escada==0)
                        {
                            matriz1[jogador.y-1][jogador.x-2]=matriz1[jogador.y-1][jogador.x-1];
                            matriz1[jogador.y-1][jogador.x-1]=' ';
                            jogador.x=jogador.x-1;
                        }

                    }
                    else if(matriz1[jogador.y-1][jogador.x-2]=='H')
                    {
                        if(escada==1)
                        {
                            matriz1[jogador.y-1][jogador.x-2]=matriz1[jogador.y-1][jogador.x-1];
                            matriz1[jogador.y-1][jogador.x-1]='H';
                            jogador.x=jogador.x-1;
                        }
                        else if(escada==0)
                        {
                            matriz1[jogador.y-1][jogador.x-2]=matriz1[jogador.y-1][jogador.x-1];
                            matriz1[jogador.y-1][jogador.x-1]=' ';
                            jogador.x=jogador.x-1;
                            escada=1;
                        }


                    }

                }
                if(IsKeyPressed(KEY_D)&&modoMin==0)
                {
                    if(matriz1[jogador.y-1][jogador.x]==' ')
                    {
                        if(escada==1)
                        {
                            matriz1[jogador.y-1][jogador.x]=matriz1[jogador.y-1][jogador.x-1];
                            matriz1[jogador.y-1][jogador.x-1]='H';
                            jogador.x=jogador.x+1;
                            escada=0;
                        }
                        else if(escada==0)
                        {
                            matriz1[jogador.y-1][jogador.x]=matriz1[jogador.y-1][jogador.x-1];
                            matriz1[jogador.y-1][jogador.x-1]=' ';
                            jogador.x=jogador.x+1;
                        }

                    }
                    else if(matriz1[jogador.y-1][jogador.x]=='H')
                    {
                        if(escada==1)
                        {
                            matriz1[jogador.y-1][jogador.x]=matriz1[jogador.y-1][jogador.x-1];
                            matriz1[jogador.y-1][jogador.x-1]='H';
                            jogador.x=jogador.x+1;
                        }
                        else if(escada==0)
                        {
                            matriz1[jogador.y-1][jogador.x]=matriz1[jogador.y-1][jogador.x-1];
                            matriz1[jogador.y-1][jogador.x-1]=' ';
                            jogador.x=jogador.x+1;
                            escada=1;
                        }


                    }


                }

                if(IsKeyPressed(KEY_W)&&escada==1&&modoMin==0&&matriz1[jogador.y-2][jogador.x-1]=='H')
                {
                    matriz1[jogador.y-2][jogador.x-1]=matriz1[jogador.y-1][jogador.x-1];
                    matriz1[jogador.y-1][jogador.x-1]='H';
                    jogador.y=jogador.y-1;

                }
                else if(IsKeyPressed(KEY_W)&&escada==1&&matriz1[jogador.y-2][jogador.x-1]==' '&&modoMin==0)
                {
                    escada=0;
                    matriz1[jogador.y-2][jogador.x-1]=matriz1[jogador.y-1][jogador.x-1];
                    matriz1[jogador.y-1][jogador.x-1]='H';
                    jogador.y=jogador.y-1;

                }
                if(IsKeyPressed(KEY_S)&&matriz1[jogador.y][jogador.x-1]=='H'&&modoMin==0)
                {
                    matriz1[jogador.y][jogador.x-1]=matriz1[jogador.y-1][jogador.x-1];
                    if(escada==0)
                    {
                        escada=1;
                        matriz1[jogador.y-1][jogador.x-1]=' ';
                    }
                    else
                    {

                        matriz1[jogador.y-1][jogador.x-1]='H';
                    }

                    jogador.y=jogador.y+1;

                }
                else if(IsKeyPressed(KEY_S)&&matriz1[jogador.y][jogador.x-1]==' '&&modoMin==0&&escada==1)
                {
                    matriz1[jogador.y][jogador.x-1]=matriz1[jogador.y-1][jogador.x-1];
                    matriz1[jogador.y-1][jogador.x-1]='H';
                    escada=0;
                    jogador.y=jogador.y+1;

                }

                if(matriz1[jogador.y][jogador.x-1]==' '&&escada==0)//serve para estabelecer as quedas
                {
                    matriz1[jogador.y][jogador.x-1]=matriz1[jogador.y-1][jogador.x-1];




                    matriz1[jogador.y-1][jogador.x-1]=' ';

                    jogador.y=jogador.y+1;
                    contagemQueda++;


                }
                else
                {
                    if(contagemQueda>3)
                    {
                        jogador.vidas--;
                    }
                    contagemQueda=0;
                }


                if (IsKeyPressed(KEY_ONE)&&(modoMin==0))
                {
                    modoMin=1;


                }
                else if(IsKeyPressed(KEY_ONE)&&(modoMin==1))
                {
                    modoMin=0;


                }
                int Ay,Ax;
                if(modoMin==1)
                {

                    if(IsKeyDown(KEY_D))
                    {
                        if(IsKeyPressed(KEY_SPACE))
                        {
                            flag=1;
                            Ay=-1;
                            Ax=0;
                        }
                    }
                    if(IsKeyDown(KEY_A))
                    {
                        if(IsKeyPressed(KEY_SPACE))
                        {
                            flag=1;
                            Ay=-1;
                            Ax=-2;
                        }
                    }
                    if(IsKeyDown(KEY_S))
                    {
                        if(IsKeyPressed(KEY_SPACE))
                        {

                            flag=1;
                            Ay=0;
                            Ax=-1;
                        }

                    }
                    if(IsKeyDown(KEY_W))
                    {
                        if(IsKeyPressed(KEY_SPACE))
                        {
                            flag=1;
                            Ay=-2;
                            Ax=-1;
                        }
                    }




                }
                if(flag==1)
                {
                    flag=0;
                    switch (matriz1[jogador.y+Ay][jogador.x+Ax])
                    {
                    case 'X':
                        matriz1[jogador.y+Ay][jogador.x+Ax]=' ';
                        jogador.energia=jogador.energia-3;
                        break;
                    case 'G':
                    {
                        matriz1[jogador.y+Ay][jogador.x+Ax]=' ';
                        jogador.energia=jogador.energia+20;
                        jogador.pontuacao=jogador.pontuacao+100;
                        strcpy(jogador.ultimoItem,"Ouro");
                    }
                    break;
                    case 'T':
                    {
                        matriz1[jogador.y+Ay][jogador.x+Ax]=' ';
                        jogador.energia=jogador.energia+30;
                        jogador.pontuacao=jogador.pontuacao+150;
                        strcpy(jogador.ultimoItem,"Titanio");
                    }
                    break;
                    case 'S':
                    {
                        matriz1[jogador.y+Ay][jogador.x+Ax]=' ';
                        jogador.energia=jogador.energia+10;
                        jogador.pontuacao=jogador.pontuacao+50;
                        strcpy(jogador.ultimoItem,"Prata");
                    }
                    break;
                    case 'U':
                    {
                        matriz1[jogador.y+Ay][jogador.x+Ax]=' ';
                        jogador.energia=jogador.energia-30;
                        strcpy(jogador.ultimoItem,"Uranio");
                    }
                    break;
                    case 'C':
                    {
                        matriz1[jogador.y+Ay][jogador.x+Ax]=' ';
                        jogador.energia=jogador.energia-20;
                        strcpy(jogador.ultimoItem,"Cesio");
                    }
                    break;
                    default:
                        break;
                    }


                }
                if(jogador.energia<=20)
                {
                    jogador.vidas--;
                    jogador.energia=100;
                }


                if (IsKeyPressed(KEY_LEFT_SHIFT)&&escada==0&&modoMin==0)
                {
                    escada=1;
                    jogador.escadas--;
                }
                if((jogador.pontuacao-pontuacaoAnterior)>=pontuacaoNivel)//verifica se a pontuação que ele teve na fase é maior ou igual a pontuação para passar de fase
                {

                    nivel++;
                    if(nivel==3)
                    {
                        flagFimJogo=1;


                    }
                    else
                    {
                        pontuacaoAnterior=jogador.pontuacao;
                        pontuacaoNivel=2*pontuacaoNivel;

                        i=0;
                        flagMudaNivel=1;
                    }
                }
                if(jogador.vidas==0)
                {
                    flagGameOver=1;
                }



            }

            else if(flagMudaNivel==1)//tela de mudança de nivel
            {
                if(i<360)
                {
                    char text[100];
                    ClearBackground(RAYWHITE);
                    DrawRectangle(0,0,1200,800,YELLOW);
                    sprintf(text,"Parabens, voce passou para o nivel %d!",nivel);
                    DrawText(text,150,250,50,RED);
                    if(i<30||(i<120&&i>=90)||(i<210&&i>=180)||(i<300&&i>=270))
                    {
                        DrawText("Carregando o proximo nivel.",270,450,50,RED);
                    }
                    if((i<60&&i>=30)||(i<150&&i>=120)||(i<240&&i>=210)||(i<330&&i>=300))
                    {
                        DrawText("Carregando o proximo nivel..",270,450,50,RED);
                    }
                    if((i<90&&i>=60)||(i<180&&i>=150)||(i<270&&i>=240)||(i<360&&i>=330))
                    {
                        DrawText("Carregando o proximo nivel...",270,450,50,RED);
                    }
                    i++;
                }

                if(i==360)
                {
                    if(nivel==2)
                    {
                        matrizMapa=fopen("nivel2.txt","r");
                    }





                        for(i=0; i<20; i++)
                        {
                            for(j=0; j<30; j++)
                            {

                                matriz1[i][j]=fgetc(matrizMapa);
                            }

                            fgetc(matrizMapa);
                        }
                        for(i=0; i<20; i++)
                        {
                            for(j=0; j<30; j++)
                            {

                                if(matriz1[i][j]=='J')
                                {
                                    jogador.x=j+1;
                                    jogador.y=i+1;
                                }
                            }


                        }



                    jogador.escadas=20;
                    strcpy(jogador.ultimoItem,"ainda nao minerou");
                    flagMudaNivel=0;

                }
            }
            if(flagGameOver==1||flagFimJogo==1)//tela de fim de jogo, considerando se acabou o jogo porque ele venceu todos os niveis
            {                                  //ou perdeu todas as vidas (game over)
                char text2[100];               //depois testa se ele pode entrar no ranking, e, se sim, pergunta se o jogador quer

                ClearBackground(RAYWHITE);
                DrawRectangle(0,0,1200,800,YELLOW);

                if(flagGameOver==1)
                {
                    DrawText("GAME OVER",350,200,80,RED);
                }
                else if(flagFimJogo==1)
                {
                    DrawText("Voce venceu o jogo,parabens!",170,200,60,RED);
                }

                if(jogadorRanking[4].pontuacao<jogador.pontuacao)
                {
                    DrawText("Voce foi um dos 5 melhores jogadores!",125,350,50,RED);
                    DrawText("Deseja ser adicionado ao ranking(S\\N)?",100,460,50,RED);
                    if(IsKeyPressed(KEY_S)&&flagInseNome==0)
                    {
                        flagInseNome=1;

                        int buffer;
                        buffer=GetKeyPressed();
                        for(i=0; i<50; i++)
                        {
                            jogador.nome[i]='\0';
                        }

                    }
                    if(flagInseNome==1)// nesse if inserimos o nome do jogador na tela
                    {
                        int caracter;
                        caracter=GetKeyPressed();


                        if(caracter==259)
                        {
                            jogador.nome[strlen(jogador.nome)-1]='\0';
                        }
                        if(caracter==280)
                        {

                            if(CapsLock==1)
                            {
                                CapsLock=0;
                            }
                            else
                            {
                                CapsLock=1;

                            }

                        }

                        if(caracter>=32&&caracter<=126)
                        {
                            if(CapsLock==1)
                            {
                                caracter=toupper(caracter);
                            }
                            else
                            {
                                caracter=tolower(caracter);
                            }

                            jogador.nome[strlen(jogador.nome)] = caracter;
                            jogador.nome[strlen(jogador.nome)] = '\0';
                        }
                        DrawText(jogador.nome,400,620,50,RED);

                        DrawText("Digite o nome do Jogador:",300,550,50,RED);






                    }
                    if(IsKeyPressed(KEY_ENTER)&&strcmp(jogador.nome,"\0")!=0)
                    {
                        flagInseNome=2;
                    }

                    if(flagInseNome==2)
                    {
                        if(delayVoltarMenu==0)
                        {
                            i=0;
                            while(jogadorRanking[i].pontuacao>=jogador.pontuacao&&i<5)
                            {
                                i++;
                            }


                            for(j=3; j>=i; j--)
                            {

                                jogadorRanking[j+1].pontuacao=jogadorRanking[j].pontuacao;
                                strcpy(jogadorRanking[j+1].nome,jogadorRanking[j].nome);

                            }
                            jogadorRanking[i].pontuacao=jogador.pontuacao;
                            strcpy(jogadorRanking[i].nome,jogador.nome);







                            for(i=0; i<5; i++)
                            {
                                fwrite(jogadorRanking[i].nome, sizeof(char), 50, arquivoRanking);
                                fwrite(&(jogadorRanking[i].pontuacao), sizeof(int), 1, arquivoRanking);
                            }
                            rewind(arquivoRanking);


                            delayVoltarMenu=1;
                        }
                        if(delayVoltarMenu>0&&delayVoltarMenu<100)
                        {
                            delayVoltarMenu++;
                        }
                        if(delayVoltarMenu==100)//para cada uma das 3 possibilidades: jogador escolhendo ou não inserir,
                                                //e jogador não podendo inserir nome no ranking, fazemos um delay e depois recarregamos algumas variaves,
                        {                       // e então voltamos para o menu
                            flagInseNome=0;
                            flagGameOver=0;
                            flagMudaNivel=0;
                            flagFimJogo=0;

                            nivel=1;
                            contagemQueda=0;
                            flag=0;
                            modoMin=0;
                            escada=0;
                            pontuacaoNivel=1000;

                            jogador.vidas=3;
                            jogador.pontuacao=0;
                            jogador.energia=100;
                            jogador.escadas=20;

                            strcpy(jogador.ultimoItem,"ainda nao minerou");
                            FILE *matrizMapa=fopen("nivel1.txt","r");


                            matriz1[20][30];
                            int i,j,aux3[5]= {0};
                            for(i=0; i<20; i++)
                            {
                                for(j=0; j<30; j++)
                                {

                                    matriz1[i][j]=fgetc(matrizMapa);
                                }

                                fgetc(matrizMapa);
                            }
                            for(i=0; i<20; i++)
                            {
                                for(j=0; j<30; j++)
                                {

                                    if(matriz1[i][j]=='J')
                                    {
                                        jogador.x=j+1;
                                        jogador.y=i+1;
                                    }
                                }


                            }
                            aux2[100]=" ";
                            ajeitaNome=0;


                            // FILE *arquivo=fopen("ranking.bin", "rb+");
                            arquivoRanking=fopen("ranking.bin", "rb+");




                            for(i=0; i<5; i++)
                            {
                                fread(jogadorRanking[i].nome, sizeof(char), 50, arquivoRanking);
                                fread(&(jogadorRanking[i].pontuacao), sizeof(int), 1, arquivoRanking);
                            }
                            rewind(arquivoRanking);
                            playingSomInicio = 0;
                            playingSomDoJogo = 0;
                            mouseRanking=0;
                            mouseSair=0;
                            mouseNovoJogo=0;
                            voltarMenu=0;
                            delayVoltarMenu=0;
                        }




                    }


                    if(IsKeyPressed(KEY_N)&&flagInseNome==0)
                    {
                        flagSemNome=1;

                    }

                    if(delayVoltarMenu==0&&flagSemNome==1)
                    {

                        delayVoltarMenu=1;
                    }
                    if(delayVoltarMenu>0&&delayVoltarMenu<100&&flagSemNome==1)
                    {
                        delayVoltarMenu++;
                    }
                    if(delayVoltarMenu==100&&flagSemNome==1)
                    {
                        flagSemNome=0;
                        flagInseNome=0;
                        flagGameOver=0;
                        flagMudaNivel=0;
                        flagFimJogo=0;

                        nivel=1;
                        contagemQueda=0;
                        flag=0;
                        modoMin=0;
                        escada=0;
                        pontuacaoNivel=1000;

                        jogador.vidas=3;
                        jogador.pontuacao=0;
                        jogador.energia=100;
                        jogador.escadas=20;

                        strcpy(jogador.ultimoItem,"ainda nao minerou");
                        FILE *matrizMapa=fopen("nivel1.txt","r");


                        matriz1[20][30];
                        int i,j,aux3[5]= {0};
                        for(i=0; i<20; i++)
                        {
                            for(j=0; j<30; j++)
                            {

                                matriz1[i][j]=fgetc(matrizMapa);
                            }

                            fgetc(matrizMapa);
                        }
                        for(i=0; i<20; i++)
                        {
                            for(j=0; j<30; j++)
                            {

                                if(matriz1[i][j]=='J')
                                {
                                    jogador.x=j+1;
                                    jogador.y=i+1;
                                }
                            }


                        }
                        aux2[100]=" ";
                        ajeitaNome=0;


                        // FILE *arquivo=fopen("ranking.bin", "rb+");
                        FILE *arquivoRanking=fopen("ranking.bin", "rb+");




                        for(i=0; i<5; i++)
                        {
                            fread(jogadorRanking[i].nome, sizeof(char), 50, arquivoRanking);
                            fread(&(jogadorRanking[i].pontuacao), sizeof(int), 1, arquivoRanking);
                        }
                        rewind(arquivoRanking);
                        playingSomInicio = 0;
                        playingSomDoJogo = 0;
                        mouseRanking=0;
                        mouseSair=0;
                        mouseNovoJogo=0;
                        voltarMenu=0;
                        delayVoltarMenu=0;
                    }

                }
                else
                {
                    if(delayVoltarMenu==0)
                    {

                        delayVoltarMenu=1;
                    }
                    if(delayVoltarMenu>0&&delayVoltarMenu<100)
                    {
                        delayVoltarMenu++;
                    }
                    if(delayVoltarMenu==100)
                    {
                        flagInseNome=0;
                        flagGameOver=0;
                        flagMudaNivel=0;
                        flagFimJogo=0;

                        nivel=1;
                        contagemQueda=0;
                        flag=0;
                        modoMin=0;
                        escada=0;
                        pontuacaoNivel=1000;

                        jogador.vidas=3;
                        jogador.pontuacao=0;
                        jogador.energia=100;
                        jogador.escadas=20;

                        strcpy(jogador.ultimoItem,"ainda nao minerou");
                        FILE *matrizMapa=fopen("nivel1.txt","r");


                        matriz1[20][30];
                        int i,j,aux3[5]= {0};
                        for(i=0; i<20; i++)
                        {
                            for(j=0; j<30; j++)
                            {

                                matriz1[i][j]=fgetc(matrizMapa);
                            }

                            fgetc(matrizMapa);
                        }
                        for(i=0; i<20; i++)
                        {
                            for(j=0; j<30; j++)
                            {

                                if(matriz1[i][j]=='J')
                                {
                                    jogador.x=j+1;
                                    jogador.y=i+1;
                                }
                            }


                        }




                        // FILE *arquivo=fopen("ranking.bin", "rb+");
                        FILE *arquivoRanking=fopen("ranking.bin", "rb+");




                        for(i=0; i<5; i++)
                        {
                            fread(jogadorRanking[i].nome, sizeof(char), 50, arquivoRanking);
                            fread(&(jogadorRanking[i].pontuacao), sizeof(int), 1, arquivoRanking);
                        }
                        rewind(arquivoRanking);
                        playingSomInicio = 0;
                        playingSomDoJogo = 0;
                        mouseRanking=0;
                        mouseSair=0;
                        mouseNovoJogo=0;
                        voltarMenu=0;
                        delayVoltarMenu=0;
                    }


                }







            }











            EndDrawing();
        }

        // Unload the texture before closing the window
        UnloadTexture(textureCapa);
        UnloadTexture(textureRanking);
        CloseWindow();


    return 0;
}


