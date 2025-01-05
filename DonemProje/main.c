#include <stdio.h>
#include <stdlib.h>

#define MAX_MUSTERILER 5  

typedef struct {
    int id;   
    int pin;   
    int uyelik;       
    int arac_parked;  
    int arac_aldi;    
} Musteri;


int dolu_park_yerleri = 0;    
int toplam_park_yeri = 5;      
int saatlik_ucret = 20;        

int uyelik_dogrula(int id, int pin, Musteri *musteriler, int mevcut_musteri_sayisi);
void kayit_ol(int *id, int *pin, Musteri **musteriler, int *mevcut_musteri_sayisi);
void arac_park(int id, int saat, int *toplam_ucret, Musteri *musteriler);
void arac_al(int id, int *toplam_ucret, Musteri *musteriler);
void raporla(int park_eden, int arac_alan, int toplam_ucret);
void kayitli_kullanicilari_goster(Musteri *musteriler, int mevcut_musteri_sayisi);


// Bu fonksiyon, program�n ana giri� noktas�d�r.
// Kullan�c�yla etkile�ime girer, se�imlerini al�r ve uygun fonksiyonlar� �a��rarak i�lemleri y�netir.
// - Otopark sistemi i�in gerekli olan ba�lang�� de�erlerini kullan�c�dan al�r.
// - Kullan�c�dan kay�t durumu (kay�tl�/kay�ts�z) bilgisi ister ve buna g�re i�lem yapar.
//   - Kay�tl�ysa do�rulama yapar ve ara� park etme ya da alma i�lemini ger�ekle�tirir.
//   - Kay�tl� de�ilse, kullan�c�y� sisteme kaydeder.
// - Program sonunda, g�n sonu raporunu ekrana yazd�r�r.
// - Dinamik olarak tahsis edilen belle�i temizleyerek, program�n d�zg�n bir �ekilde sonlanmas�n� sa�lar.
// Program�n temel i� ak���n� kontrol eder ve t�m fonksiyonlar�n koordinasyonunu sa�lar.

int main() {
    int secim, musteri_id, musteri_pin;
    int park_saat;
    int toplam_ucret = 0;         
    int park_eden = 0;            
    int arac_alan = 0;            
    int kayitli_mi;
    int cikis = 0;  
    int mevcut_musteri_sayisi = 0;  
    Musteri *musteriler = malloc(MAX_MUSTERILER * sizeof(Musteri));  

    if (musteriler == NULL) {
        printf("Bellek ayagi yapilamadi!\n");
        return 1;
    }

    printf("Oto Park Giris Sistemi\n");

    printf("Toplam park yeri sayisini girin: ");
    scanf("%d", &toplam_park_yeri);
    printf("Simdi dolu park yeri sayisini girin: ");
    scanf("%d", &dolu_park_yerleri);

    while (!cikis) {
        printf("Kayitli misiniz? (1: Evet, 0: Hayir, 9: Cikis): ");
        scanf("%d", &kayitli_mi);

        if (kayitli_mi == 9) {
            cikis = 1;  
            break;
        } else if (kayitli_mi == 1) {
            kayitli_kullanicilari_goster(musteriler, mevcut_musteri_sayisi);

            printf("Musteri ID'nizi girin: ");
            scanf("%d", &musteri_id);
            printf("PIN'inizi girin: ");
            scanf("%d", &musteri_pin);

            if (uyelik_dogrula(musteri_id, musteri_pin, musteriler, mevcut_musteri_sayisi)) {
                printf("\n1. Yeni Arac Park Etme\n");
                printf("2. Mevcut Araci Alma\n");
                printf("Seciminizi yapin (1/2): ");
                scanf("%d", &secim);

                switch (secim) {
                    case 1:
                        if (musteriler[musteri_id - 1001].arac_parked == 0) {
                            if (dolu_park_yerleri < toplam_park_yeri) {
                                printf("Arac park edilmek uzere alindi.\n");
                                printf("Arac park etme saatinizi girin: ");
                                scanf("%d", &park_saat);
                                arac_park(musteri_id, park_saat, &toplam_ucret, musteriler);
                                park_eden++;
                                dolu_park_yerleri++;
                            } else {
                                printf("Park alani dolu. Lutfen bekleyin.\n");
                            }
                        } else {
                            printf("Bu musteri zaten aracini park etti.\n");
                        }
                        break;

                    case 2:
                        if (musteriler[musteri_id - 1001].arac_parked == 1) {
                            arac_al(musteri_id, &toplam_ucret, musteriler);
                            arac_alan++;
                            printf("Araciniz alindi.\n");
                        } else {
                            printf("Aracinizi park etmediniz. Once park etmeniz gerekmektedir.\n");
                        }
                        break;

                    default:
                        printf("Gecersiz secim! Tekrar deneyin.\n");
                }
            } else {
                printf("Gecersiz ID veya PIN! Tekrar deneyin.\n");
            }
        } else if (kayitli_mi == 0) {
            kayit_ol(&musteri_id, &musteri_pin, &musteriler, &mevcut_musteri_sayisi);
            printf("Kayit basarili!\n");
            printf("Yeni kullanici ID: %d, PIN: %d\n", musteri_id, musteri_pin);
        } else {
            printf("Gecersiz secim! L�tfen 1, 0 veya 9 girin.\n");
        }
    }

    raporla(park_eden, arac_alan, toplam_ucret);
    printf("Cikis yapildi.\n");

    free(musteriler);

    return 0;
}
// Bu fonksiyon, m��terinin sisteme kay�tl� olup olmad���n� kontrol eder.
// Kullan�c�dan al�nan ID ve PIN bilgilerini mevcut m��teri listesiyle kar��la�t�r�r.
// E�er bilgiler do�ruysa 1 d�nd�r�r, aksi takdirde 0 d�nd�r�r.
// Bu i�lem, m��teri do�rulama i�lemleri i�in kullan�l�r.

int uyelik_dogrula(int id, int pin, Musteri *musteriler, int mevcut_musteri_sayisi) {
    int i;  
    for (i = 0; i < mevcut_musteri_sayisi; i++) {
        if (musteriler[i].id == id && musteriler[i].pin == pin) {
            return 1; 
        }
    }
    return 0;  
}
// Bu fonksiyon, sisteme yeni m��teri eklemek i�in kullan�l�r.
// Kullan�c�dan yeni bir ID ve PIN al�r ve m��teri listesine ekler.
// E�er mevcut m��teri say�s� art�yorsa, bellek yeniden tahsis edilir (reallocate).
// Yeni m��teri bilgileri sisteme �ye olarak kaydedilir.

void kayit_ol(int *id, int *pin, Musteri **musteriler, int *mevcut_musteri_sayisi) {
    printf("Lutfen yeni bir ID ve PIN belirleyin.\n");
    printf("Yeni ID: ");
    scanf("%d", id);
    printf("Yeni PIN: ");
    scanf("%d", pin);

    *musteriler = realloc(*musteriler, (*mevcut_musteri_sayisi + 1) * sizeof(Musteri));
    if (*musteriler == NULL) {
        printf("Bellek ayrilamadi!\n");
        exit(1);
    }

    (*musteriler)[*mevcut_musteri_sayisi].id = *id;
    (*musteriler)[*mevcut_musteri_sayisi].pin = *pin;
    (*musteriler)[*mevcut_musteri_sayisi].uyelik = 1;  
    (*musteriler)[*mevcut_musteri_sayisi].arac_parked = 0;
    (*musteriler)[*mevcut_musteri_sayisi].arac_aldi = 0;
    (*mevcut_musteri_sayisi)++;
}
// Bu fonksiyon, m��terinin arac�n� park etmesini sa�lar.
// M��terinin park etmek istedi�i s�re al�n�r ve bu s�reye g�re �cret hesaplan�r.
// Hesaplanan �cret, toplam park �cretine eklenir ve m��teri durumu g�ncellenir.
// M��teri, park alan�nda yer buldu�unda bu fonksiyon �al��t�r�l�r.

void arac_park(int id, int saat, int *toplam_ucret, Musteri *musteriler) {
    int ucret = saat * 20;  
    *toplam_ucret += ucret;
    printf("Park ucreti: %d TL\n", ucret);
    musteriler[id - 1001].arac_parked = 1; 
}
// Bu fonksiyon, park etmi� bir m��terinin arac�n� almas�n� sa�lar.
// Ara� al�nd���nda m��terinin park durumu s�f�rlan�r ve ilgili bilgiler g�ncellenir.
// Ara� alma i�lemi, m��teri arac�n� al�p park yerini bo�altt���nda kullan�l�r.

void arac_al(int id, int *toplam_ucret, Musteri *musteriler) {
    musteriler[id - 1001].arac_parked = 0; 
    printf("Araciniz alindi.\n");
}
// Bu fonksiyon, g�n sonu istatistiklerini ekrana yazd�r�r.
// Toplam park eden m��teri say�s�n�, ara� alan m��teri say�s�n� ve �denen toplam �creti raporlar.
// Bu i�lem, i�letme i�in g�nl�k faaliyetlerin �zetini verir.

void raporla(int park_eden, int arac_alan, int toplam_ucret) {
    printf("\nGun Sonu Raporu:\n");
    printf("Park eden musteri sayisi: %d\n", park_eden);
    printf("Aracini alan musteri sayisi: %d\n", arac_alan);
    printf("Toplam odenen miktar: %d TL\n", toplam_ucret);
}
// Bu fonksiyon, sisteme kay�tl� t�m m��terilerin ID'lerini ekrana yazd�r�r.
// M��teri listesindeki kay�tl� kullan�c�lar� g�r�nt�lemek i�in kullan�l�r.
// M��terilerin sisteme giri� yapmadan �nce ID'lerini ��renmelerini sa�lar.

void kayitli_kullanicilari_goster(Musteri *musteriler, int mevcut_musteri_sayisi) {
    int i; 
    printf("\nKayitli Musteriler:\n");
    for (i = 0; i < mevcut_musteri_sayisi; i++) {
        printf("ID: %d\n", musteriler[i].id);
    }
}

