# Telecom Network Monitor 2.0

A modular tool written in C for real-time monitoring of network connection quality (latency and packet loss), optimized for macOS and Linux.

## 🚀 Features
- **External Configuration:** Automatically reads target IP addresses from a `targets.txt` file.
- **Quality Analysis:** Sends multiple concurrent ping packets to accurately calculate Packet Loss percentage.
- **Advanced Statistics:** Calculates and displays minimum, maximum, and average latency per target.
- **Graceful Shutdown:** Intercepts the `Ctrl+C` (`SIGINT`) system signal to cleanly stop monitoring and output a formatted final performance report.

## 🛠️ Project Structure
- `src/` - Source code (`main.c`, `network.c`, `logger.c`)
- `include/` - Header files (`network.h`, `logger.h`)
- `targets.txt` - List of IPs to monitor
- `network_log.csv` - Automatically generated data log

## 💻 How to Run

1. Ensure your desired IPs are listed in `targets.txt` (one per line).
2. Compile and run the project using the following command:

```bash
gcc src/*.c -I include -o monitor_retea && ./monitor_retea