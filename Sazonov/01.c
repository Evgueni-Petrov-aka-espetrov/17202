#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int endnum = -1, dotnum = -1;

int input_check(FILE *input_file, char *input_array){
  int enumerator = 0;
  while(1){
    char tmp;
    fscanf(input_file, "%c", &tmp);
    if(tmp == '.'){
      if(dotnum != -1)
        return 0;
      dotnum = enumerator;
    }
    if(tmp == '\n'){
      endnum = enumerator;
      break;
    }
    ++enumerator;
  }
  fclose(input_file);
  input_file = fopen("in.txt", "r");
  fscanf(input_file, "%d %d\n", &enumerator, &enumerator);
  if(dotnum == endnum - 1 || dotnum == 0 || endnum == 0)
    return 0;
  return 1;
}

double in_string(char *input_array, int b1, int power){
  char res;
  fscanf(input_file, "%c", &res);
  if(res == '.'){
    fscanf(input_file, "%c", &res);
  }
  if(res == '\n'){
    return 0;
  }
  int result = 0;
  res -= '0';
  if(res > 9){
    result = 10;
    res += '0' - 'A';

    if(res > 9)
      res += 'A' - 'a';
  }
  result += res;
  if(result >= b1 || result < 0)
    return -1;
  return result * pow(b1, power);
}

int main(){
  FILE *input_file, *output_file;
  input_file = fopen("in.txt", "r");
  output_file = fopen("out.txt", "w");
  int b1, b2;

  fscanf(input_file, "%d %d\n", &b1, &b2);
  if(b1 > 16 || b2 > 16 || b1 < 2 || b2 < 2){
    fprintf(output_file, "bad input");
    return 0;
  }
  char input_array
  if(input_check(input_file) == 0){
    fprintf(output_file, "bad input");
    return 0;
  }
  double tmp;
  unsigned long long decimal_c = 0; // хранение целой части ввода, переведенного в десятичный вид
  for(int i = dotnum - 1; i > -1; i--){
    tmp = in_string(input_file, b1, i);
    if(tmp == -1){
      fprintf(output_file, "bad input");
      return 0;
    }
    decimal_c += tmp;
  }
  double decimal_d = 0; // хранение дробной части ввода, переведенного в десятичный вид

  for(int i = -1; i > (dotnum - endnum); i--){
    tmp = in_string(input_file, b1, i);
    printf("inside\n");
    if(tmp == -1){
      fprintf(output_file, "bad input");
      return 0;
    }
    decimal_d += tmp;
  }
  while(decimal_d >= 1){
    --decimal_d;
    ++decimal_c;
  }
  int power = 1;
  while(power * b2 < decimal_c)
    power *= b2;
  char out[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  do{
    fprintf(output_file, "%c", out[decimal_c / power]);
    decimal_c %= power;
    power /= b2;
  }while(decimal_c > 0);
  printf("%f\n", decimal_d);
  if(decimal_d == (double) 0){
    fprintf(output_file, ".0");
  } else {
    fprintf(output_file, ".");
    for(int i = 0; i < 13; i++){
      decimal_d *= b2; tmp = 0;
      while(decimal_d >= 1){
        --decimal_d;
        ++tmp;
      }
      fprintf(output_file, "%c", out[(int) tmp]);
    }
  }
  return 0;
}
