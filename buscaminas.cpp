//por Samuel Venegas Bernal
//cometario extra en un inicio no tenia casi nada de comentarios pero hubo un momento donde
//me confundi un monton, haci que hice borron y cuenta nueva para comentarios completos por si se ven un monton de esos comentarios d:
#include <iostream>   
#include <vector>     
#include <string>
#include <sstream>    
#include <fstream>    
#include <cstdlib>  //<--este es para randomizar casillas y poner minas
#include <ctime>    //<-- para el tiempo 

using namespace std;
// estos son variables globales que se estaran usando a lo largo del codigo
vector<vector<int> >  tablero;   // <-- el tablero 
vector<vector<bool> > revelado;  // <-- las casillas que el jugador puede ver
vector<vector<bool> > bandera;   // <-- casillas señaladas que tienen minas	
int filas, columnas, totalMinas; 
bool juegoTerminado;
bool victoria;

void configurarmatrices (){
	
	tablero.assign(filas,vector<int>(columnas, 0));
	revelado.assign(filas, vector<bool>(columnas, false));
    bandera.assign(filas, vector<bool>(columnas, false));
    //for (int f = 0; f < filas; f++) {		<-- esta es otra opción que vi para poder asignar matrices a los vectores por si el .assign no se puede usar. (porfis dejeme usarlo)
    //vector<int> fila;
    //for (int c = 0; c < columnas; c++) {
    //    fila.push_back(0);
    //}
    //tablero.push_back(fila);
	
}

int localizarminas(int f, int c){ //filas y columnas; voy a esta usando f o c para referirme a las filas y columnas una gran parte del codigo por si se ve mucho que uso sinonimos como df nf,o primerf d:
	int cont=0;
	for(int df =-1; df<=1; df++){ //<-- df = direcón pa las filas
		for(int dc =-1; dc<=1; dc++){ //<-- dc = dirección columnas
			int nf = f + df;
			int nc = c + dc;
			if(nf>=0 && nf < filas && nc>=0 && nc < columnas){
				if (tablero[nf][nc]== -1){ //el -1 significa que es una mina
					cont++;
				}
			} 
		}
	}
	return cont;
}

void generartablero(int primerf, int primerc ){
	srand(time(0));// <-- investigando para poder randomizar las casillas entendi que usando solo rand() no seria del todo aleatorio porque
				   //     usa exactamente el mismo patrón de numero aleatorios, lo que hace srand(time(0)) es usar la hora o el tiempo que 
				   //	  tenga el pc para generar los numero mas aelatorios asi que los numeros seran direntes ya sea 1:00PM o 1:01PM.
	int minas = 0;

	while(minas < totalMinas){
		int f = rand() % filas;
		int c = rand() % columnas;
		if (tablero[f][c] !=-1 && !(f==primerf && c==primerc)){ //uso el if para evitar que se generen minas en casillas que ya tengan una mina y para que no aparesca en la primera celda que revele el jugador
			tablero[f][c]=-1;
			minas++;
		}
	}
	for(int f= 0; f<filas; f++){
		for(int c= 0; c<columnas; c++){
			if (tablero[f][c] != -1){
				tablero[f][c]=localizarminas(f, c); 
			}
		}
	}	
}

void mostrartablero(){
	cout<<"\n";
	cout<<"    ";
	for(int c = 0; c < columnas; c++){	//<-- para mostrar las columnas
		if(c < 10){ //<-- esto es para evitar que el tablero se descuadre a la derecha
        	cout<<c<<"  ";  
    	} else {
        	cout<<c<<" ";       
    	}
	}
	cout<<"\n";
	cout<<"    ";
	for(int c = 0; c < columnas; c++) cout<<"---";
	cout<<"\n";
	
	for(int f = 0; f < filas; f++){
		if(f < 10){ //<-- esto es para evitar que el tablero se descuadre a la derecha
        	cout<<" "<<f<<" |";  
    	} else {
        	cout<<f<<" |";       
    	}
		for(int c = 0; c < columnas; c++){
			if(bandera[f][c]){ //<-- banderas colocadas
				cout<<"F  ";
			}else if(!revelado[f][c]){ //<-- casillas sin revelar
				cout<<".  ";
			}else if(tablero[f][c]== -1){ //<-- mostrar minas, paila si le mostro la mina
				cout<<"*  ";
				
			}else if(tablero[f][c]==0){ // <-- casilla vacia
				cout<<"   ";
			}else{
				cout<<tablero[f][c]<<"  ";
			}
		}
		cout<<"\n";
	}
	cout<<"\n";
}

bool verificarvictoria(){
	for (int f=0; f<filas; f++){
		for (int c=0; c<columnas; c++){
			if(tablero[f][c] != -1 && !revelado[f][c]){
				return false; //<-- para indicar que no se ha revelado alguna casilla sin mina
			} 
		}
	}
	return true; //<-- ya gano
}

void revelarcasillas(int f, int c){ //<-- la funcion recursiva importante
	
	if(f<0 || f>= filas || c<0 || c>= columnas) return;
	if(revelado[f][c]) return;
	if(bandera[f][c]) return;
	
	revelado[f][c] = true;
	
	if(tablero[f][c]==-1){ //<-- si revela una mina perdio
		juegoTerminado= true;
		victoria= false;
		return;
	}
	if (tablero[f][c]== 0){ //<-- revela casillas cercanas
		revelarcasillas(f-1,c-1);
		revelarcasillas(f-1,c);
		revelarcasillas(f-1,c+1);
		revelarcasillas(f,c-1);
		revelarcasillas(f,c+1);
		revelarcasillas(f+1,c-1);
		revelarcasillas(f+1,c);
		revelarcasillas(f+1,c+1);
	}
	
}

void guardarpartida(){
	ofstream partida("partida_guardada.txt");
	if (!partida){
		cout<<"error al guardar";
	}else if(partida.is_open()){
		partida<<filas<<" "<<columnas<<" "<<totalMinas<<"\n"; 
		for(int f = 0; f<filas; f++){ // <-- esto para guardar el tablero en si.
			for(int c=0;c<columnas; c++){
				partida << tablero[f][c]<<" ";
			}
			partida<<"\n";
		}
		for(int f = 0; f<filas; f++){ //<-- guardar lo que ya se ha revelado
			for(int c=0;c<columnas; c++){
				partida << revelado[f][c]<<" ";
			}
			partida<<"\n";
		}
		for(int f = 0; f<filas; f++){ //<-- guardar la ubicación de  las banderas
			for(int c=0;c<columnas; c++){
				partida << bandera[f][c]<<" ";
			}
			partida<<"\n";
		}
		partida.close();
		cout<<"partida guardada";
	}
}

bool cargarpartida(){
	ifstream partida("partida_guardada.txt");
	if(!partida){
		cout<<"no hay partida guardada.\n";
		return false;
	}else if(partida.is_open()){
		partida>>filas>>columnas>>totalMinas;
		configurarmatrices();
		for(int f=0;f<filas;f++){ //<-- para cargar el tablero
			for(int c=0;c<columnas;c++){
				partida>>tablero[f][c];			
			}
		}
		for(int f=0;f<filas;f++){ //<-- para cargar las casillas reveladas
			for(int c=0;c<columnas;c++){
				int tpm;
				partida>>tpm;
				revelado[f][c]=tpm;
			}
		}
		for(int f=0;f<filas;f++){ //<-- para cargar las banderas colocadas
			for(int c=0;c<columnas;c++){
				int tpm;
				partida>>tpm;
				bandera[f][c]=tpm;
			}
		}
	partida.close();
	cout<<"partida cargada.\n";
	return true;
	}
	
}

void mostrarpuntuacion(){
	ifstream puntos("puntuaciones.txt");
	if(!puntos){
		cout<<"no hay una puntuación guardada.\n";
		return;
	}else if(puntos.is_open()){
		cout<<"\n ||=== PUNTUACIONES ===|| \n";
		cout<< "Nombres || Nivel || Movimientos\n";
		cout<<"-----------------------------\n";
		string linea;
		while(getline(puntos,linea)){
			stringstream ss(linea);
			string nombre, nivel, movs;
	
	        getline(ss, nombre, ';');
	        getline(ss, nivel, ';');
	        getline(ss, movs, ';');
	        cout << nombre << " | " << nivel << " | " << movs << "\n";
		}
		puntos.close();
	}
}


string seleciodedificultad(){
	string opcion; //<-- lo puse como string porque no falta el chistoso que no lee y escribe facil o algo mas
	cout<<"\n1. facil (9x9 y 10 minas)\n";
	cout<<"2. medio (16x16 y 40 minas)\n";
	cout<<"3. dificil (16x30 y 99 minas)\n";
	cout<<"pon 1, 2 o 3 para selecionar la dificultad";
	cout<<"opcion: ";
	cin>>opcion;
	while(true){
	
		if(opcion=="1"){
			filas=9; columnas =9; totalMinas=10;
			return "facil";
		}else if(opcion=="2"){
			filas=16; columnas =16; totalMinas=40;
			return "medio";
		}else if(opcion=="3"){
			filas=16; columnas =30; totalMinas=99;
			return "dificil";
		}else{
			cout<<"dificultad no valida escoja una de las opciones\n";
			cin>>opcion;
		}
	}
}

void juego(string nivel){
	juegoTerminado=false;
	victoria=false;
	bool tablerogenerado= false;
	int movimientos = 0;
	while(!juegoTerminado){
		mostrartablero();
		string accion;
		int f, c;
		cout<<"1. revelar casilla\n"; //<-- el menu de las opciones
		cout<<"2. poner o quitar bandera\n";
		cout<<"3. guardar partida\n";
		cout<<"4. salir al menu\n";
		cout<<"acción: ";
		cin>>accion;
		
		if(accion=="3"){//<-- inicie en desorden si, pero que tortura quedarme estancado tratando de ver como hacer la acción 1
						// asi que preferi iniciar por los más faciles primero ya luego me torturo (una disculpa a los que tiene toc).
			guardarpartida();
			continue;
		}
		if(accion=="4"){ //<-- vuelve al menu
			cout<<"saliendo al menu...\n";
			return;
		}
		cout<<"fila: "; cin>> f;
		cout<<"columna: "; cin>> c;
		if(f<0 || f>=filas || c<0 || c>=columnas){
			cout<<"movimiento invalido";
			continue;
		}
		if(accion=="2"){ //<-- coloca o quita las banderas
			if(!revelado[f][c]){
				bandera[f][c]=!bandera[f][c];
				cout<<(bandera[f][c] ? "bandera colocada": "bandera quitada");
			}else{
				cout<<"esta casilla ya esta revelada";
			}
			continue;
		}
		if(accion =="1"){//<-- revelar las casillas
			if(revelado[f][c]){
				cout<<"esta casilla ya esta revelada";
				continue;
			}
			if(bandera[f][c]){
				cout<<"quita la bandera primero para revelar la casilla";
				continue;
			}
			if(!tablerogenerado){
				generartablero(f,c);
				tablerogenerado=true;
			}
			movimientos++;
			revelarcasillas(f,c);
			if(juegoTerminado){
				for(int f=0; f<filas; f++){
					for(int c=0; c<columnas; c++){
						revelado[f][c]=true;
					}
				}
				mostrartablero();
				cout<<"BOOM!! perdiste\n";
			}else if(verificarvictoria()) {
				juegoTerminado=true;
				mostrartablero();
				cout<<"GANASTE en "<< movimientos << " movimientos!\n";
				string nombre;
				cout<< "tu nombre: ";
				cin>> nombre;
				ofstream puntos("puntuaciones.txt", ios::app); //<-- aqui use el ios::app para poder la puntuación sin que borrara el resto de las puntuaciones anteriores.
				puntos<< nombre<<";"<<nivel<<";"<<movimientos<< "\n";
				puntos.close();
			}
			
		}
		if(accion!="1"&& accion!="2"&& accion!="3"&& accion!="4"){
				cout<<"seleccione una de las opciones\n";
				
		}
	}
}


int main(){
	
	while (true){
		
		string opcion;
		cout<<"\n||====BUCAMINAS====||\n";
		cout<<"1. nueva partida\n";
		cout<<"2. cargar partida\n";
		cout<<"3. ver puntuaciones\n";
		cout<<"4. salir\n";
		cout<<"opción: ";
		cin>>opcion;
		if(opcion=="1"){
			string nivel= seleciodedificultad();
			configurarmatrices();
			juego(nivel);
		}else if(opcion=="2"){
			if(cargarpartida()){
				juego("carga");
			}
		}else if(opcion=="3"){
			mostrarpuntuacion();
		}else if(opcion=="4"){
			cout<<"hasta luego";
			break;
		}else if(opcion!="1"&& opcion!="2"&& opcion!="3"&& opcion!="4"){
				cout<<"seleccione una de las opciones\n"<<endl;
				
		}
	}
	return 0;
}
