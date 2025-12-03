# Hướng dẫn kết nối phần cứng cho STM32, DHT11 và USB-TTL và hiển thị dữ liệu lên Hercules

Tài liệu này cung cấp hướng dẫn chi tiết về:
- [Hướng dẫn kết nối phần cứng cho STM32, DHT11 và USB-TTL và hiển thị dữ liệu lên Hercules](#hướng-dẫn-kết-nối-phần-cứng-cho-stm32-dht11-và-usb-ttl-và-hiển-thị-dữ-liệu-lên-hercules)
  - [1. Phân tích mã nguồn](#1-phân-tích-mã-nguồn)
  - [2. Hướng dẫn kết nối chi tiết](#2-hướng-dẫn-kết-nối-chi-tiết)
    - [A. Kết nối Cảm biến DHT11](#a-kết-nối-cảm-biến-dht11)
    - [B. Kết nối Mạch chuyển đổi USB-TTL (để theo dõi dữ liệu Serial)](#b-kết-nối-mạch-chuyển-đổi-usb-ttl-để-theo-dõi-dữ-liệu-serial)
    - [C. Kết nối ST-Link V2 (để nạp chương trình và gỡ lỗi)](#c-kết-nối-st-link-v2-để-nạp-chương-trình-và-gỡ-lỗi)
    - [D. Hiển thị dữ liệu lên Hercules SETUP Utility](#d-hiển-thị-dữ-liệu-lên-hercules-setup-utility)

---
## Hướng dẫn kết nối chi tiết
| **Thiết bị** | **Chân trên thiết bị** | **Kết nối tới** | **Chân trên STM32** | **Ghi chú** |
|--------------|------------------------|-----------------|---------------------|-------------|
| **DHT11** | VCC (Chân 1) | Nguồn | `3.3V` hoặc `5V` | Dùng 5V nếu DHT11 hỗ trợ; thường ổn hơn 3.3V |
| | DATA (Chân 2) | Dữ liệu | `PB12` |  (Cần điện trở kéo lên `4.7kΩ–10kΩ` giữa DATA và VCC) |
| | GND (Chân 4) | Nối đất | `GND` | — |
| **USB-TTL** | TX | RX | `PA10` |  *TX ↔ RX* chéo |
| | RX | TX | `PA9` |  *RX ↔ TX* chéo |
| | GND | GND | `GND` | — |
| **ST-Link V2** | SWDIO (Pin 7) | SWDIO | `PA13` *(mặc định)* | Một số bo STM32F103 đã nối sẵn tới header SWD |
| | SWCLK (Pin 9) | SWCLK | `PA14` *(mặc định)* | — |
| | GND (Pin 4/6) | GND | `GND` | — |
| | 3.3V (Pin 1/2) | 3.3V (tùy chọn) | `3.3V` | Dùng để cấp nguồn hoặc làm tham chiếu điện áp |

## Hiển thị dữ liệu lên Hercules SETUP Utility

Sau khi đã nạp code thành công và kết nối mạch USB-TTL với máy tính, bạn có thể sử dụng phần mềm Hercules SETUP Utility để hiển thị dữ liệu từ STM32.

1.  **Tải xuống Hercules:**
    *   Bạn có thể tải phần mềm Hercules SETUP Utility từ trang web chính thức của HW group tại đây: [https://www.hw-group.com/software/hercules-setup-utility](https://www.hw-group.com/software/hercules-setup-utility)

2.  **Cài đặt và cấu hình Hercules:**
    *   Sau khi tải về, giải nén và chạy file `hercules.exe`. Không cần cài đặt.
    *   Mở phần mềm Hercules.
    *   Chuyển đến tab **Serial**.

3.  **Thiết lập các thông số:**
    *   **Name:** Chọn cổng COM tương ứng với mạch USB-TTL của bạn. (Bạn có thể kiểm tra cổng COM đang sử dụng trong Device Manager của Windows, phần `Ports (COM & LPT)`).
    *   **Baud:** Đặt là `9600` (Đây là tốc độ baud rate được cấu hình trong code STM32 của bạn).
    *   **Data Size:** Đặt là `8` (8 bit dữ liệu).
    *   **Parity:** (Giữ nguyên) Đặt là `None` (Không kiểm tra chẵn lẻ).
    *   **HandShake:** (Giữ nguyên) Đặt là `Off` (Tắt kiểm soát luồng).
    *   **Mode:** (Giữ nguyên) Free

4.  **Mở kết nối:**
    *   Nhấn nút **Open** để bắt đầu kết nối với STM32.
    *   Nếu kết nối thành công, bạn sẽ thấy thông báo "Serial port opened" và cửa sổ màu đen bên dưới sẽ bắt đầu hiển thị dữ liệu nhiệt độ và độ ẩm được gửi từ STM32.

Chúc bạn thành công!