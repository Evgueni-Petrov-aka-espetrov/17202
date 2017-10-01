#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int endnum = -1, dotnum = -1;

int checkB1B2(int b1, int b2){
  if(b1 < 2 || b2 < 2 || b1 > 16 || b2 > 16) return 1;
  return 0;
}

int inputNcheck(FILE *input_file, char *input_array){ // 1 for badinput
  int enumerator = -1;
  do {
    fscanf(input_file, "%c", &input_array[++enumerator]);
    if(input_array[enumerator] == '.'){
      if(dotnum != -1){
        return 1;
      }
      dotnum = enumerator;
    }
  } while(input_array[enumerator] != '\n');
  endnum = enumerator;
  if(dotnum == endnum - 1 || dotnum == 0 || endnum == 0) return 1;
  return 0;
}

int charToDecValue(char *input_array, int b1){ // 1 for badinput
  for (int i = 0; i < endnum - 1; i++) {
    if(i == dotnum) continue;
    input_array[i] -= '0';
    if(input_array[i] > 9){
      input_array[i] += '0' - 'A';
      if(input_array[i] > 9) input_array[i] += 'A' - 'a';
      input_array[i] += 10;
    }
    if(input_array[i] >= b1 || input_array[i] < 0) return 1;
  }
  return 0;
}

double B1toDec(char input_char, int b1, int power){
    return (input_char * pow(b1, power));
}

int maxB2pow(double decimal, int b2){
  double power = b2;
  while(decimal > power) power *= b2;
  return power;
}

int DectoB2(double decimal, int b2, double power){
  double delta = (int) decimal/power;
}

char B2toChar(int b2_value){
  char out[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'F'};
  return out[b2_value];
}

int main(){
  FILE *input_file, *output_file;
  input_file = fopen("in.txt", "r");
  output_file = fopen("out.txt", "w");

  int b1, b2;
  fscanf(input_file, "%d %d\n", &b1, &b2);
  char input_array[13];

  return 0;
}
