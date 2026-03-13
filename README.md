# Linux-Syslog-LinkedList
# Linux Syslog Yönetimi ve Çift Yönlü Bağlı Liste Uygulaması

## Proje Hakkında
Bu proje, Linux işletim sistemindeki sistem günlüklerinin (syslog) bellek üzerinde verimli bir şekilde tutulması ve analiz edilmesi amacıyla geliştirilmiştir. C programlama dili kullanılarak, log kayıtlarının saklanması için **Çift Yönlü Bağlı Liste (Doubly Linked List)** veri yapısı tercih edilmiştir.

## Neden Çift Yönlü Bağlı Liste?
Sistem yöneticileri, log analizi yaparken genellikle **en son gerçekleşen (en güncel) olayları** ilk olarak incelemek isterler. Bu projede Çift Yönlü Bağlı Liste kullanılmasının temel sebepleri şunlardır:
* Yeni gelen log kayıtları listenin sonuna (tail) `O(1)` zaman karmaşıklığında, yani performans kaybı olmadan çok hızlıca eklenir.
* Her düğüm hem önceki (`prev`) hem de sonraki (`next`) düğümü işaret ettiği için, loglar listenin sonundan başına doğru (yeniden eskiye) anında ve hiçbir sıralama algoritmasına ihtiyaç duymadan okunabilir.

## Kurulum ve Çalıştırma
Projeyi kendi ortamınızda derleyip test etmek için aşağıdaki adımları izleyebilirsiniz.

### Gereksinimler
* GCC Derleyici (Ubuntu/Linux ortamı tavsiye edilir)

### Derleme
Terminal üzerinden projenin bulunduğu dizine gidin ve kodu derlemek için aşağıdaki komutu çalıştırın:
```bash
gcc main.c -o syslog_app
