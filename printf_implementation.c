#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 100
#define MAX_RESULT 10000

static int write_stdout(const char *token, int length) {
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}

const char* convert_to_string(unsigned int n, char* str, char type) {

	int nr[MAX_LEN], i = 0, base = 10;

	if (type == 'x') {
		base = 16;
	}

	// construirea vectorului nr
	while (n != 0) {
		nr[i++] = n % base;
		n /= base;
	}
	
	// construirea string-ului str
	int len = i--;
	for (int j = 0; j < len; j++) {	

		// numarul este in hexazecimal	
		if (nr[i] >= 10 && nr[i] <= 15) {
			str[j] = 'a' + nr[i--] - 10;

		} else {
			str[j] = nr[i--] + '0';
		}
	}

	return str;
}

int iocla_printf(const char *format, ...) {

	va_list ap;
	va_start(ap, format);

	// initializarea vectorului rezultat
	char result[MAX_RESULT] = {0};
	int idx = 0;

	while (idx < strlen(format)) {
		char str[MAX_LEN] = {0};

		// verificare daca urmeaza un tip de date
		if (format[idx] == '%'){
			idx++;

			// verificare daca tipul de date este string
			if (format[idx] == 's') {

				const char *s = va_arg(ap, const char*);
				strcat(result, s);
				idx++;

			// verificare daca tipul de date este int
			} else if (format[idx] == 'd') {

				int d = va_arg(ap, int);
				unsigned int n = d;

				if (d < 0) {
					n = -d;
					strcat(result, "-");
				} 
				
				const char* new_str = convert_to_string(n, str, 'd');
				strcat(result, new_str);
				idx++;

			// verificare daca tipul de date este unsigned sau hex
			} else if (format[idx] == 'u' || format[idx] == 'x') {

				unsigned int u = va_arg (ap, unsigned int);

				const char* new_str = convert_to_string(u, str, format[idx]);
				strcat(result, new_str);
				idx++;

			// verificare daca tipul de date este char
			} else if (format[idx] == 'c')	{

				char c = va_arg(ap, int);
				result[strlen(result)] = c;
				idx++;

			// caz default - printare caractere
			} else {
				result[strlen(result)] = format[idx++];
			}

		// caz default - printare caractere
		} else {
			
			result[strlen(result)] = format[idx++];
		}
	}

	//intoarcere rezultat final + lungimea acestuia
	return write_stdout(result, strlen(result));
}
