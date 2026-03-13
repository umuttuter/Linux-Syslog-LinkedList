#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* * SYSLOG DÜĞÜMÜ (NODE) YAPISI
 * Her bir log kaydını temsil edecek olan yapı (struct).
 * Çift yönlü bağlı liste kullanıldığı için hem önceki hem de sonraki düğüme işaret eder.
 */
typedef struct SyslogNode {
    char timestamp[32];  // Logun zaman damgası (Örn: "Mar 13 17:46:23")
    char hostname[64];   // Logu üreten makine adı (Örn: "ubuntu-server")
    char appName[64];    // Logu üreten uygulama veya servis (Örn: "sshd", "kernel")
    char message[256];   // Logun asıl mesaj içeriği

    struct SyslogNode* prev; // Bir önceki log kaydını gösteren işaretçi (Geçmişe dönüş için)
    struct SyslogNode* next; // Bir sonraki log kaydını gösteren işaretçi
} SyslogNode;

// Listenin başını (en eski log) ve sonunu (en yeni log) global olarak tutuyoruz
SyslogNode* head = NULL; 
SyslogNode* tail = NULL; 

/*
 * YENİ LOG DÜĞÜMÜ OLUŞTURMA FONKSİYONU
 * Bu fonksiyon bellekte (heap alanında) yeni bir düğüm için yer ayırır 
 * ve içerisine parametre olarak gelen log verilerini kopyalar.
 */
SyslogNode* createLogNode(const char* ts, const char* host, const char* app, const char* msg) {
    // malloc ile bellek tahsisi yapıyoruz
    SyslogNode* newNode = (SyslogNode*)malloc(sizeof(SyslogNode));
    
    // Bellek tahsisi başarısız olursa programın çökmemesi için kontrol
    if (newNode == NULL) {
        printf("HATA: Bellek tahsis edilemedi!\n");
        return NULL;
    }

    // Gelen verileri struct içerisine kopyalıyoruz
    strcpy(newNode->timestamp, ts);
    strcpy(newNode->hostname, host);
    strcpy(newNode->appName, app);
    strcpy(newNode->message, msg);

    // Yeni oluşturulan düğümün henüz bir bağı olmadığı için işaretçileri NULL yapıyoruz
    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}

/*
 * SİSTEM GÜNLÜĞÜ EKLEME (INSERT) FONKSİYONU
 * Gerçek bir işletim sisteminde loglar kronolojik olarak arka arkaya gelir.
 * Bu fonksiyon yeni gelen bir logu listemizin sonuna (tail) ekler. O(1) karmaşıklığında çalışır.
 */
void insertLog(const char* ts, const char* host, const char* app, const char* msg) {
    SyslogNode* newNode = createLogNode(ts, host, app, msg);
    if (newNode == NULL) return; // Düğüm oluşturulamazsa çık

    // Eğer liste tamamen boşsa, ilk log kaydımız budur
    if (head == NULL) {
        head = newNode;
        tail = newNode;
    } 
    // Liste boş değilse, güncel logu en sona (tail'in sonrasına) ekle
    else {
        tail->next = newNode;   // Mevcut son düğümün 'next' işaretçisini yeni düğüme bağla
        newNode->prev = tail;   // Yeni düğümün 'prev' işaretçisini eski son düğüme bağla
        tail = newNode;         // 'tail' işaretçisini artık en sonda olan yeni düğüme kaydır
    }
}

/*
 * TERS YÖNLÜ LOG OKUMA FONKSİYONU (Yeniden Eskiye)
 * ÇİFT YÖNLÜ BAĞLI LİSTE KULLANMAMIZIN ANA SEBEBİ BUDUR!
 * Sistem yöneticileri logları okurken genellikle en son ne olduğuna (en güncel hataya) bakarlar.
 * Bu fonksiyon 'tail' işaretçisinden başlayıp 'prev' işaretçilerini takip ederek 
 * en yeni logdan en eskisine doğru hızlıca tarama yapar.
 */
void printLogsReverse() {
    SyslogNode* current = tail;
    
    if (current == NULL) {
        printf("Sistem günlügü bos.\n");
        return;
    }

    printf("\n--- GUNCEL SISTEM GUNLUKLERI (YENIDEN ESKIYE) ---\n");
    while (current != NULL) {
        printf("[%s] %s %s: %s\n", current->timestamp, current->hostname, current->appName, current->message);
        current = current->prev; // Bir önceki (daha eski) log kaydına geç
    }
    printf("-------------------------------------------------\n\n");
}

/*
 * BELLEK TEMİZLEME FONKSİYONU
 * C dilinde malloc ile aldığımız belleği geri vermek zorundayız. 
 * Program kapanırken tüm listeyi gezip free() işlemi yapar.
 */
void freeSyslogList() {
    SyslogNode* current = head;
    while (current != NULL) {
        SyslogNode* temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    // Demo verileriyle log kayıtları oluşturuyoruz (Zaman akışını simüle ediyoruz)
    insertLog("Mar 13 17:40:01", "ubuntu-server", "kernel", "Linux version 5.15.0-82-generic booting.");
    insertLog("Mar 13 17:42:15", "ubuntu-server", "NetworkManager", "enp3s0: link up, 1000 Mbps, full duplex.");
    insertLog("Mar 13 17:45:00", "ubuntu-server", "sshd", "Accepted publickey for umut from 192.168.1.50 port 54321 ssh2");
    insertLog("Mar 13 17:46:23", "ubuntu-server", "sudo", "umut : TTY=pts/0 ; PWD=/home/umut ; USER=root ; COMMAND=/bin/su");

    // Videoda çalıştırıp göstereceğin demo fonksiyonu
    printLogsReverse();

    // Belleği temizle ve çık
    freeSyslogList();
    
    return 0;
}