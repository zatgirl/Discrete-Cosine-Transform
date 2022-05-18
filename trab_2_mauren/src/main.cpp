/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         05/2020
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.0
//
//  Instruções:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/
///Explorar quantização, imagens, compressão, transformada cosseno, análise de
///complexidade.
///Aplicar DCT e IDCT
///Exibir original, reconstruida e diferença
///Desenvolva interfaces gráficas para reduzir ou aplicar frequências específicas da imagem (ou grupos de frequências)
///e reconstruir novamente, para ver qual efeito isso causa na imagem.
///blocos de 8x8 pixels, 16x16, 32x32, ... ou sobre toda imagem de uma única vez

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <boost/lexical_cast.hpp>

#include "gl_canvas2d.h"

#include "Interface.h"
#include "Botao.h"
#include "Bmp.h"
#include "Draw.h"
#include "Histograma.h"
#include "Vector2.h"
#include "func.h"
#define M_PI           3.14159265358979323846  /* pi */


Interface interface;
Histograma *histograma[3];
Bmp *bmp[3];
Img img[3];
Graph graphic[3];

//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
int screenWidth = 1176, screenHeight = 600; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
bool click = false;
int op = 0;
int DEFAULT_START_IMG_X = (screenWidth/6)*3, DEFAULT_START_IMG_Y = 20, altura[3], largura[3];

void render()
{
    interface = Interface(screenHeight,screenWidth);
    interface.View();
    graphic[0].TransfDiscretaCosseno();
    //graphic[0].IDCT();
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   //printf("\nTecla: %d" , key);
   if( key == 95 )
   {
      click == true;
   }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if( state == 0 ) //clicou
   {
       click = true;
   }

}

int main(void)
{
   //Carregamento das Imagens
   unsigned char *data[3];

   bmp[0] = new Bmp(".\\trab_2_mauren\\resources\\Ex1.bmp");
   bmp[1] = new Bmp(".\\trab_2_mauren\\resources\\Ex2.bmp");
   bmp[2] = new Bmp(".\\trab_2_mauren\\resources\\Ex3.bmp");
   for(int i = 0; i < 3; i++){
        bmp[i]->convertBGRtoRGB();
        data[i] = bmp[i]->getImage();
        altura[i] = bmp[i]->getHeight();
        largura [i] = bmp[i]->getWidth();
        img[i] = Img(bmp[i]->getHeight(), bmp[i]->getWidth(), DEFAULT_START_IMG_X, DEFAULT_START_IMG_Y, data[i]);
        graphic[i] = Graph(largura[i],altura[i],data[i]);
   }

   CV::init(&screenWidth, &screenHeight, "Transformada Discreta de Fourier");
   CV::run();
   return 0;
}
