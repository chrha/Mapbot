
#include <stdio.h>
int MAXSIZE = 50;

struct coordinate{
	int8_t pos_x;
	int8_t pos_y;
};

struct coordinate stack[50];
int numOfCord = 0;


int rmv_position(struct coordinate c);
void insert_position(struct coordinate c);

int rmv_position(struct coordinate c){
	for(int i=0;i<numOfCord;i++){
		if ((c.pos_x== stack[i].pos_x) && (c.pos_y == stack[i].pos_y)){
			for(int t=i;i<numOfCord-1;t++){
				stack[t]=stack[t+1];
			}
			numOfCord-=1;
			return 0;
		}
	}
	return 1;
}

void insert_position(struct coordinate c){

	for(int j=numOfCord; j>0; j--){
		stack[numOfCord+1]=stack[numOfCord];
	}
	stack[0]= c;
	numOfCord+=1;
}



