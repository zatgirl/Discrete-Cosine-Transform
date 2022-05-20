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
///Implementações:
///-Aplicação da DCT sobre as imagens;
///-Aplicação da Transformada Inversa (IDCT) para reconstrução das imagens;
///-Cálculo da diferença da imagem original e a imagem reconstruída;
///-Quantização;
///-Histograma da dif


#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include "Botao.h"
#include "Bmp.h"
#include "Histograma.h"
#include "Vector2.h"
#include "func.h"
#define M_PI           3.14159265358979323846

Botao *rgb = NULL;
Botao *gray = NULL;
Botao *roll = NULL;
Botao *dif = NULL;
Botao *quant = NULL;
Botao *scalMax = NULL;
Botao *scalMin = NULL;

Histograma *histograma[3];
Bmp *bmp[4];
Graph graphic[4];

int opcao  = 50;
int screenWidth = 1176, screenHeight = 600;
int mouseX, mouseY;
bool click = false, load = false;
int op = 0, select = 1;
int altura[3], largura[3];



void interface(){
    //Painel de fundo
        CV::color(0.8705,0.8705,0.8705);
        CV::rectFill(0,0,screenWidth,screenHeight);
    //Criação dos botões
        rgb = new Botao(2*(screenWidth/6)-100,7*(screenHeight/8),120,45,"RGB",1,1,0.45);
        rgb->Draw();
        gray = new Botao(3*(screenWidth/6)-100,7*(screenHeight/8),120,45,"GRAY",0.5,0.5,0.5);
        gray->Draw();
        roll = new Botao(4*(screenWidth/6)-100,7*(screenHeight/8),120,45,"TROCAR",0.6,0.6,1);
        roll->Draw();
        quant = new Botao(5*(screenWidth/6)-100,7*(screenHeight/8),120,45,"QUANT",0.6,0.6,1);
        quant->Draw();
    //Testa se algum botão foi acionado
        Botao *botoes[] = {rgb,gray,roll,quant};
        if(click == true){
            for(int i = 0; i<2; i++){
                if(botoes[i]->Colidiu(mouseX, mouseY)){
                    select = i+1;
                }
            }
            if(roll->Colidiu(mouseX, mouseY)==true){
                if (op != 4){
                    op ++; click = false;
                }
                if (op == 4){
                    op = 0; click = false;
                }
            }
            if(quant->Colidiu(mouseX, mouseY)==true){
                graphic[op].quantChoice(true);
            }
            click = false;
        }
        switch(select){
            case 1: graphic[op].ViewGraphs(); graphic[op].RGBChoice(true); break;
            case 2: graphic[op].ViewGraphs(); graphic[op].GrayChoice(true);break;
            case 5: graphic[op].quantChoice(true);  break;
        }


}

void render()
{
    interface();
}

void keyboard(int key)
{
}

void keyboardUp(int key)
{
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   mouseX = x;
   mouseY = y;

   if( state == 0 )
   {
       click = true;
   }

}

int main(void)
{
   //Carregamento das Imagens
   unsigned char *data[4];

   bmp[0] = new Bmp(".\\trab_2_mauren\\resources\\Lena256x256.bmp");
   bmp[1] = new Bmp(".\\trab_2_mauren\\resources\\Ex2.bmp");
   bmp[2] = new Bmp(".\\trab_2_mauren\\resources\\Ex3.bmp");
   bmp[3] = new Bmp(".\\trab_2_mauren\\resources\\Lena64x64.bmp");
   for(int i = 0; i < 4; i++){
        bmp[i]->convertBGRtoRGB();
        data[i] = bmp[i]->getImage();
        altura[i] = bmp[i]->getHeight();
        largura [i] = bmp[i]->getWidth();
        graphic[i] = Graph(largura[i],altura[i],data[i]);
        graphic[i].Load();
   }



   CV::init(&screenWidth, &screenHeight, "Transformada Discreta de Fourier");
   CV::run();
   return 0;
}
