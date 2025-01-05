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


// Bu fonksiyon, programýn ana giriþ noktasýdýr.
// Kullanýcýyla etkileþime girer, seçimlerini alýr ve uygun fonksiyonlarý çaðýrarak iþlemleri yönetir.
// - Otopark sistemi için gerekli olan baþlangýç deðerlerini kullanýcýdan alýr.
// - Kullanýcýdan kayýt durumu (kayýtlý/kayýtsýz) bilgisi ister ve buna göre iþlem yapar.
//   - Kayýtlýysa doðrulama yapar ve araç park etme ya da alma iþlemini gerçekleþtirir.
//   - Kayýtlý deðilse, kullanýcýyý sisteme kaydeder.
// - Program sonunda, gün sonu raporunu ekrana yazdýrýr.
// - Dinamik olarak tahsis edilen belleði temizleyerek, programýn düzgün bir þekilde sonlanmasýný saðlar.
// Programýn temel iþ akýþýný kontrol eder ve tüm fonksiyonlarýn koordinasyonunu saðlar.

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
            printf("Gecersiz secim! Lütfen 1, 0 veya 9 girin.\n");
        }
    }

    raporla(park_eden, arac_alan, toplam_ucret);
    printf("Cikis yapildi.\n");

    free(musteriler);

    return 0;
}
// Bu fonksiyon, müþterinin sisteme kayýtlý olup olmadýðýný kontrol eder.
// Kullanýcýdan alýnan ID ve PIN bilgilerini mevcut müþteri listesiyle karþýlaþtýrýr.
// Eðer bilgiler doðruysa 1 döndürür, aksi takdirde 0 döndürür.
// Bu iþlem, müþteri doðrulama iþlemleri için kullanýlýr.

int uyelik_dogrula(int id, int pin, Musteri *musteriler, int mevcut_musteri_sayisi) {
    int i;  
    for (i = 0; i < mevcut_musteri_sayisi; i++) {
        if (musteriler[i].id == id && musteriler[i].pin == pin) {
            return 1; 
        }
    }
    return 0;  
}
// Bu fonksiyon, sisteme yeni müþteri eklemek için kullanýlýr.
// Kullanýcýdan yeni bir ID ve PIN alýr ve müþteri listesine ekler.
// Eðer mevcut müþteri sayýsý artýyorsa, bellek yeniden tahsis edilir (reallocate).
// Yeni müþteri bilgileri sisteme üye olarak kaydedilir.

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
// Bu fonksiyon, müþterinin aracýný park etmesini saðlar.
// Müþterinin park etmek istediði süre alýnýr ve bu süreye göre ücret hesaplanýr.
// Hesaplanan ücret, toplam park ücretine eklenir ve müþteri durumu güncellenir.
// Müþteri, park alanýnda yer bulduðunda bu fonksiyon çalýþtýrýlýr.

void arac_park(int id, int saat, int *toplam_ucret, Musteri *musteriler) {
    int ucret = saat * 20;  
    *toplam_ucret += ucret;
    printf("Park ucreti: %d TL\n", ucret);
    musteriler[id - 1001].arac_parked = 1; 
}
// Bu fonksiyon, park etmiþ bir müþterinin aracýný almasýný saðlar.
// Araç alýndýðýnda müþterinin park durumu sýfýrlanýr ve ilgili bilgiler güncellenir.
// Araç alma iþlemi, müþteri aracýný alýp park yerini boþalttýðýnda kullanýlýr.

void arac_al(int id, int *toplam_ucret, Musteri *musteriler) {
    musteriler[id - 1001].arac_parked = 0; 
    printf("Araciniz alindi.\n");
}
// Bu fonksiyon, gün sonu istatistiklerini ekrana yazdýrýr.
// Toplam park eden müþteri sayýsýný, araç alan müþteri sayýsýný ve ödenen toplam ücreti raporlar.
// Bu iþlem, iþletme için günlük faaliyetlerin özetini verir.

void raporla(int park_eden, int arac_alan, int toplam_ucret) {
    printf("\nGun Sonu Raporu:\n");
    printf("Park eden musteri sayisi: %d\n", park_eden);
    printf("Aracini alan musteri sayisi: %d\n", arac_alan);
    printf("Toplam odenen miktar: %d TL\n", toplam_ucret);
}
// Bu fonksiyon, sisteme kayýtlý tüm müþterilerin ID'lerini ekrana yazdýrýr.
// Müþteri listesindeki kayýtlý kullanýcýlarý görüntülemek için kullanýlýr.
// Müþterilerin sisteme giriþ yapmadan önce ID'lerini öðrenmelerini saðlar.

void kayitli_kullanicilari_goster(Musteri *musteriler, int mevcut_musteri_sayisi) {
    int i; 
    printf("\nKayitli Musteriler:\n");
    for (i = 0; i < mevcut_musteri_sayisi; i++) {
        printf("ID: %d\n", musteriler[i].id);
    }
}

