#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
//Minh Tran
//02/16/2022
//This Program convert a hex to a float in
//IEEE floating-point format.
//To start the program, enter in number of fraction bits,
//exponent bits, and the hex value without 0x. The program 
//will do the calculation and output the result.
//Special case results: +inf, -inf, and NaA
//Ex: ./fp_parse 3 4 b9 -> -1.125 

void parse(unsigned long frac_bits, unsigned long exp_bits, unsigned long hex);
float bin_to_frac(int frac, unsigned long frac_bits);

int main(int argc, char *argv[]){

	//not enough inputs	
	if(argc != 4){
		return 1;
	}

	unsigned long frac_bits, exp_bits, hex;

	//Load the 3 arguments
	frac_bits = strtoul (argv[1], NULL, 0);
	exp_bits = strtoul (argv[2], NULL, 0);
	hex = strtoul (argv[3], NULL, 16);

	//if frac_bits or exp_bits out of range
	if(frac_bits < 2 || frac_bits > 10 || exp_bits < 3 || exp_bits > 5)
		return 1;

	//make hex digits user can enter
	int MAX = 4;

	//invalid hex or hex too big
	for(int i = 0; i <= MAX; ++i){
		if(argv[3][i] == '\0')
			break;
		if(!isxdigit(argv[3][i])){
			return 1;
		}
	}

	int bits = 1 + frac_bits + exp_bits;
	int hex_mask = (1 << bits) - 1;

	//if out of bound
	if((hex & hex_mask) != hex){
		return 1;
	}

	parse(frac_bits, exp_bits, hex);
	
	return 0;
}



void parse(unsigned long frac_bits, unsigned long exp_bits, unsigned long hex){
	//int sign;
	float S, E, M, V, Bias;

	//Isolate first sign bit
	if(hex>>(frac_bits+exp_bits)==0)
		//sign = 0;
		S = 0;
	else 
		//sign = 1;
		S = 1;

	Bias = pow(2,exp_bits-1)-1;

	//Isolate and copy exp bits
	int exp = ((1 << (exp_bits)) -1) << frac_bits; 
	exp = (exp & hex) >> frac_bits;

	//Isolate and copy frac bits
	int frac = ((1 << (frac_bits))-1);
	frac = frac & hex; 
	float frac_dec =  bin_to_frac(frac, frac_bits);

	//special value	
	if((exp ^ ((1 <<exp_bits) - 1))==0){
		//check(frac, sign);
		if(frac == 0 && S == 1){
			printf("-inf\n");
			return;
		}
		if(frac == 0 && S ==0){
			printf("+inf\n");
			return;
		}
		else
			printf("NaN\n");
		return;
	}

	//denormalised
	if(exp == 0){
		E = 1 - Bias;
		M = frac_dec;
	}

	//normalised
	else{
		E = exp - Bias;
		M = 1 + frac_dec;
	}
	V = powf(-1, S) * M * pow(2, E);
	printf ("%f", V);

}

float bin_to_frac(int frac, unsigned long frac_bits){
	float total = 0;

	while(frac > 0){

		total += (frac%2)*1/pow(2,frac_bits);

		//to the next frac bit
		frac = (frac >> 1);
		--frac_bits;
	}

	return total;
}
