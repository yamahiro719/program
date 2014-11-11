
#include <stdio.h>
#include <stdlib.h>
#include <eggx.h>
#include <time.h>

#define yoko 500
#define tate 500

/*グローバル変数*/
int win;
int x[10],y[10],w[10],color[10];//背景
int a,b;//扇形の中心
double s;//回転速度
int code,type;//ggetexpress

/*プロトタイプ宣言*/
void result();
void back();
void draw();

/*******************************
 スタート画面
 ********************************/
int main(){
  int i;//カウンタ(背景)
  double g,h;//クリック時のx,y
  int l;//カウンタ(文字点滅)
    
    gsetnonblock(ENABLE);//ノンブロッキングモード
    win=gopen(yoko,tate);//500*500のウィンドウ表示
    winname(win,"final");
    gsetbgcolor(win,"Black");//背景色
    gsetfontset(win,"-*-helvetica-medium-r-normal--24-*-*-*-*-*-iso8859-1, "
                "-jis-fixed-medium-r-normal--24-*-*-*-*-*-jisx0208.1983-0");//フォントセット
    gclr(win);//画面消去
    l=0;//初期化
    srand(time(NULL));//ランダム
    
    for(i=0;i<10;i++){
        x[i]= rand() % 500;//x座標位置をランダム
        y[i]=-500 + rand() % 500;//-500することで自然にみせる
        w[i]= rand() % 10;//円の大きさ
    }
    
    
    while(1){
      newpen(win,1);//白
      drawstr(win, 50.0, 350.0, FONTSET, 0.0, "的が一周するまでの間に\n的をクリックすることで\nSCOREが+50され、\n表示速度も上昇します。\n1000でクリア！");//文字の表示

	/*文字の点滅*/
      if(l==0){
	drawstr(win, 50.0, 50.0,FONTSET, 0.0,"クリックしてスタート");
	msleep(500);
	l=1;
      }else{
	newpen(win,0);//背景色で少しの間上書きする。
	drawstr(win, 50.0, 50.0,FONTSET, 0.0,"クリックしてスタート");
	msleep(500);
	l=0;
      }
        /*クリックでスタートさせる*/
      if(ggetxpress(&type,&code,&g,&h)==win){
	if(type==ButtonPress){
	  if(code==1){
	    draw();//draw呼び出し
	  }
	}
      }
    }
}


/*******************************
クリア画面
 *******************************/
void result(){
  int y;//文字のy座標
  double g,h;//クリック時のx,y
  
  gclr(win);//画面消去
  y=500;
  while(1){
    gclr(win);
    drawstr(win, 200.0, y, 48, 0.0, "Clear!!!");//文字の表示
    y--;//下へ
    msleep(10);
    if(y==250){//真ん中にきたとき
      y++;//止める
      if(ggetxpress(&type,&code,&g,&h)==win){
	if(type==ButtonPress){
	  if(code==1){
	    exit(0);//クリックで終了
	  }
	}
      }
    }
  }
}

/*******************************
背景
 ********************************/
void back(){
  int i;//カウンタ
    
    for(i=0;i<10;i++){
        color[i] = i + 1;
        newpen(win,color[i]);//色
        fillcirc(win, x[i], y[i], w[i], w[i]);//円を描く
        y[i]+= w[i] / 2.0;//円の大きさずつ上へ移動
        
        if(y[i]>550){//550にきたら
            x[i]=rand() % 500;//x座標位置の再設定
            y[i]=0;//y座標位置を下に
        }
        
    }
    /*こっちに扇形の描画を持ってくることで扇形が点滅することを防ぐ*/
    newrgbcolor(win, 243, 152, 0);//扇形の色
    fillarc(win,a,b,60.0,60.0,91.0,s,-1);//扇形の表示
    msleep(10);//少し待つ
    
}



/*******************************
 ゲーム
 ********************************/
void draw(){
  double x,y;//クリック時の座標
  double c;//円の角度
  int score;//スコア
  int r;//距離
    
  score=0;//スコア初期値
    
  /*円の初期設定*/
  s = 90.0;
  a = 60 + rand()%381;
  b = 60 + rand()%381;

  c=1.0;//1.0倍速度
    
    while(1){
        gclr(win); //画面初期化
        back(); //背景描画
        
        /*的を描画*/
            newpen(win,1);//文字の色
            drawstr(win, 0.0, 0.0, FONTSET, 0.0, "SCORE:%d",score);//スコア表示
	    /*クリック判定*/
            if(ggetxpress(&type,&code,&x,&y)==win){
                if(type==ButtonPress){
                    if(code==1){
                        if(x>=a && y>=b){//第一象限
                            r=(x-a)*(x-a)+(y-b)*(y-b);
                        }else if(x<=a && y>=b){//第二象限
                            r=(a-x)*(a-x)+(y-b)*(y-b);
                        }else if(x<=a && y<=b){//第三象限
                            r=(a-x)*(a-x)+(b-y)*(b-y);
                        }else if(x<=a && y<=b){//第四象限
                            r=(x-a)*(x-a)+(b-y)*(b-y);
                        }
                        if(r<=3600) {
                            score+=50;//スコアを増やしていく
			    /*中心と角度の再設定*/
			    a = 60 + rand()%381;
			    b = 60 + rand()%381;
			    s=90.0;
			    
			    c+=0.1;//0.1ずつ速度を早くする
			    if(score==100){//1000になったら
      			      result();//クリア画面
			    }
                        }
                    }
                }
            }
            
            s-=10.0*c;//回転速度
            
            if (s<=-270.0) { //sが-270に到達すると次の円へ
                a = 60 + rand()%381;
                b = 60 + rand()%381;
                s = 90.0;
            }
            
            msleep(50);
    }
}

