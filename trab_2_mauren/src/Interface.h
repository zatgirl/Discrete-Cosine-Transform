#ifndef __INTERFACE_H__
#define __INTERFACE_H__
#include "gl_canvas2d.h"
#include "Botao.h"
/*
Botao *orig = NULL;
Botao *cr = NULL;
Botao *cg = NULL;
Botao *cb = NULL;
Botao *gray = NULL;
Botao *hist = NULL;
Botao *rotClock = NULL;
Botao *rotCounter = NULL;
Botao *roll = NULL;
Botao *scalMax = NULL;
Botao *scalMin = NULL;

class Interface2{
    int screenWidth, screenHeight;

public:
    Interface2(){
    }

    Interface2(int _screenHeight, int _screenWidth){
        screenHeight = _screenHeight;
        screenWidth = _screenWidth;
    }

    void interface3(){
      //Painel de fundo
        CV::color(0.8705,0.8705,0.8705);
        CV::rectFill(0,0,screenWidth,screenHeight);
        //Criação dos botões
        cr = new Botao(1*(screenWidth/6)-100,7*(screenHeight/8),120,45,"RED",1,0,0);
        cr->Draw();
        cg = new Botao(2*(screenWidth/6)-100,7*(screenHeight/8),120,45,"GREEN",0,1,0);
        cg->Draw();
        cb = new Botao(3*(screenWidth/6)-100,7*(screenHeight/8),120,45,"BLUE",0.4078,0.4705,0.8627);
        cb->Draw();
        gray = new Botao(4*(screenWidth/6)-100,7*(screenHeight/8),120,45,"GRAY",0.5,0.5,0.5);
        gray->Draw();
        roll = new Botao(3*(screenWidth/6)-100,6*(screenHeight/8),120,45,"TROCAR",1,0,1);
        roll->Draw();
        rotClock = new Botao(1*(screenWidth/6)-100,6*(screenHeight/8),120,45,"GIRAR <",1,0.5,0);
        rotClock->Draw();
        rotCounter = new Botao(1*(screenWidth/6)-100,5*(screenHeight/8),120,45,"GIRAR >",1,0.5,0);
        rotCounter->Draw();
        orig = new Botao(5*(screenWidth/6)-100,7*(screenHeight/8),120,45,"ORIGINAL",0.5,0.5,0);
        orig->Draw();
        scalMax = new Botao(2*(screenWidth/6)-100,6*(screenHeight/8),120,45,"ESCALAR+",0.8235,0.4392,0.8823);
        scalMax->Draw();
        scalMin = new Botao(2*(screenWidth/6)-100,5*(screenHeight/8),120,45,"ESCALAR-",0.8235,0.4392,0.8823);
        scalMin->Draw();
        //Testa se algum botão foi acionado
        Botao *botoes[] = {cr,cg,cb,gray,orig,rotClock,rotCounter,scalMax,scalMin};
    }
};*/

#endif
