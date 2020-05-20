#include "conio2.h"
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <stdio.h>
#include <fstream>


#define CHEATS true
//defining posotions coordinates
#define TEXTBLOCK_POS1 3
#define TEXTBLOCK_POS2 9
#define TEXTBLOCK_POS3 8
#define TEXTBLOCK_POS4 9
#define TEXTBLOCK_POS5 4
#define TEXTBLOCK_POS6 10
#define PAZZLEBLOCK_POS 5
#define MENU_SECTION_POS 14
#define BLOCKS_POS_LEFT 5 //48
#define BLOCKS_POS_RIGHT 38 //5

//defining blocks positions
#define HOT_KEYS TEXTBLOCK_POS5
#define NAME_POS TEXTBLOCK_POS1
#define SURNAME_POS TEXTBLOCK_POS2
#define STUDENTID_POS TEXTBLOCK_POS3
#define IMPLEMENTED_POS TEXTBLOCK_POS2
#define KURSOR_POS TEXTBLOCK_POS6

#define PUZZLE_SIDE BLOCKS_POS_RIGHT
#define TEXT_MENU_SIDE BLOCKS_POS_LEFT

//defining student data
#define NAME "Irina"
#define SURNAME "Frolova"
#define STUDENTID "s174548"
#define IMPLEMENTED "conio2(gotxy,getche,textcolor)"
//defining colors
#define SELECTEDITEMCOLOR YELLOW
#define UNSELECTEDITEMCOLOR WHITE
#define USERADDEDITEMCOLOR GREEN
#define TEXTBACKGROUND BLUE

#define UP 0x00
#define DOWN 0x01
#define LEFT 0x10
#define RIGHT 0x11
using namespace std;
struct Matrix {
	bool **matrix; //rozrachunkowa
	char **cmatrix; //wyświetlana
	char **ncmatrix; //szablon odpowiedzi
	char **umatrix; //szablon wprowadzania
	bool **mmatrix;	//otwieranie losowych okien
	bool correct;  
};

char *igrok ;
char *saveigr;
int l_igrok=0; //długość nazwy gracza
Matrix puzzle;	//zmienna z grą					
int cursorX,cursorY,cursorMX,cursorMY;
bool puzzleLoaded,exitAttempt,newGameAttempt,loading; // flagi
int selected_size,count1s;  //rozmiar matrycy, liczba niewypełnionych bloków
void hellow();
void exitmem();
void showMainMenu();
void printFrame();
void printTextBlock();
void printMenu();
void printPuzzle();
void clearArea();
Matrix generate(int);	
Matrix process(Matrix);
void startGenerate();
void setNextCursor(int dimention);
void setValue(bool);
bool won();
void savegame();
void loadgame();
bool wonned;
void printWon();
int sizei = 0;  //width mask
int sizej = 0;	//hight mask
bool errznak = true;  //show error

void init(){
	puzzleLoaded=false;  //status loading
	cursorX=79;			//right side
	cursorY=22;			//bottom side
	cursorMX=0;			//left side
	cursorMY=0;			//left top
	count1s=0;
	exitAttempt=false;   
	newGameAttempt=false;
	loading=false;
	selected_size=0;
	wonned=false;
	clearArea();
	printFrame();
	gotoxy(79,22);
	
}
int main(){


	
	init();
	bool exit=false;
	showMainMenu();
	int button;
	
	while(!exit){
		button=getche();
		bool arrowAction=false;
		switch(button){
			
			case 27:{//esc
				if(!newGameAttempt&&!exitAttempt)exitAttempt=true;
				else if(newGameAttempt)newGameAttempt=false;
				else if(exitAttempt)exitAttempt=false;
				if(wonned){
					init();
					wonned=false;
					arrowAction=false;
				}
				break;
			}
			case 0:{//arrows handler
				button=getche();
				switch(button){
					case 75:{//left
						if(puzzleLoaded)newGameAttempt=false;
						if(!wonned&&newGameAttempt)printMenu();
						if(!wonned)setNextCursor(LEFT);
						if(!wonned)arrowAction=true;

						break;
					}
					case 77:{//right
						if(puzzleLoaded)newGameAttempt=false;
						if(!wonned&&newGameAttempt)printMenu();
						if(!wonned)setNextCursor(RIGHT);
						if(!wonned)arrowAction=true;
						break;
					}
					case 72:{//up
						if(puzzleLoaded)newGameAttempt=false;
						if(!wonned&&newGameAttempt)printMenu();
						if(!wonned)setNextCursor(UP);
						if(!wonned)arrowAction=true;
						break;
					}
					case 80:{//down
						if(puzzleLoaded)newGameAttempt=false;
						if(!wonned&&newGameAttempt)printMenu();
						if(!wonned)setNextCursor(DOWN);
						if(!wonned)arrowAction=true;
						break;
					}
				}
				break;
			}
			case 48:{//0
				if(puzzleLoaded&&!wonned)setValue(false);
				break;
				
			}
			case 49:{//1
				if(newGameAttempt){
					selected_size=6;
					loading=true;
					newGameAttempt=false;
				}
				else if(puzzleLoaded&&!wonned)setValue(true);
				break;
			}
			case 50:{//2
				if(newGameAttempt){
					selected_size=8;
					loading=true;
					newGameAttempt=false;
				}
				break;
			}
			case 51:{//3
				if(newGameAttempt)selected_size=10;
				break;
			}
			case 121:{//y
				if(exitAttempt)exit=true;
				else if(newGameAttempt&&selected_size==10){
					loading=true;
					newGameAttempt=false;
				}
				break;
			}
			case 89:{//Y
				if(exitAttempt){
					exit=true;
					exitmem();
				}else if(newGameAttempt&&selected_size==10){
					loading=true;
					newGameAttempt=false;
				}
				break;
			}
			case 110:{//n
				if(!newGameAttempt&&!exitAttempt){
					newGameAttempt=true;
				}
				else if(exitAttempt)exitAttempt=false;
				else if(newGameAttempt&&selected_size==10)newGameAttempt=false;
				break;
			}
			case 78:{//N
				if(!newGameAttempt&&!exitAttempt){
					newGameAttempt=true;
				}
				else if(exitAttempt)exitAttempt=false;
				else if(newGameAttempt&&selected_size==10)newGameAttempt=false;
				break;
			}
			case 'p': { //włącz / wyłącz podpowiedż
				if (errznak) errznak = false;
				else errznak = true;
				gotoxy(TEXT_MENU_SIDE, STUDENTID_POS+10);				
				if (errznak)cputs(" enable  "); 
				else cputs(" disable  ");
				break;
			}
			case 's': { 
				savegame();
				break;
			}
			case 'l': {
				loadgame();
				break;
			}
		}
		if(!arrowAction)showMainMenu();
		else if(puzzleLoaded){
			
			printPuzzle();
			gotoxy(cursorX,cursorY);
		}
		else if(arrowAction)gotoxy(cursorX,cursorY);
		if(loading){
			puzzleLoaded=false;
			showMainMenu();
			startGenerate();	
		}
	
	}

}
void hellow(){	
	char button=' ';	
	char *igrok_temp = new char[20];
	int i = 0;
	for (i = 0; i < 19; i++) {
		button = getche();
		igrok_temp[i] = button;
		if (button == 13)break;
	}
	igrok_temp[i] = '\0';
	igrok = new char[i];
	igrok = igrok_temp;
	l_igrok = i;

	saveigr = new char[i];
	
	
	saveigr = igrok_temp; //nazwa pliku
	
		

	
}
void showMainMenu(){
	
	printTextBlock();
	printMenu();
	printPuzzle();
	gotoxy(cursorX,cursorY);
	printWon();
	
}
void clearArea(){
	for(int i=1;i<=80;i++){
		for(int j=1;j<=22;j++){
			textcolor(WHITE);
			textbackground(BLUE);
			gotoxy(i,j);
			cputs(" ");
		}
	}
	
}
void printFrame(){
	for(int i=1;i<=80;i++){
		for(int j=1;j<=22;j++){
			if(i==1||i==80||j==1||j==22){
				gotoxy(i,j);
				cputs("+");
			}
		}
	}
	
	gotoxy(27,1);
	cputs("--==Binary Puzzle==--");
	
}
void printTextBlock(){
	textcolor(WHITE);
	textbackground(TEXTBACKGROUND);
	gotoxy(TEXT_MENU_SIDE,NAME_POS);
	cputs("Igrok Name:");
	gotoxy(TEXT_MENU_SIDE+12,NAME_POS);
	textcolor(YELLOW);
	cputs(igrok); //NAME
	textcolor(WHITE);
	gotoxy(TEXT_MENU_SIDE,HOT_KEYS);
	cputs("Hot keys:"); 
	gotoxy(TEXT_MENU_SIDE, HOT_KEYS+1);
	cputs("p - hint:");
	if(errznak)cputs(" enable  ");
	else cputs(" disable  ");
	gotoxy(TEXT_MENU_SIDE, HOT_KEYS + 2);
	cputs("s - save igra");
	gotoxy(TEXT_MENU_SIDE, HOT_KEYS + 3);
	cputs("l - load igra");
	
	gotoxy(TEXT_MENU_SIDE+2, STUDENTID_POS);
	cputs(NAME);
	cputs("  ");
	cputs(SURNAME);	
	cputs(" : "); //Student
	cputs(STUDENTID);
	gotoxy(TEXT_MENU_SIDE, IMPLEMENTED_POS);
	cputs(IMPLEMENTED);
	gotoxy(TEXT_MENU_SIDE+5, KURSOR_POS);
	cputs(" X:    Y: ");

	for (int i=TEXT_MENU_SIDE-2;i<TEXT_MENU_SIDE+32;i++){
		for (int j=TEXTBLOCK_POS1-1;j<TEXTBLOCK_POS4+3;j++){
			if(i==TEXT_MENU_SIDE-2||i==TEXT_MENU_SIDE+31||j==TEXTBLOCK_POS1-1||j==TEXTBLOCK_POS4+2){
				gotoxy(i,j);
				cputs("=");
			}
		}
	}
}
void printMenu(){
	textcolor(YELLOW);
	for (int i=TEXT_MENU_SIDE-2;i<TEXT_MENU_SIDE+32;i++){
		for (int j=MENU_SECTION_POS-2;j<MENU_SECTION_POS+8;j++){
			if(i==TEXT_MENU_SIDE-2||i==TEXT_MENU_SIDE+31||j==MENU_SECTION_POS-2||j==MENU_SECTION_POS+7){
				gotoxy(i,j);
				cputs("-");
			}
			else {
				gotoxy(i,j);
				cputs(" ");
			}
		}
	}
	if(!newGameAttempt&&!exitAttempt&&!puzzleLoaded){
		textcolor(YELLOW);
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS);
		cputs("To play new game press");
		textcolor(BLACK);
		textbackground(YELLOW);
		gotoxy(TEXT_MENU_SIDE+23,MENU_SECTION_POS);
		cputs("N");
		textcolor(WHITE);
		textbackground(TEXTBACKGROUND);
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
		cputs("To exit press");
		textcolor(BLACK);
		textbackground(WHITE);
		gotoxy(TEXT_MENU_SIDE+14,MENU_SECTION_POS+2);
		cputs("ESC");
		textcolor(WHITE);
		textbackground(TEXTBACKGROUND);	
	}
	else if(exitAttempt){
		textcolor(YELLOW);
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS);
		cputs(igrok);
		cputs(", You are going to exit.");
		gotoxy(TEXT_MENU_SIDE, MENU_SECTION_POS+1);
		cputs("Proceed?");
		textcolor(WHITE);
		textbackground(RED);
		gotoxy(TEXT_MENU_SIDE+6,MENU_SECTION_POS+3);
		cputs("_Y_es");
		gotoxy(TEXT_MENU_SIDE+12,MENU_SECTION_POS+3);
		cputs("_N_o");
		textbackground(TEXTBACKGROUND);	
	}
	else if(newGameAttempt&&selected_size!=10){
		textcolor(YELLOW);
		gotoxy(TEXT_MENU_SIDE, MENU_SECTION_POS);
		cputs("You name?");
		gotoxy(TEXT_MENU_SIDE, MENU_SECTION_POS + 2);
		hellow();
		gotoxy(TEXT_MENU_SIDE+12, NAME_POS );		
		cputs("                   ");
		gotoxy(TEXT_MENU_SIDE+12, NAME_POS );
		cputs(igrok); //NAME

		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS);
		cputs("Select size:");
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
		cputs("Press 1 to 6x6");
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+3);
		cputs("Press 2 to 8x8");
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+4);
		cputs("Press 3 to 10x10");
		textcolor(BLACK);
		textbackground(YELLOW);
		gotoxy(TEXT_MENU_SIDE+6,MENU_SECTION_POS+2);
		cputs("1");
		gotoxy(TEXT_MENU_SIDE+6,MENU_SECTION_POS+3);
		cputs("2");
		gotoxy(TEXT_MENU_SIDE+6,MENU_SECTION_POS+4);
		cputs("3");
		textcolor(WHITE);
		textbackground(TEXTBACKGROUND);
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+6);
		cputs("To previous menu press");
		textcolor(BLACK);
		textbackground(WHITE);
		gotoxy(TEXT_MENU_SIDE+23,MENU_SECTION_POS+6);
		cputs("ESC");
		textcolor(WHITE);
		textbackground(TEXTBACKGROUND);	
		
	}
	else if(newGameAttempt&&selected_size==10){
		textcolor(YELLOW);
		

		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS);
		cputs("10x10 sized puzzle would");
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+1);
		cputs("generate big amount of");
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
		cputs("time. Proceed?");
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+4);
		cputs("Yes");
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+5);
		cputs("No");
		textcolor(BLACK);
		textbackground(YELLOW);
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+4);
		cputs("Y");
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+5);
		cputs("N");
		textcolor(WHITE);
		textbackground(TEXTBACKGROUND);	
	}
	else if(puzzleLoaded){
		textcolor(YELLOW);
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS);
		cputs("To navigate use arrows");
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+1);
		cputs("use 1 and 0 buttons to");
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
		cputs("fill gaps. Rules:");
		textcolor(WHITE);
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+3);
		cputs("1) No trios in row or column");
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+4);
		cputs("2) sum of 1's in rows and ");
		gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+5);
		cputs("   columns equal");
		
		
	}
	
}
void printPuzzle(){
	textbackground(TEXTBACKGROUND);
	for(int i=0;i<18;i++){
		for(int j=0;j<38;j++){
			gotoxy(PUZZLE_SIDE+j,TEXTBLOCK_POS1+i);
			cputs(" ");
		}
		
	}
	
	if(!puzzleLoaded){
		textcolor(YELLOW);
		textbackground(TEXTBACKGROUND);
		gotoxy(PUZZLE_SIDE+3, PAZZLEBLOCK_POS);  cputs("  ____  _                        ");
		gotoxy(PUZZLE_SIDE+3, PAZZLEBLOCK_POS +1);  cputs(" |  _ \\(_)                       ");
		gotoxy(PUZZLE_SIDE+3, PAZZLEBLOCK_POS +2);  cputs(" | |_) |_ _ __   __ _ _ __ _   _ ");
		gotoxy(PUZZLE_SIDE+3, PAZZLEBLOCK_POS +3);  cputs(" |  _ <| | '_ \\ / _` | '__| | | |");
		gotoxy(PUZZLE_SIDE+3, PAZZLEBLOCK_POS +4);  cputs(" | |_) | | | | | (_| | |  | |_| |");
		gotoxy(PUZZLE_SIDE+3, PAZZLEBLOCK_POS +5);  cputs(" |____/|_|_| |_|\\__,_|_|   \\__, |");
		gotoxy(PUZZLE_SIDE+3, PAZZLEBLOCK_POS +6);  cputs(" |  __ \\             | |    __/ |");
		gotoxy(PUZZLE_SIDE+3, PAZZLEBLOCK_POS +7);  cputs(" | |__) |   _ _______| | __|___/ ");
		gotoxy(PUZZLE_SIDE+3, PAZZLEBLOCK_POS +8);  cputs(" |  ___/ | | |_  /_  / |/ _ \\    ");
		gotoxy(PUZZLE_SIDE+3, PAZZLEBLOCK_POS +9);  cputs(" | |   | |_| |/ / / /| |  __/    ");
		gotoxy(PUZZLE_SIDE+3, PAZZLEBLOCK_POS +10);  cputs(" |_|    \\__,_/___/___|_|\\___| ");
		}
		else {
			
			textbackground(TEXTBACKGROUND);
			for(int i=0;i<sizej;i++){
				for(int j=0;j<sizei;j++){
					textcolor(YELLOW);
					char theval[2] = {0};
					theval[0]=puzzle.cmatrix[i][j];
					gotoxy(PUZZLE_SIDE+i,TEXTBLOCK_POS1+j);
					cputs(theval);
					if(puzzle.umatrix[i][j]!=' '){
						textcolor(CYAN);
						char theval1[2] = {0};
						theval1[0]=puzzle.umatrix[i][j];
						gotoxy(PUZZLE_SIDE+i,TEXTBLOCK_POS1+j);
						if(CHEATS){
							textcolor(BLACK);
							if(puzzle.umatrix[i][j]==puzzle.ncmatrix[i][j])textbackground(GREEN);
							else {
								if (errznak) textbackground(RED);
								else textbackground(GREEN);
							}
						}
						cputs(theval1);
						textbackground(TEXTBACKGROUND);
					}
				}
				
			}
			
		}
}

Matrix generate(int size) { //generuje pierwszą matrycę
	bool **matrix;
	matrix = new bool*[size];
	for (int i = 0; i<size; i++) {
		matrix[i] = new bool[size] ; 
	}
	
	bool incorrect = false;
	srand(time(NULL));
	int attemptsCaclc = 0;
	int lastLine = -1;
	for (int i = 0; i<size; i++) {
		int countj = 0;
		for (int j = 0; j<size; j++) {
			srand(rand());
			bool value = rand() % 2;
			bool byin = false;
			bool byjn = false;
			bool byi = false;
			bool byj = false;
			if (i - 2 >= 0) {
				if (matrix[j][i - 1] == matrix[j][i - 2] && matrix[j][i - 2] == value)byi = true;
				if (matrix[j][i - 1] == matrix[j][i - 2])byin = true;
			}
			if (j - 2 >= 0) {
				if (matrix[j - 1][i] == matrix[j - 2][i] && matrix[j - 2][i] == value)byj = true;
				if (matrix[j - 1][i] == matrix[j - 2][i])byjn = true;
			}


			if (byjn&&byin) {
				if (byj == byi) {
					if (byi || byj) {
						matrix[j][i] = !value;
					}
					else {
						matrix[j][i] = value;
					}

				}
				else {
					incorrect = true;					
				}
			}
			else if (byi || byj) {
				matrix[j][i] = !value;
			}
			else {

				matrix[j][i] = value;
			}

			if (incorrect)break;
			if (matrix[j][i])countj++;

		}
		if (countj != size / 2) {
			if (lastLine == i)attemptsCaclc++;
			else {
				attemptsCaclc = 1;
				lastLine = i;
			}
			--i;
		}
		countj = 0;
		if (attemptsCaclc == 100) {
			incorrect = true;
			break;
		}
	}
	for (int i = 0; i<size; i++) {
		int counti = 0;
		for (int j = 0; j<size; j++) {
			if (matrix[i][j])counti++;
		}
		if (counti != size / 2) {
			incorrect = true;
			break;
		}
	}
	
	Matrix values;
	values.matrix = matrix;
	values.correct = !incorrect;
	
	return values;
}
Matrix process(Matrix matrix){ //wykonuje wypełnianie pozostałych macierzy
	
	int matrix_size = selected_size; 

	
	bool **a;	
	a = new bool*[matrix_size];	
	for (int i = 0; i<matrix_size; i++) {
		a[i] = new bool[matrix_size] ;		
	}
	for (int i = 0; i < matrix_size; i++)
		for (int j = 0; j < matrix_size; j++)
			a[i][j] = false;

	
	matrix.mmatrix = a;
	
	srand (time(NULL));
	for(int i=0;i<matrix_size;i++)for (int j=0;j<matrix_size;j++){		
		srand (rand());
		if (rand() % 2==0) matrix.mmatrix[j][i]=false;   //   które bloki otworzyć
		else matrix.mmatrix[j][i] = true;
	}

	count1s=0;
	for(int i=0;i<matrix_size;i++)for(int j=0;j<matrix_size;j++){
		if(!matrix.mmatrix[i][j])count1s++;               //liczba ilości ukrytych bloków
	}
	//generowanie rozmiaru okna wyświetlania
	sizej =  matrix_size * 3 + 1;				//hight 18+1  
	sizei =  matrix_size + matrix_size/2 + 1;   //width 10	  

	char **a_ch;
	char **a_um;
	char **a_nc;
	a_ch = new char*[sizej];
	a_um = new char*[sizej];
	a_nc = new char*[sizej];
	
	

	for (int j = 0; j<sizej; j++) {
		a_ch[j] = new char[sizei];
		a_um[j] = new char[sizei];
		a_nc[j] = new char[sizei];
	}	

	
	for (int i = 0; i < sizej; i++)
		for (int j = 0; j < sizei; j++){
			a_ch[i][j] = ' ';
			a_um[i][j] = ' ';
			a_nc[i][j] = ' ';
		}
	matrix.cmatrix = a_ch;
	matrix.umatrix = a_um;
	matrix.ncmatrix = a_nc;
	
	

	for(int i=0;i<sizei;i++){
		for(int j=0;j<sizej;j++){
			if(i==0&&j==0)matrix.cmatrix[j][i]=218;							//left top
			else if (i==(sizei-1)&&j==0)matrix.cmatrix[j][i]=192;			//left bott	
			else if (i==0&&j==(sizej-1))matrix.cmatrix[j][i]=191;			//right top
			else if (i==(sizei-1)&&j==(sizej-1))matrix.cmatrix[j][i]=217;   // right bott
			else if ((j%6)!=0&&(i%3)==0)matrix.cmatrix[j][i]=196;			//----
			else if (i==0&&(j%6)==0)matrix.cmatrix[j][i]=194;				// T
			else if ((i%3)==0&&j==0)matrix.cmatrix[j][i]=195;
			else if (i==(sizei-1)&&(j%6)==0)matrix.cmatrix[j][i]=193;
			else if ((i%3)==0&&j==(sizej-1))matrix.cmatrix[j][i]=180;
			else if ((i%3)!=0&&(j%6)==0)matrix.cmatrix[j][i]=179;			//||
			else if ((((i%3)==0&&(j%6)==0)&&(i!=0&&i!=(sizei-1))&&(j!=0&&j!=(sizej-1))))matrix.cmatrix[j][i]=197;
			else if(i%3!=0&&(j%6)%2==0){
					if(matrix.matrix[(j-1)/3][i-i/3-1])matrix.ncmatrix[j][i]=49;    //1
					else matrix.ncmatrix[j][i]=48;										//0
					if(matrix.mmatrix[(j-1)/3][i-i/3-1]){
						if(matrix.matrix[(j-1)/3][i-i/3-1])matrix.cmatrix[j][i]=49;
						else matrix.cmatrix[j][i]=48;	
					}else matrix.cmatrix[j][i]='_';
			}
		}
	}	
	

	return matrix;
} 
void startGenerate(){
	textcolor(YELLOW);
	for (int i=TEXT_MENU_SIDE-2;i<TEXT_MENU_SIDE+32;i++){
		for (int j=MENU_SECTION_POS-2;j<MENU_SECTION_POS+8;j++){
			if(i==TEXT_MENU_SIDE-2||i==TEXT_MENU_SIDE+31||j==MENU_SECTION_POS-2||j==MENU_SECTION_POS+7){
				gotoxy(i,j);
				cputs("-");
			}
			else {
				gotoxy(i,j);
				cputs(" ");
			}
		}
	}
	textcolor(YELLOW);
	gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS);
	cputs("Generating matrix,");
	gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+1);
	cputs("please, wait");
	gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
	cputs("|---------");
	int count=0;
	bool correct=false;
	while(!correct){
		
		puzzle=generate(selected_size);			 		
		correct=puzzle.correct;
		count++;
		switch(count%100000){
			case 0:{
				gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
				cputs("|---------");
				break;
			}
			case 10000:{
				gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
				cputs("-|--------");
				break;
			}
			case 20000:{
				gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
				cputs("--|-------");
				break;
			}
			case 30000:{
				gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
				cputs("---|------");
				break;
			}
			case 40000:{
				gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
				cputs("----|-----");
				break;
			}
			case 50000:{
				gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
				cputs("-----|----");
				break;
			}
			case 60000:{
				gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
				cputs("------|---");
				break;
			}
			case 70000:{
				gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
				cputs("-------|--");
				break;
			}
			case 80000:{
				gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
				cputs("--------|-");
				break;
			}
			case 90000:{
				gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+2);
				cputs("---------|");
				break;
			}
			
		}
		//correct = true;
	}
	
	textcolor(WHITE);
	puzzle=process(puzzle);
	
	loading=false;
	puzzleLoaded=true;
	showMainMenu();
	
}
void setNextCursor(int dimention){
	if(puzzleLoaded){
		switch(dimention){
			case UP:{
				if(cursorMY!=0)cursorMY--;
				break;
			}
			case DOWN:{
				if(cursorMY!= sizej -1)cursorMY++;
				break;
			}
			case LEFT:{
				if(cursorMX!=0)cursorMX--;
				
				break;
			}
			case RIGHT:{
				if(cursorMX!= sizej -1)cursorMX++;
				
				break;
			}
		}
		cursorY=cursorMY+1+cursorMY/2+TEXTBLOCK_POS1;
		cursorX=2*(cursorMX+1)+2*(cursorMX/2)+PUZZLE_SIDE;
		gotoxy(TEXT_MENU_SIDE+9, KURSOR_POS);
		char str[3];		
		sprintf(str, "%d", cursorMX); 
		cputs( str );
		gotoxy(TEXT_MENU_SIDE+15, KURSOR_POS);
		sprintf(str, "%d", cursorMY);
		cputs( str );
	}
}
void setValue(bool value){ //set 0 1
	if(puzzleLoaded){
		if(!puzzle.mmatrix[cursorMX][cursorMY]){
			if(value){
				puzzle.umatrix[cursorX-PUZZLE_SIDE][cursorY-TEXTBLOCK_POS1]='1';
			}
			else{
				puzzle.umatrix[cursorX-PUZZLE_SIDE][cursorY-TEXTBLOCK_POS1]='0';
			}
		}
		wonned=won(); //po zainstalowaniu 1 lub 0 - próba wygrane
		
	    
		
	}
}
bool won(){
	int count=0;
	for(int i=0;i<sizei;i++){
		for(int j=0;j<sizej;j++){
			if(puzzle.umatrix[j][i]!=' ')count++;
		}
	}
	bool won=false;
	int countc=0;
	if(count1s==count){
		for(int i=0;i<sizei;i++){
			for(int j=0;j<sizej;j++){
				if(puzzle.umatrix[j][i]==puzzle.ncmatrix[j][i]&&puzzle.ncmatrix[j][i]!=' ')countc++;
			}
		}
	}
	gotoxy(81,23);
	bool wonned1=(count1s==countc);
		
	return wonned1;
}
void printWon(){ 
	if(wonned){	
		textcolor(GREEN);
		for (int i=TEXT_MENU_SIDE-2;i<TEXT_MENU_SIDE+32;i++){
			for (int j=MENU_SECTION_POS-2;j<MENU_SECTION_POS+8;j++){
				if(i==TEXT_MENU_SIDE-2||i==TEXT_MENU_SIDE+31||j==MENU_SECTION_POS-2||j==MENU_SECTION_POS+7){
					gotoxy(i,j);
					cputs("%");
				}
				else {
					gotoxy(i,j);
					cputs(" ");
				}
			}
		}
	
	gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS);
	cputs("   Congratulations!");
	gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+1);
	cputs("You solved this puzzle");
	gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+3);
	textcolor(WHITE);
	cputs(" To exit to main menu");
	gotoxy(TEXT_MENU_SIDE,MENU_SECTION_POS+4);
	cputs("      press ESC");
	}

}
void exitmem(){
	//clear array
	for(int i = 0; i < selected_size; ++i) {
		delete [] puzzle.matrix[i];
	}
	delete [] puzzle.matrix;

	for(int i = 0; i < selected_size; ++i) {
		delete [] puzzle.mmatrix[i];
	}
	delete [] puzzle.mmatrix;

	for(int i = 0; i < sizej; ++i) {
		delete [] puzzle.cmatrix[i];
	}
	delete [] puzzle.cmatrix;

	for(int i = 0; i < sizej; ++i) {
		delete [] puzzle.umatrix[i];
	}
	delete [] puzzle.umatrix;

		for(int i = 0; i < sizej; ++i) {
		delete [] puzzle.ncmatrix[i];
	}
	delete [] puzzle.ncmatrix;


}
void savegame() {
	
	FILE* f;

	int err = fopen_s( &f, saveigr, "w+t");

	if (err == 0){
		gotoxy(TEXT_MENU_SIDE+15, HOT_KEYS + 2);
		cputs(" saved ");
		
		fputs(igrok, f); //igrok
		fputs("\n", f);
		fprintf(f, "%d", selected_size);
		
		fputs("\n", f);
		bool nn = false;
		for (int i = 0; i < selected_size; i++) {
			for (int j = 0; j < selected_size; j++) {	
				if (puzzle.matrix[i][j])fputs("1", f);
				else fputs("0", f);				
			}
		}fputs("\n", f);
		for (int i = 0; i < selected_size; i++) {
			for (int j = 0; j < selected_size; j++) {
				if (puzzle.mmatrix[i][j])fputs("1", f);
				else fputs("0", f);
			}
		}fputs("\n", f);
		//count1s
		fprintf(f, "%d", count1s);
		fputs("\n", f);
		fputs("end", f);
		fputs("\n", f);


	}else{
		gotoxy(TEXT_MENU_SIDE+15, HOT_KEYS + 2);
		cputs("Error save");
	}

	_fcloseall();
	loading = false;
	puzzleLoaded = true;
	newGameAttempt = false;
	showMainMenu();
}
void loadgame() {
	
	FILE* f;
	if (!saveigr ) {
		gotoxy(TEXT_MENU_SIDE+15, HOT_KEYS + 3);
		cputs("Error name:null");
		return;
	}

	int err = fopen_s(&f, saveigr, "r");

	if (err == 0) {
		
		gotoxy(TEXT_MENU_SIDE + 15, HOT_KEYS + 3);
		cputs(" loaded ");
		char temp[128];
		int z = 0;	//linia pliku	
		
		char vacancy[4];
		//Matrix loadmatr;
		//bool **matrix, **mmatrix;

		while (!feof(f)) { //na razie nie koniec plika
			if (fgets(temp, 128, f)) { //czytanie linii
				int m = 0;
				if (z == 2) { //czytanie głównej matrycy
					for (int i = 0; i < selected_size; i++) {
						for (int j = 0; j < selected_size; j++) {
							if (temp[m] == '1')	puzzle.matrix[i][j] = true;
							if (temp[m] == '0')	puzzle.matrix[i][j] = false;
							m++;
						}
					}
				}
				m = 0;
				if (z == 3) { //matryca ukrytych pozicij
					for (int i = 0; i < selected_size; i++) {
						for (int j = 0; j < selected_size; j++) {
							if (temp[m] == '1')	puzzle.mmatrix[i][j] = true;
							if (temp[m] == '0')	puzzle.mmatrix[i][j] = false;
							m++;
						}
					}
					
				}
				for (int i = 0; i < 128; i++) {

					if (z == 1) { //tutaj jest rozmiar matrycy
						
						if (temp[0] == '6') selected_size = 6;
						else if (temp[0] == '8') selected_size = 8;
						else if ((temp[0] == '1') && (temp[1] == '0')) { selected_size = 10;
												break;
						}
						
					}
					;
					if (z == 4) { //count1s licznik ukrytych pozycji
						vacancy[i]= temp[i];
						
					}
					if (temp[i] == '\n') {
						break;
					}
				}

			}
			if (z == 1) {
				puzzle.matrix = new bool*[selected_size];
				for (int im = 0; im < selected_size; im++) {
					puzzle.matrix[im] = new bool[selected_size];
				}
				puzzle.mmatrix = new bool*[selected_size];
				for (int im = 0; im < selected_size; im++) {
					puzzle.mmatrix[im] = new bool[selected_size];
				}
			}
			z++;
		}
		
		
		//generate main view;
		sizej = selected_size * 3 + 1;				//hight 18+1  
		sizei = selected_size + selected_size / 2 + 1;   //width 10	  
		puzzle.correct = true;
		puzzle.cmatrix = new char*[sizej];
		puzzle.umatrix = new char*[sizej];
		puzzle.ncmatrix = new char*[sizej];

		for (int j = 0; j < sizej; j++) {
			puzzle.cmatrix[j] = new char[sizei];
			puzzle.umatrix[j] = new char[sizei];
			puzzle.ncmatrix[j] = new char[sizei];
		}
		int count4 = 0,count5=0;
		for (int i = 0; i < sizej; i++) {
			for (int j = 0; j < sizei; j++) {
				puzzle.cmatrix[i][j] = 32; //przerwa
				puzzle.umatrix[i][j] = 32;
				puzzle.ncmatrix[i][j] = 32;
				
			}
			
		}

		for (int i = 0; i < sizei; i++) {
			for (int j = 0; j < sizej; j++) {
				if (i == 0 && j == 0)puzzle.cmatrix[j][i] = 218;							//left top
				else if (i == (sizei - 1) && j == 0)puzzle.cmatrix[j][i] = 192;			//left bott	
				else if (i == 0 && j == (sizej - 1))puzzle.cmatrix[j][i] = 191;			//right top
				else if (i == (sizei - 1) && j == (sizej - 1))puzzle.cmatrix[j][i] = 217;   // right bott
				else if ((j % 6) != 0 && (i % 3) == 0)puzzle.cmatrix[j][i] = 196;			//----
				else if (i == 0 && (j % 6) == 0)puzzle.cmatrix[j][i] = 194;				// T
				else if ((i % 3) == 0 && j == 0)puzzle.cmatrix[j][i] = 195;
				else if (i == (sizei - 1) && (j % 6) == 0)puzzle.cmatrix[j][i] = 193;
				else if ((i % 3) == 0 && j == (sizej - 1))puzzle.cmatrix[j][i] = 180;
				else if ((i % 3) != 0 && (j % 6) == 0)puzzle.cmatrix[j][i] = 179;			//||
				else if ((((i % 3) == 0 && (j % 6) == 0) && (i != 0 && i != (sizei - 1)) && (j != 0 && j != (sizej - 1))))puzzle.cmatrix[j][i] = 197;
				else if (i % 3 != 0 && (j % 6) % 2 == 0) {
					if (puzzle.matrix[(j - 1) / 3][i - i / 3 - 1])puzzle.ncmatrix[j][i] = 49;    //1
					else puzzle.ncmatrix[j][i] = 48;										//0
					if (puzzle.mmatrix[(j - 1) / 3][i - i / 3 - 1]) {
						if (puzzle.matrix[(j - 1) / 3][i - i / 3 - 1])puzzle.cmatrix[j][i] = 49;
						else puzzle.cmatrix[j][i] = 48;
					}
					else puzzle.cmatrix[j][i] = '_';
				}
			}
		}
		
		count1s = atoi(vacancy);
		loading = false;
		puzzleLoaded = true;
		newGameAttempt = false;
		showMainMenu();
	}
	else {
		gotoxy(TEXT_MENU_SIDE + 15, STUDENTID_POS + 2);
		cputs("Error load");
	}

	_fcloseall();
	
}
