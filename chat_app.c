#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 1024

void caesarCipher(char *message, int shift, int encrypt);
void xorCipher(char *message, char *key);
void symmetricEncryption(char *message, char *key, int encrypt);
void displayMenu();
void printHex(const char *message);

int main() {
    // Vars
    char message[MAX_MESSAGE_LENGTH];
    char key[MAX_MESSAGE_LENGTH];
    int choice;
    int shift;
    int keylen;

    printf("Welcome to the Group1 Encrypted Chat Application!\n");
    printf("Press `Ctrl+C` to exit the application at any time.\n\n");

    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        if (choice == 4) {
            printf("Exiting the application.\n");
            break;
        }

        printf("Enter your message: ");
        fgets(message, MAX_MESSAGE_LENGTH, stdin); // reading string
        message[strcspn(message, "\n")] = 0;

        // Check if user has skill issue and want's to dip
        if (strcmp(message, "\\quit") == 0) {
            printf("Exiting the application. Goodbye!\n");
            break;
        }

        switch (choice) {
            case 1: // Caesar Cipher -> just shift with shift value
                printf("Enter shift value for Caesar Cipher: ");
                scanf("%d", &shift);
                getchar(); 

                caesarCipher(message, shift, 1);
                printf("\n\nEncrypted Message: %s\n", message);

                caesarCipher(message, shift, 0);
                printf("Decrypted Message: %s\n", message);
                break;

            case 2: // XOR Cipher -> perform xor with passkey
                printf("Enter key for XOR Cipher: ");
                fgets(key, MAX_MESSAGE_LENGTH, stdin);
                key[strcspn(key, "\n")] = 0; // Remove newline character

                if (strlen(key) == 0) { //error handling for if key not given
                    printf("Error: Key cannot be empty.\n");
                    break;
                }

                xorCipher(message, key);
                printf("Encrypted Message (String): %s\n", message);
                printf("Encrypted Message (Hex): ");
                printHex(message);

                xorCipher(message, key);
                printf("Decrypted Message: %s\n", message);
                break;

            case 3: // symmetric -> nothing but xor with rand gen key as passphrase
                srand(time(NULL));
                printf("Enter key length for Symmetric Encryption: ");
                scanf("%d", &keylen);
                getchar();

                for (int i = 0; i < keylen; i++) {
                    key[i] = 'A' + (rand() % 26); // generate a rand char and create a 16 char key
                }
                key[keylen] = '\0';
                printf("Generated Key for Symmetric Encryption: %s\n", key);

                symmetricEncryption(message, key, 1);
                printf("Encrypted Message: %s\n", message);

                symmetricEncryption(message, key, 0);
                printf("Decrypted Message: %s\n", message);
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }

        printf("\n");
    }

    return 0;
}

void displayMenu() {
    printf("\nSelect an encryption method:\n");
    printf("1. Caesar Cipher\n");
    printf("2. XOR Cipher\n");
    printf("3. Symmetric Encryption\n");
    printf("4. Exit\n");
}

void caesarCipher(char *message, int shift, int encrypt) {
    for (int i = 0; message[i] != '\0'; i++) {
        char offset = (message[i] >= 'a' && message[i] <= 'z') ? 'a' :
                      (message[i] >= 'A' && message[i] <= 'Z') ? 'A' : 0;
        if (offset) {
            if (encrypt) {
                message[i] = (message[i] - offset + shift) % 26 + offset;
            } else {
                message[i] = (message[i] - offset - shift + 26) % 26 + offset;
            }
        }
    }
}

void xorCipher(char *message, char *key) {
    int keyLength = strlen(key);
    if (keyLength == 0) {
        printf("Error: Key cannot be empty.\n");
        return;
    }

    for (int i = 0; message[i] != '\0'; i++) {
        message[i] ^= key[i % keyLength];
    }
}

void symmetricEncryption(char *message, char *key, int encrypt) {
    xorCipher(message, key); // Symmetric encryption using XOR and rand gen key
}

void printHex(const char *message) { // this needed cause sometimes xor creates binary values that fall outside of ascii range, so it aint gonna print in the terminal
    for (int i = 0; message[i] != '\0'; i++) {
        printf("%02x ", (unsigned char)message[i]);
    }
    printf("\n");
}
