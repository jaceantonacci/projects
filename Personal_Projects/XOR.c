#include <stdio.h>
#include <string.h>

void encrypt(char* plainFile, char* cipherFile, int key) {
	FILE* plainFile1 = fopen(plainFile, "r");
	FILE* cipherFile1 = fopen(cipherFile, "w");

	int c;
	while ((c = fgetc(plainFile1)) != EOF) {
		fputc(c ^ key, cipherFile1);
	}

	fclose(plainFile1);
	fclose(cipherFile1);
}

void decrypt(char* plainFile, char* cipherFile, int key) {
	FILE* plainFile2 = fopen(plainFile, "r");
	FILE* cipherFile2 = fopen(cipherFile, "w");
	
	int c;
	while ((c = fgetc(cipherFile2)) != EOF) {
		fputc(c ^ key, plainFile2);
	}

	fclose(cipherFile2);
	fclose(plainFile2);
}

int main() {

	int key;
	char choice[256], keyFile[256], plainFile[256], cipherFile[256];

	printf("Choose to Encrypt or to Decrypt(e/d) \n");
	scanf(" %c", &choice);

	printf("Enter file of key \n");
	scanf("%s", keyFile);

	printf("Enter file to store plain text \n");
	scanf("%s", plainFile);

	printf("Enter file to store cipher text \n");
	scanf("%s", cipherFile);

	FILE* keyFileOpen = fopen(keyFile, "r");

	fscanf(keyFileOpen, "%d", &key);
	fclose(keyFileOpen);

	if (choice[0] == 'e') {
		encrypt(plainFile, cipherFile, key);
		printf("Encryption Successful", cipherFile);
	}
	else if (choice == 'd') {
		decrypt(plainFile, cipherFile, key);
		printf("Decryption Successful", cipherFile);
	}

	return 0;
}
