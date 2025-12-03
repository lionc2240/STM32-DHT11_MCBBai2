# Giải thích Giao thức Giao tiếp One-Wire của Cảm biến DHT11

Giao thức giao tiếp được sử dụng bởi cảm biến nhiệt độ và độ ẩm DHT11 là một giao thức đơn dây (one-wire) tùy chỉnh. Điều này có nghĩa là toàn bộ dữ liệu được truyền và nhận chỉ trên một đường dây duy nhất. Thay vì sử dụng một tín hiệu clock (xung nhịp) riêng biệt để đồng bộ hóa, giao thức này mã hóa thông tin trực tiếp vào **độ dài thời gian (timing)** của các xung tín hiệu điện.

## Quá trình Giao tiếp

Toàn bộ quá trình giao tiếp để đọc dữ liệu từ cảm biến diễn ra trong khoảng 4ms và bao gồm các bước sau:

### 1. MCU Gửi Tín hiệu Khởi động (Start Signal)

Đây là bước để vi điều khiển (MCU) "đánh thức" cảm biến và yêu cầu nó bắt đầu gửi dữ liệu.

- **Bước 1.1:** MCU cấu hình chân DATA là `output` và kéo đường truyền xuống mức **thấp (LOW)**.
- **Bước 1.2:** MCU giữ mức thấp này trong một khoảng thời gian **ít nhất là 18 mili giây (ms)**.
- **Bước 1.3:** MCU kéo đường truyền lên mức **cao (HIGH)** và chờ phản hồi từ DHT11 trong khoảng **20-40 micro giây (us)**. Sau đó, MCU chuyển chân DATA thành `input` để sẵn sàng lắng nghe.

### 2. DHT11 Phản hồi (Response Signal)

Sau khi nhận được tín hiệu khởi động từ MCU, DHT11 sẽ gửi một tín hiệu phản hồi để xác nhận nó đã sẵn sàng.

- **Bước 2.1:** DHT11 kéo đường truyền xuống mức **thấp (LOW)** trong **80 micro giây (us)**.
- **Bước 2.2:** DHT11 kéo đường truyền lên mức **cao (HIGH)** trong **80 micro giây (us)** để chuẩn bị cho việc truyền dữ liệu.

### 3. DHT11 Truyền Dữ liệu (Data Transmission)

Đây là phần cốt lõi của giao thức, nơi dữ liệu được mã hóa thành các xung. DHT11 sẽ gửi một chuỗi 40 bit dữ liệu (16 bit độ ẩm, 16 bit nhiệt độ, 8 bit checksum).

Mỗi bit được truyền đi theo quy tắc sau:

- **Bắt đầu bit:** Mỗi bit luôn bắt đầu bằng một xung **thấp (LOW)** kéo dài **50 micro giây (us)**.
- **Mã hóa bit:** Giá trị của bit (`0` hay `1`) được quyết định bởi độ dài của xung **cao (HIGH)** ngay sau đó:
    - **Bit 0:** Xung cao kéo dài **26-28 micro giây (us)**.
    - **Bit 1:** Xung cao kéo dài **70 micro giây (us)**.

## Tóm tắt

Việc giao tiếp thành công với DHT11 đòi hỏi sự chính xác tuyệt đối về mặt thời gian ở cả cấp độ mili giây và micro giây.
- MCU phải **tạo ra** các độ trễ chính xác để gửi đúng tín hiệu khởi động.
- MCU phải **đo lường** chính xác độ dài các xung tín hiệu nhận về để giải mã đúng từng bit dữ liệu.

Bất kỳ sai lệch nào trong việc định thời gian này sẽ dẫn đến việc đọc dữ liệu sai hoặc thất bại hoàn toàn.