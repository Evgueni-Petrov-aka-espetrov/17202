    #include <stdio.h>
    #include <math.h>
    #include <string.h>



    char char_to_num(char c) {
        if (c >= 'a' && c <= 'f')
            c -= 'a' - 'A';

        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;
        return c - '0';
    }

    char num_to_char(int n) {
        if (n >= 0 && n <= 9)
            return '0' + n;
        return n + 'A' - 10;
    }





    long double sys_to_dec(int b1, char *X) {

        long double sum = 0;
        long long int integer_part = 0;
        int i = 0;
        char c = 0;


        for (i = 0; (c = *X) != '.'; ++i) {
            integer_part = (integer_part * b1) + char_to_num(c);
            ++X;
        }


        ++X;
        for (i = 0; integer_part > 0; ++i) { //summa razradov do zapyatoy
            sum += (integer_part % b1) * pow((float)b1, i);
            integer_part /= b1;
        }


        for (i = -1; (c = *X) != '\0'; --i) { //summa after zapyatoy
            sum += char_to_num(c) * pow((float)b1, i);
            ++X;
        }
        return sum;
    }

    long double sys_to_integer_dec(int b1, char *X) {
        long double sum = 0;
        long long int integer_part = 0;
        int i = 0;
        char c = 0;

        for (i = 0; (c = *X) != '\0'; ++i) {
            integer_part = (integer_part * b1) + char_to_num(c);
            ++X;
        }


        return integer_part; //
    }


    void dec_to_sys(long double X, int b2) {
        int i = 0, j = 0;
        char c = 0;
        char integer_result[63], fractional_result_str[63];
        long long int integer_part = X;
        long double fractional_part = X - integer_part;


        do { //integer part
            integer_result[i++] = num_to_char(integer_part % b2);
            integer_part /= b2;
        } while (integer_part);
        integer_result[i] = '\0';

        int len = i; //razvorachivaem integer part
        for (i = 0; i < len / 2; ++i) {
            int temp = integer_result[i];
            integer_result[i] = integer_result[len - i - 1];
            integer_result[len - i - 1] = temp;
        }


        i = 0; //fractional part
        while (fractional_part > 0 && i < 12) {
            fractional_part *= b2;
            integer_part = (long long)fractional_part;
            fractional_part -= integer_part;
            fractional_result_str[i++] = num_to_char(integer_part);
        }

        fractional_result_str[i] = '\0';

        if (i == 0)
            printf("%s\n", integer_result);
        else
            printf("%s.%s\n", integer_result, fractional_result_str);
    }

    int main() {

        int b1, b2;
        char X[14];
        long double dec = 0;
        int fractional_part = 0;

        scanf("%d %d\n", &b1, &b2);
        gets(X);

        if ((b1 < 2 || b1 > 16) || (b2 < 2 || b2 > 16)) {
            printf("bad input");
        }

        char *p = X;

        if (*p == '.') {
            printf("bad input");
            return 0;
        }
        while (*p) {

            if ((char_to_num(*p) > b1 - 1) && (*p != '.')) {
                printf("bad input");
                return 0;
            }

            if (*p == '.') {
                fractional_part += 1;
            }

            ++p;

        }

        --p;

        if (*p == '.') {
            printf("bad input");
            return 0;
        }
        if (fractional_part > 1) {
            printf("bad input");
            return 0;
        }


        if (fractional_part>0)
            dec = sys_to_dec(b1, X);
        else
            dec = sys_to_integer_dec(b1, X);

        dec_to_sys(dec, b2);
        return 0;
    }
