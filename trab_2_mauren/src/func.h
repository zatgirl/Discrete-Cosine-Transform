#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "gl_canvas2d.h"
#define M_PI           3.14159265358979323846  /* pi */

class Graph{
    int largura,altura;
    unsigned char *pixel;
    float dct[256][256]; ///Guarda a matriz final
    float idct[64][64]; ///Guarda a matriz final
    float redd[256][256], greenn[256][256], bluee[256][256], grayy[64][64];

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
                grayy[j][i] = ((0.3*((float)(pixel[idx]))) + (0.59*((float)(pixel[idx+1])))+ (0.11*((float)(pixel[idx+2]))));
                CV::color(grayy[j][i]/255.0, grayy[j][i]/255.0,grayy[j][i]/255.0);
                //grayy[j][i] = ((0.3*((float)(pixel[idx]))) + (0.59*((float)(pixel[idx+1])))+ (0.11*((float)(pixel[idx+2]))))-128;
                redd[j][i] = ((float)pixel[idx]);
                greenn[j][i] = ((float)pixel[idx+1]);
                bluee[j][i] = ((float)pixel[idx+2]);
                idx +=3;
                CV::rectFill(coluna+x, linha+y, coluna+x+2, linha+y+2);
                x+=0.5; i++;
            }   y+=0.5; j++; i = 0;
        } x = 20; idx = 0;  j = 0;

    }

    void TransfDiscretaCosseno(){
        SplitBands();
        float minv=255,maxv=-255;
        int N = 8, M = 8, blocot=0; ///Blocos de 8x8
        float const_u, const_v, tempDCT;

        CV::color(0,0,0);
        CV::text(370, 300, "Transformada");

        ///Separa a imagem em blocos de 8x8
        for(int imgx = 0; imgx < 256; imgx +=8){
            for(int imgy = 0; imgy < 256; imgy +=8){
                //printf("bloco: %d, imgx %d, imgy %d\n",blocot,imgx, imgy);
                ///Aplicação da transformada
                for(int u = 0; u < M; u ++){
                    for(int v = 0; v < N; v ++){
                         ///const_u e const_v dependem da frequência
                        const_u = (u == 0) ? 1 / sqrt(M) : sqrt(2) / sqrt(M);
                        const_v = (v == 0) ? 1 / sqrt(N) : sqrt(2) / sqrt(N);
                        tempDCT = 0;
                        for(int x = 0; x < M; x ++){
                            for(int y = 0; y < N; y ++){
                                tempDCT += grayy[imgx+x][imgy+y]*
                                                  cos(((2 * x + 1) * M_PI * u) / 16.0) *
                                                  cos(((2 * y + 1) * M_PI * v) / 16.0);
                                                  //printf("grayy[%d][%d]: %.2f\n",imgx+x,imgy+y,grayy[imgx+x][imgy+y]);
                            }                     //dct[u][v] = (const_u * const_v * (tempDCT/4.0))/10;
                        }//printf("tempDCT: %.2f\n",tempDCT);
                        dct[imgx+u][imgy+v] = (const_u * const_v *((tempDCT/4.0)+0.5)*127);
                        //printf("tempDCT: %.2f | dct: %.2f\n",tempDCT, dct[u][v]/4.0);
                            minv = (minv > dct[imgx+u][imgy+v]) ? dct[imgx+u][imgy+v] : minv;
                            maxv = (maxv < dct[imgx+u][imgy+v]) ? dct[imgx+u][imgy+v] : maxv;
                    }
                }
            }
        }
        //printf("max: %.2f | min: %.2f \n",maxv,minv);
        int iniciox, inicioy = 20;
        for(int i = 0; i < 256; i ++){
            iniciox = 370;
            for(int j = 0; j < 256; j ++){
                //printf("dct[%d][%d]: %.2f\n",i,j,dct[i][j]/255.0);
                CV::color(dct[i][j]/255.0, dct[i][j]/255.0, dct[i][j]/255.0);
                CV::rectFill(iniciox+j,inicioy+i,iniciox+j+2,inicioy+i+2);
                iniciox +=0.5;
            } inicioy +=0.5;
        }
    }

    void IDCT(){
        int N = 64, M = 64, blocot=0; ///Blocos de 8x8
        float const_u, const_v, tempIDCT;
        ///Aplicação da transformada
        for(int imgx = 0; imgx < 64; imgx +=8){
            for(int imgy = 0; imgy < 64; imgy +=8){

                //printf("bloco: %d, imgx %d, imgy %d\n",blocot,imgx, imgy);
                ///Aplicação da transformada
                for(int u = 0; u < M; u ++){
                    for(int v = 0; v < N; v ++){
                         ///const_u e const_v dependem da frequência
                        const_u = (u == 0) ? 1 / sqrt(M) : sqrt(2) / sqrt(M);
                        const_v = (v == 0) ? 1 / sqrt(N) : sqrt(2) / sqrt(N);
                        tempIDCT = 0;
                        for(int x = 0; x < M; x ++){
                            for(int y = 0; y < N; y ++){
                                tempIDCT += dct[imgx+x][imgy+y]*
                                                  cos(((2 * x + 1) * M_PI * u) / 16.0) *
                                                  cos(((2 * y + 1) * M_PI * v) / 16.0);
                            }
                        }
                        idct[imgx+u][imgy+v] = ( const_u * const_v *(tempIDCT/4.0));
                    }
                }

            }
        }
        int iniciox, inicioy = 20;
        for(int i = 0; i < 64; i ++){
            iniciox = 370;
            for(int j = 0; j < 64; j ++){
                //printf("dct[%d][%d]: %.2f\n",i,j,dct[i][j]/255.0);
                CV::color(idct[i][j]/255.0, idct[i][j]/255.0, idct[i][j]/255.0);
                CV::rectFill(iniciox+j,inicioy+i,iniciox+j+2,inicioy+i+2);
                iniciox ++;
            } inicioy ++;
        }





    }


};
#endif

