#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#include "gl_canvas2d.h"
#include "Botao.h"

Botao *rgb = NULL;
Botao *gray = NULL;
Botao *roll = NULL;
Botao *dif = NULL;
Botao *quant = NULL;
Botao *scalMax = NULL;
Botao *scalMin = NULL;

class Interface{
    int screenWidth, screenHeight;

public:
    Interface(){
    }

    Interface(int _screenHeight, int _screenWidth){
        screenHeight = _screenHeight;
        screenWidth = _screenWidth;
    }

    void View(){
      //Painel de fundo
        CV::color(0.8705,0.8705,0.8705);
        CV::rectFill(0,0,screenWidth,screenHeight);
        //Criação dos botões
        rgb = new Botao(1*(screenWidth/6)-100,7*(screenHeight/8),120,45,"RED",1,0.2,0.2);
        rgb->Draw();
        gray = new Botao(2*(screenWidth/6)-100,7*(screenHeight/8),120,45,"GRAY",0.5,0.5,0.5);
        gray->Draw();
        roll = new Botao(3*(screenWidth/6)-100,7*(screenHeight/8),120,45,"TROCAR",0.6,0.6,1);
        roll->Draw();
        dif = new Botao(4*(screenWidth/6)-100,7*(screenHeight/8),120,45,"DIFF",0.6,0.6,1);
        dif->Draw();
        quant = new Botao(5*(screenWidth/6)-100,7*(screenHeight/8),120,45,"QUANT",0.6,0.6,1);
        quant->Draw();
        //scalMin = new Botao(2*(screenWidth/6)-100,5*(screenHeight/8),120,45,"",0.8235,0.4392,0.8823);
        //scalMin->Draw();
        //Testa se algum botão foi acionado
        Botao *botoes[] = {rgb,gray,roll,dif,quant};

    }
};

#endif
