#include <stdio.h>

void encrypt(char *plainFile, char *cipherFile, int key) {
	FILE *plainFile1 = fopen(plainFile, "r");
	FILE *cipherFile1 = fopen(cipherFile, "w");

	char c;
	while ((c = fgetc(plainFile1)) != EOF) {
		if (isupper(c)) {
			c = (c - 'A' + key) % 26 + 'A';
		}
		else if (islower(c)) {
			c = (c - 'A' + key) % 26 + 'A';
		}
		fputc(c, cipherFile1);
	}

	fclose(plainFile1);
	fclose(cipherFile1);
}

void decrytion(char* plainFile, char* cipherFile, int key) {
	FILE* plainFile2 = fopen(plainFile, "r");
	FILE* cipherFile2 = fopen(cipherFile, "w");

	char c;
	while ((c = fgetc(cipherFile2)) != EOF) {
		if (isupper(c)) {
			c = (c - 'A' + key + 26) % 26 + 'A';
		}
		else if (islower(c)) {
			c = (c - 'A' + key + 26) % 26 + 'A';
		}
		fputc(c, cipherFile2);
	}

	fclose(cipherFile2);
	fclose(plainFile2);
}

int main() {
	
	int key;
	char choice[50], keyFile[256], plainFile[256], cipherFile[256];

	printf("Choose to Encrypt or to Decrypt(e/d) \n");
	scanf(" %c", &choice);

	printf("Enter file of key \n");
	scanf("%s", &keyFile);

	printf("Enter file to store plain text \n");
	scanf("%s", &plainFile);

	printf("Enter file to store cipher text \n");
	scanf("%s", &cipherFile);

	FILE *keyFileOpen = fopen(keyFile, "r");

	fscanf(keyFileOpen, "%d", &key);
	fclose(keyFileOpen);

	if (choice[0] == 'e') {
		encrypt(plainFile, cipherFile, key);
		printf("Encryption Successful", cipherFile);
	}
	else if (choice == 'd') {
		decrytion(plainFile, cipherFile, key);
		printf("Decryption Successful", cipherFile);
	}

	return 0;
}
