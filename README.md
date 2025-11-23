# Hướng dẫn kết nối phần cứng cho STM32, DHT11 và USB-TTL và hiển thị dữ liệu lên Hercules

Tài liệu này cung cấp hướng dẫn chi tiết về:
*   [Phân tích mã nguồn](#1-phan-tich-ma-nguon)
*   [Hướng dẫn kết nối chi tiết](#2-huong-dan-ket-noi-chi-tiet)
    *   [Kết nối Cảm biến DHT11](#a-ket-noi-cam-bien-dht11)
    *   [Kết nối Mạch chuyển đổi USB-TTL](#b-ket-noi-mach-chuyen-doi-usb-ttl-de-theo-doi-du-lieu-serial)
    *   [Kết nối ST-Link V2](#c-ket-noi-st-link-v2-de-nap-chuong-trinh-va-go-loi)
    *   [Hiển thị dữ liệu lên Hercules SETUP Utility](#d-hien-thi-du-lieu-len-hercules-setup-utility)

---

### 1. Phân tích mã nguồn

Trước khi tiến hành kết nối, chúng ta hãy xem xét các chân GPIO được sử dụng trong mã nguồn:

*   **File `USER\main.c`:**
    *   Các hàm như `GPIO_Config()`, `DHT11_Start()`, và `DHT11_Read_Byte()` đều sử dụng chân **PB12**. Điều này có nghĩa là chân DATA của cảm biến DHT11 được kết nối với chân **PB12** trên STM32 của bạn.
*   **File `USER\usart1.c`:**
    *   Hàm `USART1_DEBUG_Init()` cấu hình chân **PA9** làm chân TX (truyền dữ liệu) và chân **PA10** làm chân RX (nhận dữ liệu) cho giao tiếp USART1.

### 2. Hướng dẫn kết nối chi tiết

Dựa trên phân tích mã nguồn và các tiêu chuẩn kết nối, bạn hãy thực hiện như sau:

#### A. Kết nối Cảm biến DHT11

Cảm biến DHT11 cần ba chân cơ bản: VCC (nguồn), DATA (dữ liệu), và GND (nối đất).

*   **VCC (Chân 1 trên DHT11):** Nối vào chân **3.3V** hoặc **5V** trên bo mạch STM32 của bạn.
*   **DATA (Chân 2 trên DHT11):** Nối vào chân **PB12** trên bo mạch STM32.
*   **GND (Chân 4 trên DHT11):** Nối vào chân **GND** trên bo mạch STM32.

**Lưu ý quan trọng:** Để đảm bảo tín hiệu DATA ổn định, đặc biệt trong môi trường nhiễu hoặc khi dây dài, bạn nên thêm một điện trở kéo lên (pull-up resistor) có giá trị từ **4.7kΩ đến 10kΩ** giữa chân **DATA** của DHT11 và chân **VCC**.

#### B. Kết nối Mạch chuyển đổi USB-TTL (để theo dõi dữ liệu Serial)

Mạch này giúp bạn xem dữ liệu nhiệt độ và độ ẩm mà STM32 gửi qua cổng Serial trên máy tính.

*   **RX (của USB-TTL):** Nối vào chân **PA9** (chân TX của STM32) trên bo mạch STM32.
*   **TX (của USB-TTL):** Nối vào chân **PA10** (chân RX của STM32) trên bo mạch STM32.
*   **GND:** Nối vào chân **GND** trên bo mạch STM32.

Sau khi kết nối, hãy cắm mạch USB-TTL vào máy tính. Bạn cần mở một phần mềm terminal (ví dụ: PuTTY, Tera Term, hoặc Serial Monitor trong Arduino IDE), chọn đúng cổng COM và cài đặt tốc độ Baud Rate là **9600** để đọc dữ liệu.

#### C. Kết nối ST-Link V2 (để nạp chương trình và gỡ lỗi)

ST-Link V2 được sử dụng để nạp firmware (chương trình) vào STM32 và để gỡ lỗi (debug) quá trình hoạt động của vi điều khiển.

*   **SWDIO (ST-Link Pin 7):** Nối vào chân tương ứng trên bo mạch STM32.
*   **SWCLK (ST-Link Pin 9):** Nối vào chân tương ứng trên bo mạch STM32.
*   **GND (ST-Link Pin 4 hoặc 6):** Nối vào chân **GND** trên bo mạch STM32.
*   **3.3V (ST-Link Pin 1 hoặc 2):** Nối vào chân **3.3V** trên bo mạch STM32.
    *   *Lưu ý:* Chân này có thể dùng để cấp nguồn cho STM32 thông qua ST-Link, hoặc chỉ để ST-Link đọc điện áp tham chiếu của bo mạch. Nếu bạn đã cấp nguồn cho STM32 qua USB-TTL hoặc một nguồn khác, việc kết nối chân 3.3V này là tùy chọn hoặc chỉ cần để nhận biết mức logic.

Sau khi kết nối ST-Link V2, bạn có thể sử dụng các công cụ lập trình như Keil uVision, STM32CubeIDE hoặc STM32 ST-Link Utility để nạp chương trình và gỡ lỗi.

#### D. Hiển thị dữ liệu lên Hercules SETUP Utility

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
    *   **Parity:** Đặt là `None` (Không kiểm tra chẵn lẻ).
    *   **Stop Bits:** Đặt là `1` (1 bit dừng).
    *   **HandShake:** Đặt là `Off` (Tắt kiểm soát luồng).

4.  **Mở kết nối:**
    *   Nhấn nút **Open** để bắt đầu kết nối với STM32.
    *   Nếu kết nối thành công, bạn sẽ thấy thông báo "Serial port opened" và cửa sổ màu đen bên dưới sẽ bắt đầu hiển thị dữ liệu nhiệt độ và độ ẩm được gửi từ STM32.

Chúc bạn thành công!