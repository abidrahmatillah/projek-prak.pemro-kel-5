#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Struktur untuk pengguna
typedef struct {
    char username[50];
    char password[50];
} User;

// Prototipe fungsi login/registrasi
void registrasiPengguna();
int loginPengguna();
void menuLogin();

// Prototipe fungsi kuis
void clearScreen();
void printLine();
void tampilkanPeraturan();
void mulaiQuiz();

// Fungsi utama
int main() {
    int pilihan;

    do {
        menuLogin();
        printf("Masukkan pilihan Anda: ");
        scanf("%d", &pilihan);
        getchar(); // Menghapus karakter newline dari buffer

        switch (pilihan) {
            case 1:
                registrasiPengguna();
                break;
            case 2:
                if (loginPengguna()) {
                    printf("\nLogin berhasil! Mulai bermain kuis...\n");
                    getchar(); // Menunggu input sebelum mulai kuis
                    tampilkanPeraturan();
                    mulaiQuiz();
                } else {
                    printf("Login gagal, coba lagi!\n");
                }
                break;
            case 3:
                printf("Program selesai, terima kasih!\n");
                break;
            default:
                printf("Pilihan salah, coba lagi!\n");
        }
    } while (pilihan != 3);

    return 0;
}

// Fungsi untuk menampilkan menu login/registrasi
void menuLogin() {
    printf("\n=== Sistem Login dan Registrasi ===\n");
    printf("1. Registrasi\n");
    printf("2. Login\n");
    printf("3. Keluar\n");
}

// Fungsi untuk registrasi pengguna baru
void registrasiPengguna() {
    FILE *file = fopen("login.bin", "ab");
    if (!file) {
        perror("Gagal membuka file");
        return;
    }

    User penggunaBaru;
    printf("Masukkan username: ");
    fgets(penggunaBaru.username, sizeof(penggunaBaru.username), stdin);
    strtok(penggunaBaru.username, "\n"); // Menghapus karakter newline
    printf("Masukkan password: ");
    fgets(penggunaBaru.password, sizeof(penggunaBaru.password), stdin);
    strtok(penggunaBaru.password, "\n"); // Menghapus karakter newline

    fwrite(&penggunaBaru, sizeof(User), 1, file);
    fclose(file);

    printf("Registrasi berhasil!\n");
}

// Fungsi untuk login pengguna
int loginPengguna() {
    FILE *file = fopen("login.bin", "rb");
    if (!file) {
        perror("Gagal membuka file");
        return 0;
    }

    char username[50], password[50];
    printf("Masukkan username: ");
    fgets(username, sizeof(username), stdin);
    strtok(username, "\n"); // Menghapus karakter newline
    printf("Masukkan password: ");
    fgets(password, sizeof(password), stdin);
    strtok(password, "\n"); // Menghapus karakter newline

    User pengguna;
    while (fread(&pengguna, sizeof(User), 1, file)) {
        if (strcmp(pengguna.username, username) == 0 &&
            strcmp(pengguna.password, password) == 0) {
            fclose(file);
            return 1; // Login berhasil
        }
    }

    fclose(file);
    return 0; // Login gagal
}

// Fungsi untuk membersihkan layar
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Fungsi untuk menampilkan garis pemisah
void printLine() {
    printf("==========================================\n");
}

// Fungsi untuk menampilkan peraturan kuis
void tampilkanPeraturan() {
    clearScreen();
    printLine();
    printf("           SELAMAT DATANG DI HOLLY WINGS GAME !      \n");
    printLine();
    printf("Peraturan:\n");
    printf("1. Jawab setiap pertanyaan dengan memilih A, B, C, atau D.\n");
    printf("2. Setiap jawaban benar mendapatkan hadiah.\n");
    printf("3. Jika salah menjawab, permainan berakhir, dan hadiah Anda akan turun.\n");
    printf("4. Anda memiliki satu bantuan *50:50* untuk menghilangkan dua opsi yang salah.\n");
    printLine();
    printf("Tekan ENTER untuk memulai...\n");
    getchar();
}

// Fungsi untuk menjalankan kuis
void mulaiQuiz() {
    clearScreen();

    // Data pertanyaan, pilihan, jawaban benar, dan hadiah
    const char *pertanyaan[5] = {
        "1. Sawah:padi =...:...\n   A. Gajah:gunung\n   B. Hutan:Pohon\n   C. Pabrik:Mobil\n   D. Daun:Teh",
        "2. Apa nama jenis kumbang terbesar di dunia? ?\n   A. Gloowall\n   B. GOAT\n   C. Goliath\n   D. Jupiter",
        "3. Siapa penemu bola lampu?\n   A. Nikola Tesla\n   B. Thomas Edison\n   C. Albert Einstein\n   D. Alexander Graham Bell",
        "4. 23, 26, 19, 22, 15, 18, ..., ....\n   A. 12 dan 13\n   B. 11 dan 14\n   C. 11 dan 18\n   D. 21 dan 24",
        "5. Pilihlah kelanjutan pola berikut: A, C, E, G, ...?\n   A. J\n   B. I\n   C. F\n   D. K"
    };

    const char jawabanBenar[5] = {'B', 'C', 'B', 'B', 'B'};
    const int hadiah[5] = {1000, 5000, 10000, 25000, 100000};
    int hadiahAman = 0;
    int bantuanTersedia = 1;

    char jawab;
    int i;

    for (i = 0; i < 5; i++) {
        clearScreen();
        printLine();
        printf("PERTANYAAN %d: (Hadiah: $%d)\n", i + 1, hadiah[i]);
        printf("%s\n", pertanyaan[i]);
        printLine();

        if (bantuanTersedia) {
            printf("Ketik 'H' untuk menggunakan bantuan 50:50.\n");
        }
        printf("Jawaban Anda: ");
        scanf(" %c", &jawab); 
        jawab = toupper(jawab);
