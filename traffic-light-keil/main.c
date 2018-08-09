#include <reg51.h>

//states							// B1, B2, 	N1,  N2,  M1,  M2,  T1,  T2
unsigned char ST0[8] = {'G', 'G', 'R', 'G', 'R', 'G', 'R', 'G'};
unsigned char ST1[8] = {'Y', 'G', 'R', 'G', 'R', 'G', 'R', 'G'};
unsigned char ST2[9] = {'R', 'G', 'R', 'G', 'Y', 'G', 'R', 'G', '0'};
unsigned char ST3[8] = {'R', 'G', 'R', 'G', 'G', 'G', 'R', 'G'};
unsigned char ST4[9] = {'R', 'G', 'R', 'G', 'Y', 'G', 'R', 'G', '1'};
unsigned char ST5[9] = {'R', 'G', 'R', 'G', 'R', 'G', 'Y', 'G', '0'};
unsigned char ST6[8] = {'R', 'G', 'R', 'G', 'R', 'G', 'G', 'G'};
unsigned char ST7[9] = {'R', 'G', 'R', 'G', 'R', 'G', 'Y', 'G', '1'};
unsigned char ST8[8] = {'R', 'Y', 'R', 'Y', 'R', 'Y', 'R', 'Y'};
unsigned char ST9[8] = {'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R'};
unsigned char ST10[8] = {'Y', 'Y', 'R', 'Y', 'R', 'Y', 'R', 'Y'};
unsigned char ST11[8] = {'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'Y'};
unsigned char *present_state = ST0;
unsigned char *next_state = ST0;


// sensor inputs / interrupts
sbit urgent_B = P2^0;	// emergency vehicle approaching from Bagmati side
sbit urgent_N = P2^1; // emergency vehicle approaching from Norvic side
sbit urgent_M = P2^2; // emergency vehicle approaching from Maitighar side
sbit urgent_T = P2^3; // emergency vehicle approaching from Tripureshwors side

sbit vehicle_B = P2^4; // at least one vehicle approaching from Bagmati side
sbit vehicle_N = P2^5; // at least one vehicle approaching from Norvic side 
sbit vehicle_M = P2^6; // at least one vehicle approaching from Maitighar side
sbit vehicle_T = P2^7; // at least one vehicle approaching from Tripureshwor side

// traffic lights

// port 0
sbit T1_R = P0^0;	// T1 traffic light controller of color RED at Tripureshwor side
sbit T1_Y = P0^1;	// T1 traffic light controller of color YELLOW at Tripureshwor side
sbit T1_G = P0^2;	// T1 traffic light controller of color GREEN at Tripureshwor side

sbit T2_R = P0^3;	// T2 traffic light controller of color RED at Tripureshwor side
sbit T2_Y = P0^4;	// T2 traffic light controller of color YELLOW at Tripureshwor side
sbit T2_G = P0^5;	// T2 traffic light controller of color GREEN at Tripureshwor side

sbit M1_R = P0^6; // M1 traffic light controller of color RED at Maitighar side
sbit M2_R = P0^7; // M2 traffic light controller of color RED at Maitighar side

// port 1
sbit B1_R = P1^0; // B1 traffic light controller of color RED at Bagmati side
sbit B1_Y = P1^1; // B1 traffic light controller of color YELLOW at Bagmati side
sbit B1_G = P1^2; // B1 traffic light controller of color GREEN at Bagmati side

sbit B2_R = P1^3; // B2 traffic light controller of color RED at Bagmati side
sbit B2_Y = P1^4; // B2 traffic light controller of color YELLOW at Bagmati side
sbit B2_G = P1^5; // B2 traffic light controller of color GREEN at Bagmati side

sbit M1_Y = P1^6; // M1 traffic light controller of color YELLOW at Maitighar side
sbit M2_Y = P1^7; // M2 traffic light controller of color YELLOW at Maitighar side

// port 3
sbit N1_R = P3^0; // N1 traffic light controller of color RED at Norvice side
sbit N1_Y = P3^1; // N1 traffic light controller of color YELLOW at Norvice side
sbit N1_G = P3^2; // N1 traffic light controller of color GREEN at Norvice side

sbit N2_R = P3^3; // N2 traffic light controller of color RED at Norvice side
sbit N2_Y = P3^4; // N2 traffic light controller of color YELLOW at Norvice side
sbit N2_G = P3^5; // N2 traffic light controller of color GREEN at Norvice side

sbit M1_G = P3^6; // M1 traffic light controller of color GREEN at Maitighar side
sbit M2_G = P3^7; // M2 traffic light controller of color GREEN at Maitighar side

void delay3s(void);
void delay51s(void);
void delay3sAlways(void);
int equal(unsigned char *lhs, unsigned char *rhs, int n);
void display(unsigned char *state);

void main(void){
		
	// initialize everything to zero
	P0 = 0x00;	// output pin
	P1 = 0x00;	// output pin
	P2 = 0x00;	// input pin
	P3 = 0x00;	// output pin
	
	while(1){

		if (equal((unsigned char *)present_state, (unsigned char *)ST0, 8) == 1){
			
			// B1:GREEN, B2:GREEN, T1:RED, T2:GREEN, N1:RED, N2:GREEN, M1:RED, M2:GREEN
			display((unsigned char *)present_state);
			
			delay51s();
			
			if (urgent_B == 1){
				urgent_B = 0;
				next_state = ST0;
			} else if (urgent_B == 0 && (urgent_T == 1 || urgent_M == 1) ){
				next_state = ST11;
			} else if (vehicle_T == 1 || vehicle_N == 1 || vehicle_M == 1 ){
				next_state = ST1;
			} else {
				next_state = ST0;
			}
			
		} else if (equal((unsigned char *)present_state, (unsigned char *)ST1, 8) == 1){
			
			// B1:YELLOW, B2:GREEN, T1:RED, T2:GREEN, N1:RED, N2:GREEN, M1:RED, M2:GREEN
			display((unsigned char *)present_state);
			
			delay3s();
			
			if (urgent_B == 1 || urgent_T == 1 || urgent_M == 1) {
				next_state = ST11;
			} else {
				next_state = ST2;
			}
			
		} else if (equal((unsigned char *)present_state, (unsigned char *)ST2, 9) == 1) {
			
			// B1:RED, B2:GREEN, T1:RED, T2:GREEN, N1:RED, N2:GREEN, M1:YELLOW, M2:GREEN
			display((unsigned char *)present_state);
			
			delay3s();
			
			if (urgent_B == 1 || urgent_T == 1 || urgent_M == 1) {
				next_state = ST11;
			} else {
				next_state = ST3;
			}
			
		} else if (equal((unsigned char *)present_state, (unsigned char *)ST3, 8) == 1){
			
			// B1:RED, B2:GREEN, T1:RED, T2:GREEN, N1:RED, N2:GREEN, M1:GREEN, M2:GREEN
			display((unsigned char *)present_state);
			
			delay51s();
			
			if (urgent_M == 1){
				urgent_M = 0;
				next_state = ST3;
			} else if (urgent_M == 0 && (urgent_T == 1 || urgent_B == 1) ){
				next_state = ST11;
			} else if (vehicle_T == 1 || vehicle_N == 1 || vehicle_M == 1){
				next_state = ST4;
			} else {
				next_state = ST3;
			}
			
		} else if (equal((unsigned char *)present_state, (unsigned char *)ST4, 9) == 1){
			
			// B1:RED, B2:GREEN, T1:RED, T2:GREEN, N1:RED, N2:GREEN, M1:YELLOW, M2:GREEN
			display((unsigned char *)present_state);
			
			delay3s();
			
			if (urgent_B == 1 || urgent_T == 1 || urgent_M == 1) {
				next_state = ST11;
			} else {
				next_state = ST5;
			}
			
		} else if (equal((unsigned char *)present_state, (unsigned char *)ST5, 9) == 1){
			
			// B1:RED, B2:GREEN, T1:YELLOW, T2:GREEN, N1:RED, N2:GREEN, M1:RED, M2:GREEN
			display((unsigned char *)present_state);
			
			delay3s();
			
			if (urgent_B == 1 || urgent_T == 1 || urgent_M == 1) {
				next_state = ST11;
			} else {
				next_state = ST6;
			}
			
		} else if (equal((unsigned char *)present_state, (unsigned char *)ST6, 8) == 1){
			
			// B1:RED, B2:GREEN, T1:GREEN, T2:GREEN, N1:RED, N2:GREEN, M1:RED, M2:GREEN
			display((unsigned char *)present_state);
			
			delay51s();
			
			if (urgent_T == 1){
				urgent_T = 0;
				next_state = ST6;
			} else if (urgent_T == 0 && (urgent_M == 1 || urgent_B == 1) ){
				next_state = ST11;
			} else if (vehicle_T == 1 || vehicle_N == 1 || vehicle_M == 1 ){
				next_state = ST7;
			} else {
				next_state = ST6;
			}
			
		} else if (equal((unsigned char *)present_state, (unsigned char *)ST7, 9) == 1){
			
			// B1:RED, B2:GREEN, T1:YELLOW, T2:GREEN, N1:RED, N2:GREEN, M1:RED, M2:GREEN
			display((unsigned char *)present_state);
			
			delay3s();
			
			if (urgent_B == 1 || urgent_T == 1 || urgent_M == 1) {
				next_state = ST11;
			} else {
				next_state = ST8;
			}
			
		} else if (equal((unsigned char *)present_state, (unsigned char *)ST8, 8) == 1){
			
			// B1:RED, B2:YELLOW, T1:RED, T2:YELLOW, N1:RED, N2:YELLOW, M1:RED, M2:YELLOW
			display((unsigned char *)present_state);
			
			delay3s();
			
			if (urgent_B == 1 || urgent_T == 1 || urgent_M == 1) {
				next_state = ST11;
			} else {
				next_state = ST9;
			}
			
		} else if (equal((unsigned char *)present_state, (unsigned char *)ST9, 8) == 1){
			
			// B1:RED, B2:RED, T1:RED, T2:RED, N1:RED, N2:RED, M1:RED, M2:RED
			display((unsigned char *)present_state);
			
			delay51s();
			
			if (urgent_T == 0 || urgent_M == 1 || urgent_B == 1){
				next_state = ST11;
			} else if (vehicle_T == 1 || vehicle_N == 1 || vehicle_M == 1 || vehicle_B == 1 ){
				next_state = ST10;
			} else {
				next_state = ST9;
			}
		} else if (equal((unsigned char *)present_state, (unsigned char *)ST10, 8) == 1){
			
			// B1:YELLOW, B2:YELLOW, T1:RED, T2:YELLOW, N1:RED, N2:YELLOW, M1:RED, M2:YELLOW
			display((unsigned char *)present_state);
			
			delay3s();
			
			if (urgent_B == 1 || urgent_T == 1 || urgent_M == 1) {
				next_state = ST11;
			} else {
				next_state = ST0;
			}
			
		} else if (equal((unsigned char *)present_state, (unsigned char *)ST11, 8) == 1){
			
			// B1:YELLOW, B2:YELLOW, T1:YELLOW, T2:YELLOW, N1:YELLOW, N2:YELLOW, M1:YELLOW, M2:YELLOW
			display((unsigned char *)present_state);
			
			delay3sAlways();
			
			if (urgent_B == 1){
				urgent_B = 0;
				next_state = ST0;
			} else if (urgent_M == 1){
				urgent_M = 0;
				next_state = ST3;
			} else if (urgent_T == 1){
				urgent_T = 0;
				next_state = ST6;
			} else {
				next_state = ST0;
			}
			
		} else {
			next_state = ST0;
		}
		
		present_state = next_state;
		
	} // end while(1)
} // end main()

void display(unsigned char *state){
	
	P0 = 0x00;
	P1 = 0x00;
	P3 = 0x00;
	
	if (state[0] == 'R'){
		B1_R = 1;
	} else if (state[0] == 'Y'){
		B1_Y = 1;
	} else if (state[0] == 'G'){
		B1_G = 1;
	} else {	// error state all are on!
		B1_R = 1; B1_Y = 1; B1_G = 1;
	}
	
	if (state[1] == 'R'){
		B2_R = 1;
	} else if (state[1] == 'Y'){
		B2_Y = 1;
	} else if (state[1] == 'G'){
		B2_G = 1;
	} else {	// error state all are on!
		B2_R = 1; B2_Y = 1; B2_G = 1;
	}
	
	if (state[2] == 'R'){
		N1_R = 1;
	} else if (state[2] == 'Y'){
		N1_Y = 1;
	} else if (state[2] == 'G'){
		N1_G = 1;
	} else {	// error state all are on!
		N1_R = 1; N1_Y = 1; N1_G = 1;
	}
	
	if (state[3] == 'R'){
		N2_R = 1;
	} else if (state[3] == 'Y'){
		N2_Y = 1;
	} else if (state[3] == 'G'){
		N2_G = 1;
	} else {	// error state all are on!
		N2_R = 1; N2_Y = 1; N2_G = 1;
	}
	
	if (state[4] == 'R'){
		M1_R = 1;
	} else if (state[4] == 'Y'){
		M1_Y = 1;
	} else if (state[4] == 'G'){
		M1_G = 1;
	} else {	// error state all are on!
		M1_R = 1; M1_Y = 1; M1_G = 1;
	}
	
	if (state[5] == 'R'){
		M2_R = 1;
	} else if (state[5] == 'Y'){
		M2_Y = 1;
	} else if (state[5] == 'G'){
		M2_G = 1;
	} else {	// error state all are on!
		M2_R = 1; M2_Y = 1; M2_G = 1;
	}
	
	if (state[6] == 'R'){
		T1_R = 1;
	} else if (state[6] == 'Y'){
		T1_Y = 1;
	} else if (state[6] == 'G'){
		T1_G = 1;
	} else {	// error state all are on!
		T1_R = 1; T1_Y = 1; T1_G = 1;
	}
	
	if (state[7] == 'R'){
		T2_R = 1;
	} else if (state[7] == 'Y'){
		T2_Y = 1;
	} else if (state[7] == 'G'){
		T2_G = 1;
	} else {	// error state all are on!
		T2_R = 1; T2_Y = 1; T2_G = 1;
	}
	
}

int equal(unsigned char *lhs, unsigned char *rhs, int n){
	// if equal returns 1 else returns 0
	int i; 
	for (i = 0; i < n; i++){
		if (lhs[i] != rhs[i]){
			return 0;
		}
	}
	return 1;
}

void delay51s(){
	
	unsigned int i, j;
	//i	= 18;
	i = 5;
	while (i--){
		j = 43;
		while (j--){
			TMOD = 0x01;	// timer 0 in mode 1
			TL0 = 0xD4;	
			TH0 = 0x04;
			TR0 = 1; 			// turn on timer 0
			while (TF0 == 0){	// wait for TF0 to roll over
				if (urgent_B == 1 || urgent_M == 1 || urgent_T == 1){
					TR0 = 0;	// turn off timer 0
					TF0 = 0;
					return;
				}
			}
			TR0 = 0;			// turn off timer 0
			TF0 = 0; 	
		}
	}
}

void delay3s(){
	
	unsigned int i;
	i	= 43;
	while (i--){
		TMOD = 0x01;	// timer 0 in mode 1
		TL0 = 0xD4;	
		TH0 = 0x04;
		TR0 = 1; 			// turn on timer 0
		while (TF0 == 0){	// wait for TF0 to roll over
			if (urgent_B == 1 || urgent_M == 1 || urgent_T == 1){
					TR0 = 0;	// turn off timer 0
					TF0 = 0;
					return;
			}
		}
		TR0 = 0;			// turn off timer 0
		TF0 = 0; 	
	}
}

void delay3sAlways(){
	
	unsigned int i;
	i	= 43;
	while (i--){
		TMOD = 0x01;	// timer 0 in mode 1
		TL0 = 0xD4;	
		TH0 = 0x04;
		TR0 = 1; 			// turn on timer 0
		while (TF0 == 0);	// wait for TF0 to roll over
		TR0 = 0;			// turn off timer 0
		TF0 = 0; 	
		
	}
}