void append_number(char* dest, unsigned long n);
void i_to_a(unsigned long n, char *s, int b);
void left_pad(unsigned long n, char* s, int size);
int a_to_i(char s[], unsigned char size);
char charToHex(const unsigned char c);
void charToHexString(char* input, char* output, int size);
char charToNumber(char c);
char isNumber(char val);
char hexToi(char c);
unsigned long convertToSecond(char tm_hour, char tm_min, char tm_sec);
unsigned long convertToDate(int tm_day, int tm_mon, int tm_year);
void append_hexa(char* dest, unsigned long n);