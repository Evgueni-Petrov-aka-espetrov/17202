#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


char* read(FILE* iFile, char* model) {
	int i;
	fgets(model, 18, iFile);
	model[strlen(model) - 1] = '\0';
	//int len_model = strlen(model);
	fseek(iFile, 0, SEEK_END);
	int len_string = ftell(iFile);
	len_string -= strlen(model);
	fseek(iFile, strlen(model) + 2, SEEK_SET);
	char* string = (char*)malloc(sizeof(char) * len_string);
	for (i = 0; i < len_string - 1; i++) {
		string[i] = (char)fgetc(iFile);
	}
	string[i] = '\0';
	return string;
}


int* setpows(int size) {

	int* pows = (int*)malloc(sizeof(int)*size);
	for (int i = 0; i < size; i++){
		pows[i] =(int)(pow(3, i));
	}
	return pows;
}


int sethash(char* model, int* pows, int size) {
	int sum = 0;
	for (int i = 0; i < size; i++) {
		if (model[i] > 0)
			sum += (model[i] % 3) * pows[i];
		else 
			sum += ((model[i] + 127) % 3) * pows[i];
	}
	return sum;

}

int change_hash(char ch_del, char ch_new, int hash,int size_of_model, int* pows) {
	if (ch_del > 0)
		hash -= ch_del % 3;
	else
		hash -= (ch_del + 127) % 3;

	hash /= 3; 

	if (ch_new > 0)
		hash += (ch_new % 3) * pows[size_of_model - 1]; 
	else 
		hash += ((ch_new + 127) % 3) * pows[size_of_model - 1];
	return hash;
}


void compare_str(char* string, char* model, int position, FILE* oFile) {
	for (unsigned int i = 0; i < strlen(model); i++) {
		fprintf(oFile, "%d ", i + position + 1);

		if (string[i + position] != model[i]) {
			break;
		}
			
	}
}

void rabin_karp(char* string, char* model, FILE* oFile, int* pows) {
	int modelhash = sethash(model, pows, strlen(model));
	fprintf(oFile, "%d ", modelhash);
	int hash = sethash(string, pows, strlen(model));
	for (unsigned int i = 0; i < strlen(string) - strlen(model); i++) {
		
		if (modelhash == hash) {
			compare_str(string, model, i, oFile);
		}
		hash = change_hash(string[i], string[i + strlen(model)], hash, strlen(model), pows);
		
	}

}

int main(){
	FILE *iFile = fopen("in.txt", "r");
	FILE *oFile = fopen("out.txt", "w");
	char model[18] = { 0 };

	char* string = read(iFile, model);
	
	int* pows = setpows(strlen(model));

	rabin_karp(string, model, oFile, pows);

	free(pows);
	free(string);
	fclose(iFile);
	fclose(oFile);


    return 0;
}

