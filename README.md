HỆ THỐNG QUẢN LÝ ĐÀO TẠO (MINI SIS)

Một chương trình quản lý sinh viên, lớp học, môn học và điểm số (Mini Student Information System) được viết bằng ngôn ngữ C++. Dự án này là sự kết hợp thực tiễn giữa kiến thức Cấu trúc dữ liệu & Giải thuật (Linked List, Merge Sort) và Cơ sở dữ liệu (Mô hình thực thể liên kết, thao tác JOIN bảng).

TÍNH NĂNG NỔI BẬT

- Quản lý danh mục (CRUD): Thêm, sửa, xóa thông tin Sinh viên, Lớp học, Môn học.
- Quản lý điểm số: Nhập điểm, tự động tính toán GPA (hệ 10 và hệ 4) và phân loại học lực (Xuất sắc, Giỏi, Khá, Trung bình, Yếu).
- Xử lý dữ liệu tối ưu: Sử dụng thuật toán Merge Sort hoán đổi con trỏ để sắp xếp danh sách sinh viên theo Tên hoặc theo GPA mà không làm gián đoạn cấu trúc dữ liệu.
- Truy xuất thông tin (Tương tự JOIN trong SQL): Tra cứu bảng điểm cá nhân, bảng điểm tổng hợp của toàn lớp.
- Tiện ích tự động: Chuẩn hóa chuỗi (xóa khoảng trắng thừa, viết hoa chữ cái đầu) và tự động sinh email sinh viên theo định dạng chuẩn (VD: thanh.pn2418987@sis.hust.edu.vn).
- Lưu trữ vĩnh viễn (File I/O): Hỗ trợ Import/Export dữ liệu từ các file .txt (classes.txt, students.txt, courses.txt, grades.txt).

CẤU TRÚC DỮ LIỆU

Hệ thống lưu trữ dữ liệu hoàn toàn trên RAM thông qua 4 Danh sách liên kết đơn (Singly Linked Lists), mô phỏng lại 4 bảng trong Cơ sở dữ liệu quan hệ:
1. ClassNode: Lưu trữ thông tin lớp học và giáo viên chủ nhiệm.
2. StudentNode: Lưu trữ hồ sơ sinh viên.
3. CourseNode: Lưu trữ danh mục môn học và số tín chỉ.
4. GradeNode: Lưu trữ điểm thi chi tiết của từng sinh viên theo môn học và học kỳ.

CÀI ĐẶT & HƯỚNG DẪN SỬ DỤNG

Yêu cầu hệ thống:
- Một trình biên dịch C++ (Hỗ trợ tiêu chuẩn C++11 trở lên, khuyến nghị C++17).
- Môi trường thực thi: Windows (MinGW), Linux (GCC), hoặc macOS (Clang).

Cách chạy chương trình:
1. Clone hoặc tải mã nguồn về máy.
2. Mở terminal/command prompt tại thư mục chứa file nguồn.
3. Biên dịch chương trình: g++ main.cpp -o minisis
4. Chạy chương trình:
   - Trên Windows: minisis.exe
   - Trên Linux/macOS: ./minisis

Lưu ý về File dữ liệu:
Khi chạy lần đầu, chương trình sẽ báo danh sách trống. Có thể chọn chức năng "Import từ File (.txt)" trong menu hoặc tự tạo các file classes.txt, students.txt, courses.txt, grades.txt cùng cấp với file thực thi để load dữ liệu mẫu. Khi chọn "0. Luu du lieu va Thoat", hệ thống sẽ tự động ghi đè và lưu lại toàn bộ thay đổi vào các file này.
