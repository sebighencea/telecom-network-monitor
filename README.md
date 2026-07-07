# Telecom Network Monitor 2.0

Un instrument modular scris în C pentru monitorizarea calității conexiunii de rețea (latență și packet loss) în timp real, optimizat pentru macOS și Linux.

## 🚀 Caracteristici
- **Configurare externă:** Citește automat adresele IP țintă dintr-un fișier `targets.txt`.
- **Analiză de calitate:** Trimite mai multe pachete ping simultan pentru a calcula procentul de pierderi (*Packet Loss*).
- **Statistici avansate:** Calculează latența minimă, maximă și latența medie per IP.
- **Închidere elegantă:** Interceptează semnalul `Ctrl+C` (`SIGINT`) pentru a opri monitorizarea curat și a afișa un raport final formatat.

## 🛠️ Structura Proiectului
- `src/` - Conține codul sursă (`main.c`, `network.c`, `logger.c`)
- `include/` - Conține fișierele header (`network.h`, `logger.h`)
- `targets.txt` - Lista de IP-uri de monitorizat
- `network_log.csv` - Jurnalul de date generat automat

## 💻 Cum se rulează

1. Asigură-te că ai IP-urile dorite în `targets.txt` (câte unul pe linie).
2. Compilează și rulează proiectul folosind comanda:

```bash
gcc src/*.c -I include -o monitor_retea && ./monitor_retea