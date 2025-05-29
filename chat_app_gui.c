#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MESSAGE_LENGTH 1024

void caesarCipher(char *message, int shift, int encrypt);
void xorCipher(char *message, char *key);
void symmetricEncryption(char *message, char *key, int encrypt);

void on_encrypt_clicked(GtkWidget *widget, gpointer data);
void on_decrypt_clicked(GtkWidget *widget, gpointer data);

GtkWidget *message_entry;
GtkWidget *key_entry;
GtkWidget *result_label;
GtkWidget *method_combo;

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Encrypted Chat Application");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *method_label = gtk_label_new("Select Encryption Method:");
    method_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(method_combo), "Caesar Cipher");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(method_combo), "XOR Cipher");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(method_combo), "Symmetric Encryption");
    gtk_combo_box_set_active(GTK_COMBO_BOX(method_combo), 0);

    GtkWidget *message_label = gtk_label_new("Enter Message:");
    message_entry = gtk_entry_new();

    GtkWidget *key_label = gtk_label_new("Enter Key/Shift:");
    key_entry = gtk_entry_new();

    GtkWidget *encrypt_button = gtk_button_new_with_label("Encrypt");
    GtkWidget *decrypt_button = gtk_button_new_with_label("Decrypt");

    result_label = gtk_label_new("Result will appear here.");

    // Attach  to grid
    gtk_grid_attach(GTK_GRID(grid), method_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), method_combo, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), message_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), message_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), key_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), key_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), encrypt_button, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), decrypt_button, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), result_label, 0, 4, 2, 1);

    g_signal_connect(encrypt_button, "clicked", G_CALLBACK(on_encrypt_clicked), NULL);
    g_signal_connect(decrypt_button, "clicked", G_CALLBACK(on_decrypt_clicked), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

void on_encrypt_clicked(GtkWidget *widget, gpointer data) {
    const char *message = gtk_entry_get_text(GTK_ENTRY(message_entry));
    const char *key = gtk_entry_get_text(GTK_ENTRY(key_entry));
    int method = gtk_combo_box_get_active(GTK_COMBO_BOX(method_combo));

    char result[MAX_MESSAGE_LENGTH];
    strcpy(result, message);

    if (method == 0) { // Caesar Cipher
        int shift = atoi(key);
        caesarCipher(result, shift, 1);
    } else if (method == 1) { // XOR Cipher
        xorCipher(result, (char *)key);
    } else if (method == 2) { // Symmetric Encryption
        symmetricEncryption(result, (char *)key, 1);
    }

    gtk_label_set_text(GTK_LABEL(result_label), result);
}

void on_decrypt_clicked(GtkWidget *widget, gpointer data) {
    const char *encrypted_message = gtk_label_get_text(GTK_LABEL(result_label)); 
    const char *key = gtk_entry_get_text(GTK_ENTRY(key_entry));
    int method = gtk_combo_box_get_active(GTK_COMBO_BOX(method_combo));

    char result[MAX_MESSAGE_LENGTH];
    strcpy(result, encrypted_message); 

    if (method == 0) { // Caesar Cipher
        int shift = atoi(key);
        caesarCipher(result, shift, 0);
    } else if (method == 1) { // XOR Cipher
        xorCipher(result, (char *)key);
    } else if (method == 2) { // Symmetric Encryption
        symmetricEncryption(result, (char *)key, 0);
    }

    gtk_label_set_text(GTK_LABEL(result_label), result);
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
    for (int i = 0; message[i] != '\0'; i++) {
        message[i] ^= key[i % keyLength];
    }
}

void symmetricEncryption(char *message, char *key, int encrypt) {
    if (encrypt) {
        int keyLength = strlen(key); 
        srand(time(NULL));
        for (int i = 0; i < keyLength; i++) {
            key[i] = 'A' + (rand() % 26); 
        }
        key[keyLength] = '\0';
        gtk_label_set_text(GTK_LABEL(result_label), key); 
    }
    xorCipher(message, key);
}
