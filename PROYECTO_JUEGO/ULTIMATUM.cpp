#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>

using namespace std;

#define ARRIBA 1
#define ABAJO 2
#define IZQUIERDA 3
#define DERECHA 4
#define TAM 36
#define ANCHO 1725
#define ALTO 720
#define OPCION_INICIAR_JUEGO 1
#define SIN_ACCION 0
#define PIX 35
const int COL = 49, REN = 20;
const int NIVEL2=3,NIVEL3=3;
const int TIEMPO=80000;
const int ENEA=1700, ENEAL=650;

ALLEGRO_TIMER* timer = NULL;
ALLEGRO_EVENT_QUEUE* cola = NULL;
ALLEGRO_DISPLAY* pantalla = NULL;
ALLEGRO_FONT* font = NULL;
ALLEGRO_EVENT event;

//funciones de allegro
int init();
void deinit();
void mostrarFinal();

//fucniones 
class Nivel1{
public:
    ALLEGRO_BITMAP* mapaN1P;
    ALLEGRO_BITMAP* puntosN1;
    ALLEGRO_BITMAP* decoroN1;
    ALLEGRO_BITMAP* mabel;
    ALLEGRO_BITMAP* fondoN1;
    ALLEGRO_BITMAP* modulo;
    ALLEGRO_BITMAP* personaje;
    ALLEGRO_BITMAP* enemigoN1;

    //variables de pintar
    int punt,vidas,tiempo;
    //variables del personaje 
	int jMat,iMat,kMat,hMat,direccion;
    int xPer,yPer,vXPer,vYPer;
    double dt;
    
    //variables del enemigo
    int xE,yE,vxE,vyE;
    double dtE;
    bool colision;
    
    //variable del mapa
	char mapaN1[REN][COL]={
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
	"X                                              X",
	"XXXXXXXXXXX      . . . . . . .      XXXXXXXXXXXX",
	"X               XXXXXXXXXXXXXXX                X",
	"X . . . . . .         XXX         . . . . . .  X",
	"X                      X                       X",
	"X         XXXXXXXXXXX  .  XXXXXXXXXXX          X",
	"XXXXXXX   X. . . . .X  .  X. . . . .X    XXXXXXX",
	"XXXXXXX   X         X  .  X         X    XXXXXXX",
	"X. . .    X.       .X  .  X.       .X     . . .X",
	"X         XXXX   XXXX  .  XXXX   XXXX          X",
	"X                                              X",
	"X    . . .            XX             . . .     X",
	"XXXXXXXXXXXXXXX   . . XX . .     XXXXXXXXXXXXXXX",
	"X . . . . . . X    XXXXXXXX      X . . . . . . X",
	"X             X   . . XX . .     X             X",
	"X    XXXX     X       XX         X     XXXX    X",
	"X                                              X",
	"X . . . . . .      . . . . .       . . . . . . X",
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"};
public:
	
	bool partida;
    
    Nivel1() : partida(false){
        mapaN1P = al_load_bitmap("paredN1.png");
        puntosN1 = al_load_bitmap("puntosN1.png");
        decoroN1= al_load_bitmap("decoro2.png");
        mabel = al_load_bitmap("dipper.png");
        fondoN1 = al_load_bitmap("fondoN1.jpg");
        personaje = al_load_bitmap ("dipperID.png");
        enemigoN1= al_load_bitmap("fantasma.png");
        
        //puntuacion 
        punt = 0;
        vidas=3;
        tiempo=10000;
        //movimiento entra casillas
        jMat= 0;
        iMat = 0;
        kMat=0;
        //direccion del personaje
        direccion = 0;
        //posicion del personaje en el mapa
        xPer= 50;
        yPer= 36;
        //velocidad del personaje en el mapa
        vXPer= 190;
        vYPer= 190;
        //calculo del tiempo
        dt = 0;
        //enemigo
        xE=rand()%1725;
		yE=rand()%720;
		vxE=rand()%120;
		vyE=rand()%120;
		colision=false;
        
        if (!mapaN1P || !puntosN1) {
            cout << "Error al cargar imágenes." << endl;
        }
    }
    
    void iniciarPartida() {
    	partida = true;
	}
	void enemigo(double dt);
    virtual void pintar();   //poliformismo
    void fisicaPersonaje(double dt,int direccion);
    bool colisionEnemigo(int xPer,int yPer,int xE,int yE);
    void reinicio();
};

void Nivel1::enemigo(double dt){
	xE += vxE*dt;
	yE += vyE*dt;
	if((vxE < 0 && xE<=0) || (vxE > 0 && xE >= 1700)){
		vxE = -vxE;
	}//mov de columna
	if((vyE < 0 && yE <= 0) || (vyE > 0 && yE >=650)){
		vyE = -vyE;
	}
}

bool Nivel1::colisionEnemigo(int xPer,int yPer,int xE,int yE){
    //se verifica la colisi�n entre el enemigo y el personaje por medio de su tama�o
    if (xPer < xE+ PIX && xPer + PIX > xE && yPer < yE + PIX && yPer + PIX > yE){
    	// se detecta la colisi�n
        return true;
    }else{
        return false;
    }
}//bool


void Nivel1::pintar() {
	
	char mensaje[50];
    al_clear_to_color(al_map_rgb(0, 0, 0));
    //se pinta el mapa 
    al_draw_bitmap(fondoN1, 0, 0, 0);
    al_draw_bitmap(fondoN1,1000,0,0);
   
    
    //imagen del mapa
    for (int j = 0; j < REN; j++) {
        for (int i = 0; i < COL; i++) {
            if (mapaN1[j][i] == 'X') {
                al_draw_bitmap(mapaN1P, i * TAM, j * TAM, 0);
            } else if (mapaN1[j][i] == '.') {
                al_draw_bitmap(puntosN1, i * TAM, j * TAM, 0);
            }
        }
    }
    
    //MENSAJES-----------------------------------------------
    //datos del jugador
    sprintf(mensaje, "PUNTUACION DIPPER: %d",punt);
    al_draw_text(font, al_map_rgb(0,255,255),200,723, 0, mensaje);
    
    sprintf(mensaje, "VIDAS DE DIPPER: %d",vidas);
    al_draw_text(font, al_map_rgb(255, 0, 256),200,750, 0, mensaje);
    
    sprintf(mensaje, "MOVIMIENTO %d-%d, dt=%f", yPer,xPer,dt);
    al_draw_text(font, al_map_rgb(255, 255, 0),200,780, 0, mensaje);
    
    //nivel
    sprintf(mensaje, "NIVEL 1 - NOMO EN DESQUICIO ", yPer,xPer,dt);
    al_draw_text(font, al_map_rgb(255, 0, 255),200,810, 0, mensaje);
    
    //nivel
    sprintf(mensaje, "TIEMPO RESTANTE: %d",tiempo--,"segundos");
    al_draw_text(font, al_map_rgb(255, 0, 255),400,723, 0, mensaje);
    
        //se pinta el modulo de las vidas
    al_draw_bitmap(personaje,0,723,0);
     
    //imagen del jugador
    al_draw_bitmap(mabel, xPer, yPer, 0);
    
    //enemigo
    al_draw_bitmap(enemigoN1, xE, yE, 0);
    
    //se manda todo a imprimir
    al_flip_display();
}

void Nivel1::reinicio(){
	//se regresa a la posicion inicial
    xPer= 50;
    yPer= 50;
    //se remplazan de nuevo las recompensas
    for (int j = 0; j < REN; j++) {
        for (int i = 0; i < COL; i++) {
            if (mapaN1[j][i] == '-') {
            	mapaN1[j][i]='.';
            }
        }
    }

}


void Nivel1::fisicaPersonaje(double dt,int direccion){

	if(direccion == DERECHA){
		jMat=xPer/TAM;
		iMat=yPer/TAM;
		kMat = (yPer+35)/TAM;
		
		if(kMat == COL){
			kMat--;
		}
		
		//movimiento del pacman
		xPer += vXPer * dt;
		if(xPer > ANCHO - 35){
			xPer= ANCHO - 35;
		}
		
		//imposicion de limites
		if(mapaN1[iMat][jMat+1] == 'X'){
			xPer = jMat *TAM;
		}
		
		if(mapaN1[kMat][jMat+1] == 'X'){
			xPer=jMat*TAM;
		}
		
		//recompensa 
		if(mapaN1[kMat][jMat+1] =='.'){
			//xPer=jMat*TAM;
			mapaN1[kMat][jMat+1] = '-';
			punt++;
		}
		
		if(mapaN1[iMat][jMat+1] =='.'){
			mapaN1[iMat][jMat+1] = '-';
			punt++;
		}
	}//movimiento hacia la derecha
	
	if(direccion == IZQUIERDA){
		jMat=(xPer+35)/TAM; //tamaño por casilla
		iMat=(yPer+35)/TAM;
		kMat=yPer/TAM;
		
		if(jMat==REN){ //
			jMat--;
		}
		if(iMat==COL){  //
			iMat--;
		}
		
		//mov pacman 
		xPer -= vXPer * dt;
		if(xPer<0){
			xPer=0;
		}
		
		if(mapaN1[iMat][jMat-1]=='X'){
			xPer=jMat*TAM;
		}
		
		if(mapaN1[kMat][jMat-1]=='X'){
			xPer=jMat*TAM;
		}
		
		//recompensa
		if(mapaN1[iMat][jMat-1]=='.'){
			mapaN1[iMat][jMat-1]='-';
			punt++;
		}
		
		if(mapaN1[kMat][jMat-1]=='.'){
			mapaN1[kMat][jMat-1]='-';
			punt++;
		}
	}//moviemiento a la izquierda
	
	if(direccion == ARRIBA){
		jMat=(xPer+35)/TAM; //tamaño por casilla
		iMat=(yPer+35)/TAM;
		hMat=xPer/TAM;
		
		if(jMat==REN){
			jMat--;
		}
		if(iMat==COL){
			iMat--;
		}
		
		yPer -= vYPer * dt;
		if(yPer<0){
			yPer=0;
		}
		
		if(mapaN1[iMat-1][jMat]=='X'){
			yPer=iMat*TAM;
		}
		
		if(mapaN1[iMat-1][hMat]=='X'){
			yPer=iMat*TAM;
		}
		
		//recompensa
		if(mapaN1[iMat-1][jMat]=='.'){
			mapaN1[iMat-1][jMat]='-';
			punt++;
		}
		
		if(mapaN1[iMat-1][hMat]=='.'){
			mapaN1[iMat-1][hMat]='-';
			punt++;
		}
	}//movimeinto hacia arriba
	
	//movimiento hacia abajo
	if(direccion == ABAJO){
		jMat=xPer/TAM;
		iMat=yPer/TAM;
		hMat=(xPer+35)/TAM;
		
		if(hMat==REN){
			hMat--;
		}
		
		//mov pacman
		yPer+= vYPer * dt;
		if(yPer>ALTO-35){
			yPer=ALTO-35;
		}
		
		//matriz
		if(mapaN1[iMat+1][jMat]=='X'){
			yPer=iMat*TAM;
		}
		if(mapaN1[iMat+1][hMat]=='X'){
			yPer=iMat*TAM;
		}
		
		//recompensa
		if(mapaN1[iMat+1][jMat]=='.'){
		    mapaN1[iMat+1][jMat]='-';
			punt++;	
		}
		if(mapaN1[iMat+1][hMat]=='.'){
			mapaN1[iMat+1][hMat]='-';
			punt++;
		}
	}
	if(colisionEnemigo(xPer,yPer,xE,yE)==true){
		if(!colision){
			vidas--;
			colision=true;
		}
	}else{
		colision=false;
	}
}

//CLASE DE NIVEL 2
class Nivel2 : public Nivel1 {
    public:
    	ALLEGRO_BITMAP* enemigoN;
    	ALLEGRO_BITMAP* enemigoN2;
    	ALLEGRO_BITMAP* enemigoN3;
    	
    	int xE1,xE2;
    	int yE1,yE2;
    	int vxE1,vxE2;
    	int vyE1,vyE2;
    	char mapaN2[REN][COL]={
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
	"X                                              X",
	"XXXXXXXXXXX      . . . . . . .      XXXXXXXXXXXX",
	"X               XXXXXXXXXXXXXXX                X",
	"X   X.   .X     X . . . . . . X     X.   . X   X",
	"X   X. . .X     X .         . X     X. . . X   X",
	"X   XXXXXXX     X . .     . . X     XXXXXXXX   X",
	"X               XXXXX  .  XXXXX                X",
	"X . . . . . .       X  .  X        . . . . . . X",
	"XXXXXXXXXXXXX      .X  .  X.       XXXXXXXXXXXXX",
	"X . . . . . X          .           X . . . . . X",
	"X .                                          . X",
	"X . . . . .     XXXXXXXXXXXXXXX      . . . . . X",
	"XXXXXXXXXXXX      . . . . . .      XXXXXXXXXXXXX",
	"X . . . . . .                      . . . . . . X",
	"X                 X . . . . X                  X",
	"X    XXXX    XX   XXXXXXXXXXX   XX      XXXX   X",
	"X            XX                 XX             X",
	"X . . . . . .XX   . . . . . .   XX. . . . . . .X",
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"};
    	
	Nivel2(){
		
		//punteros
		mapaN1P = al_load_bitmap("3paredN.png");
        puntosN1 = al_load_bitmap("recomMabel.png");
        decoroN1= al_load_bitmap("decoro2.png");
        mabel = al_load_bitmap("mabel.png");
        fondoN1 = al_load_bitmap("fondoN2.jpg");
        personaje = al_load_bitmap ("Imagen3.jpg");
        enemigoN1= al_load_bitmap("decoroN1.png");
		enemigoN2= al_load_bitmap("decoroN1.png");
		enemigoN3= al_load_bitmap("decoroN1.png");
		
		//enemigo
        xE=rand()%1700;
		yE=rand()%720;
		vxE=rand()%500;
		vyE=rand()%500;
		//enemigo
		xE1=rand()%1700;
		yE1=rand()%720;
		vxE1=rand()%500;
		vyE1=rand()%500;
		//SEGUNDO ENEMIGO
		xE2=rand()%1700;
		yE2=rand()%720;
		vxE2=rand()%500;
		vyE2=rand()%500;
		//matriz;
		jMat= 0;
        iMat = 0;
        kMat=0;
        punt=0;

    }
    void enemigosN2(double dt);
    virtual void pintar();
    void enemigosN3(double dt);
    void fisicaPersonajeN2(double dt,int direccion);
    bool colisionEnemigoN2(int xPer,int yPer,int xE,int yE);
    bool colisionEnemigoN3(int xE1,int yE1,int xE2,int yE2);
    bool colisionEnemigoN4(int xE1,int yE1,int xE2,int yE2);
    //bool Nivel1::colisionEnemigo(int xPer,int yPer,int xE,int yE){
};

bool Nivel2::colisionEnemigoN2(int xPer,int yPer,int xE,int yE){
    //se verifica la colisi�n entre el enemigo y el personaje por medio de su tama�o
    if (xPer < xE+ PIX && xPer + PIX > xE && yPer < yE + PIX && yPer + PIX > yE){
    	// se detecta la colisi�n
        return true;
    }else{
        return false;
    }
    
}//bool

bool Nivel2::colisionEnemigoN3(int xPer,int yPer,int xE,int yE){

    if (xPer < xE1+ PIX && xPer + PIX > xE1 && yPer < yE1 + PIX && yPer + PIX > yE1){
    	// se detecta la colisi�n
        return true;
    }else{
        return false;
    }
    
}//bool

bool Nivel2::colisionEnemigoN4(int xPer,int yPer,int xE,int yE){
    if (xPer < xE2+ PIX && xPer + PIX > xE2 && yPer < yE2 + PIX && yPer + PIX > yE2){
    	// se detecta la colisi�n
        return true;
    }else{
        return false;
    }
    
}//bool



void Nivel2::fisicaPersonajeN2(double dt,int direccion){

	if(direccion == DERECHA){
		jMat=xPer/TAM;
		iMat=yPer/TAM;
		kMat = (yPer+35)/TAM;
		
		if(kMat == COL){
			kMat--;
		}
		
		//movimiento del pacman
		xPer += vXPer * dt;
		if(xPer > ANCHO - 35){
			xPer= ANCHO - 35;
		}
		
		//imposicion de limites
		if(mapaN2[iMat][jMat+1] == 'X'){
			xPer = jMat *TAM;
		}
		
		if(mapaN2[kMat][jMat+1] == 'X'){
			xPer=jMat*TAM;
		}
		
		//recompensa 
		if(mapaN2[kMat][jMat+1] =='.'){
			//xPer=jMat*TAM;
			mapaN2[kMat][jMat+1] = '-';
			punt++;
		}
		
		if(mapaN2[iMat][jMat+1] =='.'){
			mapaN2[iMat][jMat+1] = '-';
			punt++;
		}
	}//movimiento hacia la derecha
	
	if(direccion == IZQUIERDA){
		jMat=(xPer+35)/TAM; //tamaño por casilla
		iMat=(yPer+35)/TAM;
		kMat=yPer/TAM;
		
		if(jMat==REN){ //
			jMat--;
		}
		if(iMat==COL){  //
			iMat--;
		}
		
		//mov pacman 
		xPer -= vXPer * dt;
		if(xPer<0){
			xPer=0;
		}
		
		if(mapaN2[iMat][jMat-1]=='X'){
			xPer=jMat*TAM;
		}
		
		if(mapaN2[kMat][jMat-1]=='X'){
			xPer=jMat*TAM;
		}
		
		//recompensa
		if(mapaN2[iMat][jMat-1]=='.'){
			mapaN2[iMat][jMat-1]='-';
			punt++;
		}
		
		if(mapaN2[kMat][jMat-1]=='.'){
			mapaN2[kMat][jMat-1]='-';
			punt++;
		}
	}//moviemiento a la izquierda
	
	if(direccion == ARRIBA){
		jMat=(xPer+35)/TAM; //tamaño por casilla
		iMat=(yPer+35)/TAM;
		hMat=xPer/TAM;
		
		if(jMat==REN){
			jMat--;
		}
		if(iMat==COL){
			iMat--;
		}
		
		yPer -= vYPer * dt;
		if(yPer<0){
			yPer=0;
		}
		
		if(mapaN2[iMat-1][jMat]=='X'){
			yPer=iMat*TAM;
		}
		
		if(mapaN2[iMat-1][hMat]=='X'){
			yPer=iMat*TAM;
		}
		
		//recompensa
		if(mapaN2[iMat-1][jMat]=='.'){
			mapaN2[iMat-1][jMat]='-';
			punt++;
		}
		
		if(mapaN2[iMat-1][hMat]=='.'){
			mapaN2[iMat-1][hMat]='-';
			punt++;
		}
	}//movimeinto hacia arriba
	
	//movimiento hacia abajo
	if(direccion == ABAJO){
		jMat=xPer/TAM;
		iMat=yPer/TAM;
		hMat=(xPer+35)/TAM;
		
		if(hMat==REN){
			hMat--;
		}
		
		//mov pacman
		yPer+= vYPer * dt;
		if(yPer>ALTO-35){
			yPer=ALTO-35;
		}
		
		//matriz
		if(mapaN2[iMat+1][jMat]=='X'){
			yPer=iMat*TAM;
		}
		if(mapaN2[iMat+1][hMat]=='X'){
			yPer=iMat*TAM;
		}
		
		//recompensa
		if(mapaN2[iMat+1][jMat]=='.'){
		    mapaN2[iMat+1][jMat]='-';
			punt++;	
		}
		if(mapaN2[iMat+1][hMat]=='.'){
			mapaN2[iMat+1][hMat]='-';
			punt++;
		}
	}
	//coliciones con enemigo
	//int xPer,int yPer,int xE,int yE,int xE1,int yE1,int xE2,int yE2
	if(colisionEnemigoN2(xPer,yPer,xE,yE)==true){
		if(!colision){
			vidas--;
			colision=true;
		}
	}else{
		colision=false;
	}
	
	if(colisionEnemigoN3(xPer,yPer,xE1,yE1)==true){
		if(!colision){
			vidas--;
			colision=true;
		}
	}else{
		colision=false;
	}
	
	if(colisionEnemigoN4(xPer,yPer,xE2,yE2)==true){
		if(!colision){
			vidas--;
			colision=true;
		}
	}else{
		colision=false;
	}
}

void Nivel2::enemigosN2(double dt){
	xE1 += vxE1*dt;
	yE1 += vyE1*dt;
	if((vxE1 < 0 && xE1<=0) || (vxE1 > 0 && xE1 >= 1700)){
		vxE = -vxE;
	}//mov de columna
	if((vyE1 < 0 && yE1 <= 0) || (vyE1 > 0 && yE1 >=650)){
		vyE1 = -vyE1;
	}
}

void Nivel2::enemigosN3(double dt){
	xE2 += vxE2*dt;
	yE2 += vyE2*dt;
	if((vxE2 < 0 && xE2<=0) || (vxE2 > 0 && xE2 >= 1700)){
		vxE2 = -vxE2;
	}//mov de columna
	if((vyE1 < 0 && yE2 <= 0) || (vyE2 > 0 && yE2 >=650)){
		vyE2 = -vyE2;
	}
}

void Nivel2::pintar(){
	
	char mensaje[50];
    al_clear_to_color(al_map_rgb(0, 0, 0));
    //se pinta el mapa 
    al_draw_bitmap(fondoN1, 0, 0, 0);
    al_draw_bitmap(fondoN1,1000,0,0);
   
    
    //imagen del mapa
    for (int j = 0; j < REN; j++) {
        for (int i = 0; i < COL; i++) {
            if (mapaN2[j][i] == 'X') {
                al_draw_bitmap(mapaN1P, i * TAM, j * TAM, 0);
            } else if (mapaN2[j][i] == '.') {
                al_draw_bitmap(puntosN1, i * TAM, j * TAM, 0);
            }
        }
    }
    
    //MENSAJES-----------------------------------------------
    //datos del jugador
    sprintf(mensaje, "PUNTUACION MABBEL: %d",punt);
    al_draw_text(font, al_map_rgb(0,255,255),200,723, 0, mensaje);
    
    sprintf(mensaje, "VIDAS DE MABBEL: %d",vidas);
    al_draw_text(font, al_map_rgb(255, 0, 256),200,750, 0, mensaje);
    
    sprintf(mensaje, "MOVIMIENTO %d-%d, dt=%f", yPer,xPer,dt);
    al_draw_text(font, al_map_rgb(255, 255, 0),200,780, 0, mensaje);
    
    //nivel
    sprintf(mensaje, "NIVEL 2 - AGUAS QUE TE ATRAPAN ", yPer,xPer,dt);
    al_draw_text(font, al_map_rgb(255, 0, 255),200,810, 0, mensaje);
    
    //nivel
    sprintf(mensaje, "TIEMPO RESTANTE: %d",tiempo--,"segundos");
    al_draw_text(font, al_map_rgb(255, 0, 255),400,723, 0, mensaje);
    
        //se pinta el modulo de las vidas
    al_draw_bitmap(personaje,0,723,0);
     
    //imagen del jugador
    al_draw_bitmap(mabel, xPer, yPer, 0);
    
    //enemigo
    al_draw_bitmap(enemigoN1, xE, yE, 0);
    al_draw_bitmap(enemigoN2, xE1, yE1, 0);
    al_draw_bitmap(enemigoN3, xE2, yE2, 0);
    //se manda todo a imprimir
    al_flip_display();
}


//CLASE NIVEL3
class Nivel3:public Nivel1{
	public:
    	int xE6[5];
    	int yE6[5];
    	int vxE6[5];
    	int vyE6[5];
    	char mapaN3[REN][COL]={
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
	"X                                              X",
	"XXXXXXXXXXX     XXXXXXXXXXXXXXX     XXXXXXXXXXXX",
	"X               X  ..  X  ..  X                X",
	"X . . . . . .   X .  . X .  . X  . . . . . . . X",
	"X               X .  . X .  . X                X",
	"X       XXXXXXXXX .  . X .  . XXXXXXXXX        X",
	"X. . . .                                . . . .X",
	"XXXXXXXX        . . . . . . . .         XXXXXXXX",
	"X         XXX   XXXXXXXXXXXXXXX   XXX          X",
	"X                                              X",
	"XXXXXXX . . . . . . . . . . . . . . . . XXXXXXXX",
	"X                                              X",
	"X       XXXXXXXXXXXX      XXXXXXXXXXXX         X",
	"X . . . X . . . . .X      X. . . . . X . . . . X",
	"X       X. . . . . X      X . . . . .X         X",
	"X       X .  XX  . X      X .  XX  . X         X",
	"X         .  XX  .          .  XX  .           X",
	"X . . . . . .XX    . . . . .   XX  . . . . . . X",
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"};
	
		Nivel3(){
		
		//punteros
		mapaN1P = al_load_bitmap("cuadroA.png");
        puntosN1 = al_load_bitmap("recomFinal.png");
        decoroN1= al_load_bitmap("decoro2.png");
        mabel = al_load_bitmap("Imagen2.png");
        fondoN1 = al_load_bitmap("fondoN4.png");
        personaje = al_load_bitmap ("Imagen9.jpg");
        enemigoN1= al_load_bitmap("gnomo.png");
		
		//enemigo
		for(int i=0 ; i<6 ;i++){
			xE6[i]= rand()%1000;
			yE6[i]=rand()%1000;
			vxE6[i]=rand()%1000;
			vyE6[i]=rand()%1000;
		}
		//matriz;
		jMat= 0;
        iMat = 0;
        kMat=0;
        punt=0;

    }
	
	//funciones
    virtual void pintar();
    void enemigosN6(double dt);
    void fisicaPersonajeN6(double dt,int direccion);
    bool colisionEnemigoN6(int xPer,int yPer,int xE,int yE);
};

void Nivel3::pintar(){
	char mensaje[50];
    al_clear_to_color(al_map_rgb(0, 0, 0));
    //se pinta el mapa 
    al_draw_bitmap(fondoN1, 0, 0, 0);
    al_draw_bitmap(fondoN1,1000,0,0);
   
    
    //imagen del mapa
    for (int j = 0; j < REN; j++) {
        for (int i = 0; i < COL; i++) {
            if (mapaN3[j][i] == 'X') {
                al_draw_bitmap(mapaN1P, i * TAM, j * TAM, 0);
            } else if (mapaN3[j][i] == '.') {
                al_draw_bitmap(puntosN1, i * TAM, j * TAM, 0);
            }
        }
    }
    
    //MENSAJES-----------------------------------------------
    //datos del jugador
    sprintf(mensaje, "PUNTUACION STAN: %d",punt);
    al_draw_text(font, al_map_rgb(0,255,255),300,723, 0, mensaje);
    
    sprintf(mensaje, "VIDAS DE STAN: %d",vidas);
    al_draw_text(font, al_map_rgb(255, 0, 256),300,750, 0, mensaje);
    
    sprintf(mensaje, "MOVIMIENTO %d-%d, dt=%f", yPer,xPer,dt);
    al_draw_text(font, al_map_rgb(255, 255, 0),300,780, 0, mensaje);
    
    //nivel
    sprintf(mensaje, "NIVEL 3 - AQUI ES DONDE HAY MAS GNOMOS ", yPer,xPer,dt);
    al_draw_text(font, al_map_rgb(255, 0, 255),300,810, 0, mensaje);
    
    //nivel
    sprintf(mensaje, "TIEMPO RESTANTE: %d",tiempo--,"segundos");
    al_draw_text(font, al_map_rgb(255, 0, 255),600,723, 0, mensaje);
    
        //se pinta el modulo de las vidas
    al_draw_bitmap(personaje,0,723,0);
     
    //imagen del jugador
    al_draw_bitmap(mabel, xPer, yPer, 0);
    
    //enemigo
    for(int i=0;i<5;i++){
	    al_draw_bitmap(enemigoN1,xE6[i], yE6[i], 0);	
	}
    //se manda todo a imprimir
    al_flip_display();
}

void Nivel3::enemigosN6(double dt){
	for(int i=0;i<5;i++){
		xE6[i] += vxE6[i]*dt;
		yE6[i] += vyE6[i]*dt;
		
		if((vxE6[i] < 0 && xE6[i]<=0) || (vxE6[i] > 0 && xE6[i] >= 1700)){
		   vxE6[i] = -vxE6[i];
		}//mov de columna
		
		if((vyE6[i] < 0 && yE6[i] <= 0) || (vyE6[i] > 0 && yE6[i] >=650)){
		  vyE6[i] = -vyE6[i];  
		}
	}

}

void Nivel3::fisicaPersonajeN6(double dt,int direccion){
	int i=0;
	if(direccion == DERECHA){
		jMat=xPer/TAM;
		iMat=yPer/TAM;
		kMat = (yPer+35)/TAM;
		
		if(kMat == COL){
			kMat--;
		}
		
		//movimiento del pacman
		xPer += vXPer * dt;
		if(xPer > ANCHO - 35){
			xPer= ANCHO - 35;
		}
		
		//imposicion de limites
		if(mapaN3[iMat][jMat+1] == 'X'){
			xPer = jMat *TAM;
		}
		
		if(mapaN3[kMat][jMat+1] == 'X'){
			xPer=jMat*TAM;
		}
		
		//recompensa 
		if(mapaN3[kMat][jMat+1] =='.'){
			//xPer=jMat*TAM;
			mapaN3[kMat][jMat+1] = '-';
			punt++;
		}
		
		if(mapaN3[iMat][jMat+1] =='.'){
			mapaN3[iMat][jMat+1] = '-';
			punt++;
		}
	}//movimiento hacia la derecha
	
	if(direccion == IZQUIERDA){
		jMat=(xPer+35)/TAM; //tamaño por casilla
		iMat=(yPer+35)/TAM;
		kMat=yPer/TAM;
		
		if(jMat==REN){ //
			jMat--;
		}
		if(iMat==COL){  //
			iMat--;
		}
		
		//mov pacman 
		xPer -= vXPer * dt;
		if(xPer<0){
			xPer=0;
		}
		
		if(mapaN3[iMat][jMat-1]=='X'){
			xPer=jMat*TAM;
		}
		
		if(mapaN3[kMat][jMat-1]=='X'){
			xPer=jMat*TAM;
		}
		
		//recompensa
		if(mapaN3[iMat][jMat-1]=='.'){
			mapaN3[iMat][jMat-1]='-';
			punt++;
		}
		
		if(mapaN3[kMat][jMat-1]=='.'){
			mapaN3[kMat][jMat-1]='-';
			punt++;
		}
	}//moviemiento a la izquierda
	
	if(direccion == ARRIBA){
		jMat=(xPer+35)/TAM; //tamaño por casilla
		iMat=(yPer+35)/TAM;
		hMat=xPer/TAM;
		
		if(jMat==REN){
			jMat--;
		}
		if(iMat==COL){
			iMat--;
		}
		
		yPer -= vYPer * dt;
		if(yPer<0){
			yPer=0;
		}
		
		if(mapaN3[iMat-1][jMat]=='X'){
			yPer=iMat*TAM;
		}
		
		if(mapaN3[iMat-1][hMat]=='X'){
			yPer=iMat*TAM;
		}
		
		//recompensa
		if(mapaN3[iMat-1][jMat]=='.'){
			mapaN3[iMat-1][jMat]='-';
			punt++;
		}
		
		if(mapaN3[iMat-1][hMat]=='.'){
			mapaN3[iMat-1][hMat]='-';
			punt++;
		}
	}//movimeinto hacia arriba
	
	//movimiento hacia abajo
	if(direccion == ABAJO){
		jMat=xPer/TAM;
		iMat=yPer/TAM;
		hMat=(xPer+35)/TAM;
		
		if(hMat==REN){
			hMat--;
		}
		
		//mov pacman
		yPer+= vYPer * dt;
		if(yPer>ALTO-35){
			yPer=ALTO-35;
		}
		
		//matriz
		if(mapaN3[iMat+1][jMat]=='X'){
			yPer=iMat*TAM;
		}
		if(mapaN3[iMat+1][hMat]=='X'){
			yPer=iMat*TAM;
		}
		
		//recompensa
		if(mapaN3[iMat+1][jMat]=='.'){
		    mapaN3[iMat+1][jMat]='-';
			punt++;	
		}
		if(mapaN3[iMat+1][hMat]=='.'){
			mapaN3[iMat+1][hMat]='-';
			punt++;
		}
	}
	//coliciones con enemigo
    if(colisionEnemigoN6(xPer, yPer, xE6[0], yE6[0]) == true) {
    if(!colision) {
        vidas--;
        colision = true;
    }
	}
    
    if(colisionEnemigoN6(xPer, yPer, xE6[1], yE6[1]) == true) {
    if(!colision) {
        vidas--;
        colision = true;
    }
	}
	
	if(colisionEnemigoN6(xPer, yPer, xE6[2], yE6[2]) == true) {
    if(!colision) {
        vidas--;
        colision = true;
    }
	}
	
	if(colisionEnemigoN6(xPer, yPer, xE6[3], yE6[3]) == true) {
    if(!colision) {
        vidas--;
        colision = true;
    }
	}
	
	if(colisionEnemigoN6(xPer, yPer, xE6[4], yE6[4]) == true) {
    if(!colision) {
        vidas--;
        colision = true;
    }
	}
	
	if(colisionEnemigoN6(xPer, yPer, xE6[5], yE6[5]) == true) {
    if(!colision) {
        vidas--;
        colision = true;
    }
	}

}

bool Nivel3::colisionEnemigoN6(int xPer,int yPerm ,int xE, int yE){
    if (xPer < xE+ PIX && xPer + PIX > xE && yPer < yE + PIX && yPer + PIX > yE){
    	// se detecta la colisi�n
        return true;
    }else{
        return false;
    }
      
}//bool




class CMenu {
public:
    CMenu();
    ~CMenu();

    void mostrar(ALLEGRO_MOUSE_STATE& estadoRaton);
    int manejarEventoRaton(ALLEGRO_MOUSE_STATE& estadoRaton, Nivel1& mapaN1);
    int obtenerSeleccion();

private:
    ALLEGRO_BITMAP* menu1;
    ALLEGRO_BITMAP* menu2;
    //Musica para el menu
	ALLEGRO_SAMPLE* stMenu = al_load_sample("intro.wav");
	
	/*if (!stMenu) {
    printf("Error al cargar la música.\n");
    return 1;
	}*/

    int seleccion;
};

CMenu::CMenu() {
    seleccion = 0;

    // Carga las imágenes para el menú
    menu1 = al_load_bitmap("menu1.png");
    menu2 = al_load_bitmap("menu2.png");
}

CMenu::~CMenu() {
    al_destroy_bitmap(menu1);
    al_destroy_bitmap(menu2);
}

void CMenu::mostrar(ALLEGRO_MOUSE_STATE& estadoRaton) {
    al_draw_bitmap(menu1, 0, 0, 0);

    // Verifica si el cursor del ratón está sobre el botón de inicio
    if (estadoRaton.x >= 480 && estadoRaton.x <= 771 && estadoRaton.y >= 362 && estadoRaton.y <= 489) {
        al_draw_bitmap(menu2, 0, 0, 0);
    }
    al_play_sample(stMenu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
}

int CMenu::manejarEventoRaton(ALLEGRO_MOUSE_STATE& estadoRaton, Nivel1& mapaN1) {
	al_draw_bitmap(menu1, 0, 0, 0);
    // Verifica si el usuario hizo clic en el botón de inicio
    if (estadoRaton.x >= 480 && estadoRaton.x <= 771 && estadoRaton.y >= 362 && estadoRaton.y <= 489 && estadoRaton.buttons & 1) {
    	mapaN1.iniciarPartida();
        return OPCION_INICIAR_JUEGO;
    }
    return SIN_ACCION;
}

int CMenu::obtenerSeleccion() {
    return seleccion;
}

int main() {
    if (init() != 0) {
        deinit();
        return 1;
    }

    al_start_timer(timer);

    Nivel1 mapaN1;
    Nivel2 nivelN2;
    Nivel3 nivelN3;
    //objeto
    
    CMenu menu;
    
    //variables para el manejo de evento 
    bool hecho = false;
    bool redibujar = true;
    //variables para funciones
    double tiempo = 0;  // función de física
    //variables para el movimiento del objeto
    double tAct = 0, tAnt = 0;
    int direccion = DERECHA;
    //paso de nivel
    int nivel=0;
    
    ALLEGRO_MOUSE_STATE estadoRaton;
    
    

    while (1) {
    	al_wait_for_event(cola, &event);
        tiempo = tAct - tAnt;

        // Manejar la lógica del juego (movimiento del personaje, etc.)
        if (mapaN1.partida){
        	nivel=1;
            mapaN1.fisicaPersonaje(tiempo, direccion);
            mapaN1.enemigo(tiempo);
            
            if(mapaN1.tiempo<=0 || mapaN1.vidas==0){
            	mapaN1.tiempo=TIEMPO;
            	mapaN1.vidas=3;
            	al_rest(0.50);
            	mapaN1.reinicio();
			}
			
			if(mapaN1.vidas>0 && mapaN1.punt==87){
				nivel=2;
				nivelN2.partida=true;
			}
        }
        
        if(nivelN2.partida){
        	mapaN1.partida=false;
        	
        	nivelN2.enemigosN2(tiempo);
        	nivelN2.enemigo(tiempo);
        	nivelN2.enemigosN3(tiempo);
        	nivelN2.fisicaPersonajeN2(tiempo,direccion);
    
        	if(nivelN2.tiempo <= 0 || nivelN2.vidas==0){
        		nivelN2.tiempo=TIEMPO;
        		nivelN2.vidas=3;
        		al_rest(0.50);
        		nivelN2.reinicio();
			}
			
			if(nivelN2.vidas>0 && nivelN2.punt==110){
				nivelN3.partida=true;
				nivel=3;
				//nivel3N.partida=true;
			}
        	
		}
	
		if(nivelN3.partida){
			nivelN2.partida=false;
			nivelN3.enemigosN6(tiempo);
			nivelN3.fisicaPersonajeN6(tiempo,direccion);
			
			if(nivelN3.tiempo <= 0 || nivelN3.vidas==0){
        		nivelN3.tiempo=TIEMPO;
        		nivelN3.vidas=3;
        		al_rest(0.50);
        		nivelN3.reinicio();
			}
			
			if(nivelN3.vidas>0 && nivelN3.punt==110){
				nivelN3.partida=false;
				mostrarFinal();
				al_rest(0.90);
				nivel=3;
				//nivel3N.partida=true;
			}
        	
		}
        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                redibujar = true;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                    direccion = IZQUIERDA;
                } else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                    direccion = DERECHA;
                } else if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                    direccion = ARRIBA;
                } else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                    direccion = ABAJO;
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                hecho = true;
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (!mapaN1.partida) {
                	al_stop_samples();
                	ALLEGRO_SAMPLE* stN1 = al_load_sample("OST.wav");
                    al_play_sample(stN1, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
                    
                    // Verificar clic del ratón solo si no estás en la partida
                    ALLEGRO_MOUSE_STATE estadoRaton;
                    al_get_mouse_state(&estadoRaton);
                    int accion = menu.manejarEventoRaton(estadoRaton, mapaN1);
                    if (accion == OPCION_INICIAR_JUEGO) {
                        mapaN1.iniciarPartida();
                    }
                }
                break;
        }

        if (hecho) {
            break;
        }

        if (redibujar && al_is_event_queue_empty(cola)) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            
            if (mapaN1.partida==true) {
                mapaN1.pintar();
            }else if (nivelN2.partida==true){
            	nivelN2.pintar();
            	
			}else if(nivelN3.partida==true){
            	nivelN3.pintar();
            	
			}else{
				menu.mostrar(estadoRaton);	
			}
			

            redibujar = false;
            al_flip_display();
        }

        // Resta de tiempos
        tAnt = tAct;
        tAct = al_get_time();
    }
    
    deinit();

    return 0;
}


int init() {
    if (!al_init()) {
        printf("No se pudo inicializar Allegro.\n");
        return 1;
    }

    if (!al_install_keyboard()) {
        printf("No se pudo inicializar el teclado.\n");
        return 1;
    }

    timer = al_create_timer(1.0 / 30.0);
    if (!timer) {
        printf("No se pudo inicializar el temporizador.\n");
        return 1;
    }

    cola = al_create_event_queue();
    if (!cola) {
        printf("No se pudo inicializar la cola de eventos.\n");
        return 1;
    }
//800 y 830
    pantalla = al_create_display(1725,850);
    if (!pantalla) {
        printf("No se pudo inicializar la pantalla.\n");
        return 1;
    }

    al_register_event_source(cola, al_get_keyboard_event_source());
    al_register_event_source(cola, al_get_display_event_source(pantalla));
    al_register_event_source(cola, al_get_timer_event_source(timer));

    al_install_mouse();
    al_register_event_source(cola, al_get_mouse_event_source());


    font= al_create_builtin_font();
    if (!font) {
        printf("No se pudo inicializar la fuente.\n");
        return 1;
    }

    if (!al_init_image_addon()) {
        printf("No se pudo inicializar el complemento de imagen.\n");
        return 1;
    }

    if (!al_init_primitives_addon()) {
        printf("No se pudieron inicializar las primitivas.\n");
        return 1;
    }
    
    if (!al_install_mouse()) {
    printf("No se pudo inicializar el ratón.\n");
    return 1;
	}
	
	if (!al_install_audio()) {
    printf("Error al inicializar el sistema de audio.\n");
    return 1;
	}

	if (!al_init_acodec_addon()) {
    printf("Error al inicializar el complemento de códecs de audio.\n");
    return 1;
	}

	if (!al_reserve_samples(1)) {
    printf("Error al reservar canales de audio.\n");
    return 1;
	}
		

    return 0;
}

void mostrarFinal(){
	ALLEGRO_BITMAP* final;
	final = al_load_bitmap("final.png");
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_bitmap(final, 0, 0, 0);
	
}

void deinit() {
    al_destroy_font(font);
    al_destroy_display(pantalla);
    al_destroy_timer(timer);
    al_destroy_event_queue(cola);
}
