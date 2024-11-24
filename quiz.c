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
void registrasiPengguna(const char *username, const char *password);
int loginPengguna(const char *username, const char *password);

// Prototipe fungsi kuis
void clearScreen();
void printLine();
void tampilkanPeraturan();
void mulaiQuiz();

// Fungsi untuk registrasi pengguna baru
void registrasiPengguna(const char *username, const char *password) {
    FILE *file = fopen("database/login.bin", "ab");
    if (!file) {
        printf("Gagal membuka file. Pastikan direktori 'database' sudah ada.\n");
        return;
    }

    User penggunaBaru;
    strncpy(penggunaBaru.username, username, sizeof(penggunaBaru.username) - 1);
    strncpy(penggunaBaru.password, password, sizeof(penggunaBaru.password) - 1);

    // Menyimpan data pengguna baru ke file
    fwrite(&penggunaBaru, sizeof(User), 1, file);
    fclose(file);
    printf("Registrasi berhasil! Silakan login dengan username dan password Anda.\n");
}

// Fungsi untuk login pengguna menggunakan CLA
int loginPengguna(const char *username, const char *password) {
    FILE *file = fopen("database/login.bin", "rb");
    if (!file) {
        printf("Gagal membuka file. Pastikan file 'database/login.bin' ada.\n");
        return 0;
    }

    User pengguna;
    int loginsuccess = 0;
    // Membaca data dari file dan mencocokkan dengan input
    while (fread(&pengguna, sizeof(User), 1, file)) {
        if (strcmp(pengguna.username, username) == 0 && strcmp(pengguna.password, password) == 0) {
            loginsuccess = 1;
            break; // Keluar dari loop jika login berhasil
        }
    }

    fclose(file);

    if (loginsuccess) {
        printf("Login berhasil! Selamat datang, %s.\n", username);
    } else {
        printf("Login gagal! Username atau password salah.\n");
    }

    return loginsuccess;
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

// Evaluasi jawaban
        if (jawab == jawabanBenar[i]) {
            printf("\nJawaban benar! Anda memenangkan $%d.\n", hadiah[i]);
            hadiahAman = hadiah[i]; // Hadiah terakhir yang aman
        } else {
            printf("\nJawaban salah! Permainan berakhir.\n");
            break;
        }

        printf("Tekan ENTER untuk lanjut ke pertanyaan berikutnya...\n");
        getchar();
        getchar();
    }

    clearScreen();
    printLine();
    if (i == 5) {
        printf("Selamat! Anda memenangkan hadiah utama: $%d!\n", hadiah[4]);
    } else {
        printf("Anda membawa pulang hadiah aman sebesar: $%d\n", hadiahAman);
    }
    printLine();
    printf("Terima kasih telah bermain!\n");
}

// Fungsi utama
int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Gunakan program ini dengan:\n");
        printf("  ./program register <username> <password> -> untuk registrasi\n");
        printf("  ./program login <username> <password>    -> untuk login\n");
        return 0;
    }

    if (argc == 4) {
        if (strcmp(argv[1], "register") == 0) {
            // Registrasi pengguna baru
            registrasiPengguna(argv[2], argv[3]);
        } else if (strcmp(argv[1], "login") == 0) {
            // Login pengguna
            if (loginPengguna(argv[2], argv[3])) {
                printf("\nLogin berhasil! Mulai bermain kuis...\n");
                tampilkanPeraturan();
                mulaiQuiz();
            } else {
                printf("Login gagal, coba lagi!\n");
            }
        } else {
            printf("Perintah tidak dikenali. Gunakan 'register' atau 'login'.\n");
        }
    } else {
        printf("Format perintah salah.\n");
    }

    return 0;
}
 
