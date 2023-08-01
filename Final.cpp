//Final

#include <iostream>
#include <cstdio>
#include <fstream>
#include <cmath>
#include <filesystem>

#define x1 648		// 1/3 = 216
#define y1 486 		// 1/3 = 162
#define poorFilter3 1250
#define poorFilter5 200


// seperate , get 3x3 & 5x5 gradient , individual histograms , clear & poor histograms

int main(int argc, char *argv[]){

	FILE* inp;
	unsigned char img[y1][x1] , TX[3][3] , TY[3][3] , FX[5][5] , FY[5][5];
	
	int hist3[16] , hist5[16] , histPoor3[16] , histClear3[16] , histPoor5[16] , histClear5[16];
	
	int valH = 0;
	int valV = 0;
	int sum = 0;
	int gradient = 0;
	int gradMax3 = 0;
	int gradMax5 = 0;
	int clearCount = 0;
	int poorCount = 0;
	int range = 0;
	
	
	//3x3 horizontal
	TX[0][0] = -1; TX[0][1] = 0; TX[0][2] = 1; 
	TX[1][0] = -2; TX[1][1] = 0; TX[1][2] = 2;  
	TX[2][0] = -1; TX[2][1] = 0; TX[2][2] = 1;

	//3x3 vertical
	TY[0][0] = -1; TY[0][1] = -2; TY[0][2] = -1;    
	TY[1][0] =  0; TY[1][1] =  0; TY[1][2] =  0;    
	TY[2][0] =  1; TY[2][1] =  2; TY[2][2] =  1;
	
	//5x5 horizontal
	FX[0][0] = -1; FX[0][1] = -2; FX[0][2] = 0; FX[0][3] = 2; FX[0][4] = 1; 
	FX[1][0] = -2; FX[1][1] = -4; FX[1][2] = 0; FX[1][3] = 4; FX[1][4] = 2; 
	FX[2][0] = -4; FX[2][1] = -8; FX[2][2] = 0; FX[2][3] = 8; FX[2][4] = 4; 
	FX[3][0] = -2; FX[3][1] = -4; FX[3][2] = 0; FX[3][3] = 4; FX[3][4] = 2;
	FX[4][0] = -1; FX[4][1] = -2; FX[4][2] = 0; FX[4][3] = 2; FX[4][4] = 1;

	//5x5 vertical
	FY[0][0] = -1; FY[0][1] = -2; FY[0][2] = -4; FY[0][3] = -2; FY[0][4] = -1; 
	FY[1][0] = -2; FY[1][1] = -4; FY[1][2] = -8; FY[1][3] = -4; FY[1][4] = -2; 
	FY[2][0] =  0; FY[2][1] =  0; FY[2][2] =  0; FY[2][3] =  0; FY[2][4] =  0; 
	FY[3][0] =  2; FY[3][1] =  4; FY[3][2] =  8; FY[3][3] =  4; FY[3][4] =  2;
	FY[4][0] =  1; FY[4][1] =  2; FY[4][2] =  4; FY[4][3] =  2; FY[4][4] =  1;
	
	
	//---------------------------------------------------------------------------------------------------------------
	
	for (int k=0 ; k<argc ; k++){
	
		inp = fopen(argv[k], "rb");
		fread(img , 1 , y1*x1 , inp);
		
		
		std::cout << std::endl << argv[k] << std::endl;

		
		//3x3 gradient
		gradMax3 = 0;
		std::fill_n (hist3, 16, 0);
		for (int j=162 ; j<y1 - 162 ; j++){
			for (int i=216 ; i<x1 - 216 ; i++){
				
				
				valH =   (img[j - 1][i - 1] * TX[0][0]) + (img[j - 1][i] * TX[0][1]) + (img[j - 1][i + 1] * TX[0][2])
						+ (img[j][i - 1] * TX[1][0])     + (img[j][i] * TX[1][1])     + (img[j][i + 1] * TX[1][2])
						+ (img[j + 1][i - 1] * TX[2][0]) + (img[j + 1][i] * TX[2][1]) + (img[j + 1][i + 1] * TX[2][2]);
						
				sum = (img[j - 1][i + 1] * TX[0][2]) + 
					  (img[j][i + 1] * TX[1][2]) + 
					  (img[j + 1][i + 1] * TX[2][2]);	  
				if (sum == 0) sum = 1;
				
				valH = valH / sum;
				
						
				valV =   (img[j - 1][i - 1] * TY[0][0]) + (img[j - 1][i] * TY[0][1]) + (img[j - 1][i + 1] * TY[0][2])
						+ (img[j][i - 1] * TY[1][0])     + (img[j][i] * TY[1][1])     + (img[j][i + 1] * TY[1][2])
						+ (img[j + 1][i - 1] * TY[2][0]) + (img[j + 1][i] * TY[2][1]) + (img[j + 1][i + 1] * TY[2][2]);
						
				sum = (img[j + 1][i - 1] * TY[2][0]) + 
					  (img[j + 1][i] * TY[2][1]) + 
					  (img[j + 1][i + 1] * TY[2][2]);
				if (sum == 0) sum = 1;
				
				valV = valV / sum;
				
				
				gradient = sqrt( (valH*valH) + (valV*valV) );
				
				bool run = true;
				range = 0;
				while(run){
					if(gradient >= range && gradient < range + 16){
						hist3[range/16]++;
						run = false;
					}
					else if (gradient >= 256){
						hist3[15]++;
						run = false;
					}
					else if (gradient < 0){
						hist3[0]++;
						run = false;
					}
					
					range = range + 16;
				}
				
				if (gradient > gradMax3) gradMax3 = gradient;
				
			}
		}
		
		
		//5x5 gradient
		gradMax5 = 0;
		std::fill_n (hist5, 16, 0);
		for (int j=162 ; j<y1 - 162 ; j++){
			for (int i=216 ; i<x1 - 216 ; i++){
				
				
				valH   =   (img[j - 2][i - 2] * FX[0][0]) + (img[j - 2][i -1] * FX[0][1]) + (img[j - 2][i] * FX[0][2]) + (img[j - 2][i + 1] * FX[0][3]) + (img[j - 2][i + 2] * FX[0][4])
						+ (img[j - 1][i - 2] * FX[1][0]) + (img[j - 1][i -1] * FX[1][1]) + (img[j - 1][i] * FX[1][2]) + (img[j - 1][i + 1] * FX[1][3]) + (img[j - 1][i + 2] * FX[1][4])
						+ (img[j][i - 2] * FX[2][0]) + (img[j][i -1] * FX[2][1]) + (img[j][i] * FX[2][2]) + (img[j][i + 1] * FX[2][3]) + (img[j][i + 2] * FX[2][4])
						+ (img[j + 1][i - 2] * FX[3][0]) + (img[j + 1][i -1] * FX[3][1]) + (img[j + 1][i] * FX[3][2]) + (img[j + 1][i + 1] * FX[3][3]) + (img[j + 1][i + 2] * FX[3][4])
						+ (img[j + 2][i - 2] * FX[4][0]) + (img[j + 2][i -1] * FX[4][1]) + (img[j + 2][i] * FX[4][2]) + (img[j + 2][i + 1] * FX[4][3]) + (img[j + 2][i + 2] * FX[4][4]);
				
				sum = (img[j - 2][i + 1] * FX[0][3]) + (img[j - 2][i + 2] * FX[0][4]) + 
					  (img[j - 1][i + 1] * FX[1][3]) + (img[j - 1][i + 2] * FX[1][4]) +
					  (img[j][i + 1] * FX[2][3]) + (img[j][i + 2] * FX[2][4]) +
					  (img[j + 1][i + 1] * FX[3][3]) + (img[j + 1][i + 2] * FX[3][4]) +
					  (img[j + 2][i + 1] * FX[4][3]) + (img[j + 2][i + 2] * FX[4][4]);
				if (sum == 0) sum = 1;
				
				valH = valH / sum;
				
				
				valV   =   (img[j - 2][i - 2] * FY[0][0]) + (img[j - 2][i -1] * FY[0][1]) + (img[j - 2][i] * FY[0][2]) + (img[j - 2][i + 1] * FY[0][3]) + (img[j - 2][i + 2] * FY[0][4])
						+ (img[j - 1][i - 2] * FY[1][0]) + (img[j - 1][i -1] * FY[1][1]) + (img[j - 1][i] * FY[1][2]) + (img[j - 1][i + 1] * FY[1][3]) + (img[j - 1][i + 2] * FY[1][4])
						+ (img[j][i - 2] * FY[2][0]) + (img[j][i -1] * FY[2][1]) + (img[j][i] * FY[2][2]) + (img[j][i + 1] * FY[2][3]) + (img[j][i + 2] * FY[2][4])
						+ (img[j + 1][i - 2] * FY[3][0]) + (img[j + 1][i -1] * FY[3][1]) + (img[j + 1][i] * FY[3][2]) + (img[j + 1][i + 1] * FY[3][3]) + (img[j + 1][i + 2] * FY[3][4])
						+ (img[j + 2][i - 2] * FY[4][0]) + (img[j + 2][i -1] * FY[4][1]) + (img[j + 2][i] * FY[4][2]) + (img[j + 2][i + 1] * FY[4][3]) + (img[j + 2][i + 2] * FY[4][4]);
				
				sum = (img[j + 1][i - 2] * FY[3][0]) + (img[j + 1][i -1] * FY[3][1]) + 
					  (img[j + 1][i] * FY[3][2]) + (img[j + 1][i + 1] * FY[3][3]) + 
					  (img[j + 1][i + 2] * FY[3][4]) + (img[j + 2][i - 2] * FY[4][0]) + 
					  (img[j + 2][i -1] * FY[4][1]) + (img[j + 2][i] * FY[4][2]) + 
					  (img[j + 2][i + 1] * FY[4][3]) + (img[j + 2][i + 2] * FY[4][4]);
				if (sum == 0) sum = 1;
				
				valV = valV / sum;
				
				
				gradient = sqrt( (valH*valH) + (valV*valV) );  	

				bool run = true;
				range = 0;
				while(run){
					if(gradient >= range && gradient < range + 16){
						hist5[range/16]++;
						run = false;
					}
					else if (gradient >= 256){
						hist5[15]++;
						run = false;
					}
					else if (gradient < 0){
						hist5[0]++;
						run = false;
					}
					
					range = range + 16;
				}
				
				if (gradient > gradMax5) gradMax5 = gradient;
				
			}
		}
		
		
		//clear/poor histogram seperation
		if(gradMax3 >= poorFilter3 && gradMax5 >= poorFilter5){
			clearCount++;
			for (int i=0 ; i<16 ; i++){
				histClear3[i] += hist3[i];
				histClear5[i] += hist5[i];
			}
		}
		else{
			poorCount++;
			for (int i=0 ; i<16 ; i++){
				histPoor3[i] += hist3[i];
				histPoor5[i] += hist5[i];
			}
		}
		
		
		
		
		
		std::cout << "--------------------------------------------------------3X3--------------------------------------------------------" << std::endl;
		for (int i=0 ; i<16 ; i++){
			std::cout << 16 * i << "-" << (16 * (i+1)) - 1 << ": " << hist3[i] << std::endl;
			
		}
		
		std::cout << std::endl << "--------------------------------------------------------5X5--------------------------------------------------------" << std::endl;
		for (int i=0 ; i<16 ; i++){
			std::cout << 16 * i << "-" << (16 * (i+1)) - 1 << ": " << hist5[i] << std::endl;
		}	
			
		
		fclose(inp);
		
	}

	//clear histogram
	std::cout << std::endl << "CLEAR WEATHER  " << clearCount << std::endl; //~785
	
	std::cout << "--------------------------------------------------------3X3--------------------------------------------------------" << std::endl;
	for (int i=0 ; i<16 ; i++){
		std::cout << 16 * i << "-" << (16 * (i+1)) - 1 << ": " << histClear3[i]/clearCount << std::endl;
			
	}
		
	std::cout << std::endl << "--------------------------------------------------------5X5--------------------------------------------------------" << std::endl;
	for (int i=0 ; i<16 ; i++){
		std::cout << 16 * i << "-" << (16 * (i+1)) - 1 << ": " << histClear5[i]/poorCount << std::endl;
	}
		
		
	//poor histogram
	std::cout << std::endl << "POOR WEATHER  " << poorCount << std::endl;
	
	std::cout << "--------------------------------------------------------3X3--------------------------------------------------------" << std::endl;
	for (int i=0 ; i<16 ; i++){
		std::cout << 16 * i << "-" << (16 * (i+1)) - 1 << ": " << histPoor3[i]/clearCount << std::endl;
			
	}
		
	std::cout << std::endl << "--------------------------------------------------------5X5--------------------------------------------------------" << std::endl;
	for (int i=0 ; i<16 ; i++){
		std::cout << 16 * i << "-" << (16 * (i+1)) - 1 << ": " << histPoor5[i]/poorCount << std::endl;
	}	

}//end main **/