#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "gl_canvas2d.h"
#define M_PI           3.14159265358979323846  /* pi */
#define N 8
#define M 8

class Graph{
    int largura = 0,altura = 0;
    unsigned char *pixel;
    float dctGray[32][32], dctRed[32][32], dctGreen[32][32], dctBlue[32][32];
    float idctGray[32][32], idctRed[32][32], idctGreen[32][32], idctBlue[32][32];
    float red[32][32], green[32][32], blue[32][32], grayScale[32][32];
    bool rgb = true, gray = false;

public:
    Graph(){
    }

    Graph(int _largura, int _altura, unsigned char *_pixel)
    {
        pixel = _pixel;
        altura = _altura;
        largura = _largura;
    }

    void SplitBands(){
        int idx = 0, i = 0, j = 0;
        int y = 20, x;
        CV::color(0,0,0);
        CV::text(70, 300, "Original");

        for(int linha = 0; linha < largura; linha++){
            x = 70;
            for(int coluna = 0; coluna < altura; coluna++){
                grayScale[j][i] = ((0.3*((float)(pixel[idx]))) + (0.59*((float)(pixel[idx+1])))+ (0.11*((float)(pixel[idx+2]))));
                grayScale[j][i] = (grayScale[j][i] > 255) ? 255 : grayScale[j][i];

                red[j][i] = ((float)pixel[idx]);
                green[j][i] = ((float)pixel[idx+1]);
                blue[j][i] = ((float)pixel[idx+2]);

                if (gray == true){
                    CV::color(grayScale[j][i]/255.0, grayScale[j][i]/255.0,grayScale[j][i]/255.0);
                }
                if (rgb == true){
                    CV::color(red[j][i]/255.0,green[j][i]/255.0,blue[j][i]/255.0);
                }
                CV::rectFill(coluna+x, linha+y, coluna+x+6, linha+y+6);
                idx +=3;
                x +=5;
                i ++;
            }
            y+=5;
            j++;
            i = 0;
        }
        x = 20;
        idx = 0;
        j = 0;
    }

    void DCT(bool _rgb, bool _gray){
        rgb = _rgb;
        gray = _gray;
        SplitBands();
        float const_u, const_v, tempDCT_gray, tempDCT_red, tempDCT_green, tempDCT_blue;

        CV::color(0,0,0);
        CV::text(370, 300, "DCT");

        ///Separa a imagem em blocos de 8x8
        for(int imgx = 0; imgx < 32; imgx +=8)
        {

            for(int imgy = 0; imgy < 32; imgy +=8)
            {

                for (int u = 0; u < N; u ++)
                {
                    for (int v = 0; v < M; v ++)
                    {
                        tempDCT_gray = 0;
                        tempDCT_red  = 0;
                        tempDCT_green= 0;
                        tempDCT_blue = 0;
                        for (int x = 0; x < N; x ++)
                            for (int y = 0; y < M; y ++)
                            {
                                if(gray == true) {
                                    tempDCT_gray = tempDCT_gray + grayScale[imgx+x][imgy+y] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M));
                                }
                                if(rgb == true) {
                                    tempDCT_red = tempDCT_red + red[imgx+x][imgy+y] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M));
                                    tempDCT_green = tempDCT_green + green[imgx+x][imgy+y] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M));
                                    tempDCT_blue = tempDCT_blue + blue[imgx+x][imgy+y] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M));
                                }

                            }
                        const_u = (u == 0) ? 1 / sqrt(2) : 1;
                        const_v = (v == 0) ? 1 / sqrt(2) : 1;
                        if(gray == true) {
                            dctGray[imgx+u][imgy+v]= (1/4.0) * const_u * const_v * tempDCT_gray;
                        }
                        if(rgb == true) {
                            dctRed[imgx+u][imgy+v]  = (1/4.0) * const_u * const_v * tempDCT_red;
                            dctGreen[imgx+u][imgy+v]= (1/4.0) * const_u * const_v * tempDCT_green;
                            dctBlue[imgx+u][imgy+v] = (1/4.0) * const_u * const_v * tempDCT_blue;
                        }
                    }
                }
            }
        }

        //Desenha a DCT gerada
        int iniciox, inicioy = 20;
        for(int i = 0; i < 32; i ++){
            iniciox = 370;
            for(int j = 0; j < 32; j ++){
                if(gray == true) {
                    CV::color(dctGray[i][j]/255.0, dctGray[i][j]/255.0, dctGray[i][j]/255.0);
                }
                if(rgb == true) {
                    CV::color(dctRed[i][j]/255.0, dctGreen[i][j]/255.0, dctBlue[i][j]/255.0);
                }
                CV::rectFill(iniciox+j,inicioy+i,iniciox+j+6,inicioy+i+6);
                iniciox +=5;
            } inicioy +=5;
        }

        IDCT();
    }

    void IDCT(){
        float const_u, const_v, tempIDCT_gray, tempIDCT_red, tempIDCT_green, tempIDCT_blue;
        CV::color(0,0,0);
        CV::text(640, 300, "IDCT");

        for(int imgx = 0; imgx < 32; imgx +=8)
        {
            for(int imgy = 0; imgy < 32; imgy +=8)
            {

                for (int x = 0; x < N; x ++)
                {
                    for (int y = 0; y < M; y ++)
                    {
                        tempIDCT_gray   = 0;
                        tempIDCT_red    = 0;
                        tempIDCT_green  = 0;
                        tempIDCT_blue= 0;
                        for (int u = 0; u < N; u ++)
                            for (int v = 0; v < M; v ++)
                            {
                                const_u = (u == 0) ? 1 / sqrt(2) : 1;
                                const_v = (v == 0) ? 1 / sqrt(2) : 1;

                                if(gray == true) {
                                    tempIDCT_gray = tempIDCT_gray + const_u * const_v * dctGray[imgx+u][imgy+v] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M));
                                }
                                if(rgb == true) {
                                    tempIDCT_red = tempIDCT_red + const_u * const_v * dctRed[imgx+u][imgy+v] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M));
                                    tempIDCT_green = tempIDCT_green + const_u * const_v * dctGreen[imgx+u][imgy+v] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M));
                                    tempIDCT_blue = tempIDCT_blue + const_u * const_v * dctBlue[imgx+u][imgy+v] *
                                    cos(((2.0*x+1)*u*PI)/(2.0*(float)N))*
                                    cos(((2.0*y+1)*v*PI)/(2.0*(float)M));
                                }

                            }
                        if(gray == true){
                            idctGray[imgx+x][imgy+y]= (1/4.0) * tempIDCT_gray;
                        }
                        if(rgb == true){
                            idctRed[imgx+x][imgy+y]= (1/4.0) * tempIDCT_red;
                            idctGreen[imgx+x][imgy+y]= (1/4.0) * tempIDCT_green;
                            idctBlue[imgx+x][imgy+y]= (1/4.0) * tempIDCT_blue;
                        }

                    }
                }
            }
        }

        int iniciox, inicioy = 20;
        for(int i = 0; i < 32; i ++){
            iniciox = 640;
            for(int j = 0; j < 32; j ++){
                if(gray == true) {
                    CV::color(idctGray[i][j]/255.0, idctGray[i][j]/255.0, idctGray[i][j]/255.0);
                }
                if(rgb == true) {
                    CV::color(idctRed[i][j]/255.0, idctGreen[i][j]/255.0, idctBlue[i][j]/255.0);
                }
                CV::rectFill(iniciox+j,inicioy+i,iniciox+j+6,inicioy+i+6 );
                iniciox +=5;
            } inicioy +=5;
        }

    }


};
#endif

