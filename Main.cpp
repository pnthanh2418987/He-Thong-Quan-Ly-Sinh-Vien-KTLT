#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// ==========================================
// 1. CẤU TRÚC DỮ LIỆU
// ==========================================
// Định nghĩa cấu trúc Sinh viên
struct Student {
    string id;          // MSSV
    string name;        // Họ và tên
    string className;   // Lớp
    string dob;         // Ngày sinh
    double gpa;         // Điểm GPA
};

vector<Student> studentList;

// ==========================================
// CÁC HÀM TIỆN ÍCH & NGHIỆP VỤ
// ==========================================
// Hàm xếp loại học lực dựa trên GPA 
string classifyStudent(double gpa) {
    if (gpa >= 3.6) return "Xuat sac";
    if (gpa >= 3.2) return "Gioi";
    if (gpa >= 2.5) return "Kha";
    if (gpa >= 2.0) return "Trung binh";
    return "Yeu";
}

// Hàm chuẩn hóa chuỗi (xóa khoảng trắng thừa) 
void trim(string &s) {
    s.erase(0, s.find_first_not_of(" \t\n\r\f\v"));
    s.erase(s.find_last_not_of(" \t\n\r\f\v") + 1);
}

// ==========================================
// MODULE 5: FILE I/O
// ==========================================
void loadFromFile() {
    ifstream file("students.txt");
    if (!file.is_open()) return;

    studentList.clear();
    Student st;
    while (file >> st.id) {
        file.ignore(); // Bỏ qua ký tự newline
        getline(file, st.name);
        getline(file, st.className);
        getline(file, st.dob);
        file >> st.gpa;
        studentList.push_back(st);
    }
    file.close();
}

void saveToFile() {
    ofstream file("students.txt");
    for (const auto& st : studentList) {
        file << st.id << "\n"
             << st.name << "\n"
             << st.className << "\n"
             << st.dob << "\n"
             << st.gpa << "\n";
    }
    file.close();
}

// ==========================================
// MODULE 1: QUẢN LÝ SINH VIÊN (CRUD) 
// ==========================================
void addStudent() {
    Student st;
    cout << "\n--- THEM SINH VIEN ---\n";
    cout << "Nhap MSSV: "; cin >> st.id;
    
    // Kiểm tra trùng lặp MSSV 
    for (const auto& s : studentList) {
        if (s.id == st.id) {
            cout << "Loi: MSSV da ton tai!\n";
            return;
        }
    }
    
    cin.ignore();
    cout << "Nhap Ho va Ten: "; getline(cin, st.name); trim(st.name);
    cout << "Nhap Lop: "; getline(cin, st.className); trim(st.className);
    cout << "Nhap Ngay sinh (DD/MM/YYYY): "; getline(cin, st.dob); trim(st.dob);
    cout << "Nhap GPA (0.0 - 4.0): "; cin >> st.gpa;
    
    if(st.gpa < 0.0 || st.gpa > 4.0) {
        cout << "Loi: GPA khong hop le!\n";
        return;
    }

    studentList.push_back(st);
    cout << "=> Them sinh vien thanh cong!\n";
}

void displayStudents() {
    if (studentList.empty()) {
        cout << "\nDanh sach sinh vien trong!\n";
        return;
    }
    cout << "\n" << string(90, '-') << "\n";
    cout << left << setw(15) << "MSSV" 
         << setw(25) << "Ho va Ten" 
         << setw(15) << "Lop" 
         << setw(15) << "Ngay Sinh" 
         << setw(10) << "GPA" 
         << "Xep loai" << "\n";
    cout << string(90, '-') << "\n";
    
    for (const auto& st : studentList) {
        cout << left << setw(15) << st.id 
             << setw(25) << st.name 
             << setw(15) << st.className 
             << setw(15) << st.dob 
             << setw(10) << fixed << setprecision(2) << st.gpa 
             << classifyStudent(st.gpa) << "\n";
    }
    cout << string(90, '-') << "\n";
}

void deleteStudent() {
    string id;
    cout << "Nhap MSSV can xoa: "; cin >> id;
    for (auto it = studentList.begin(); it != studentList.end(); ++it) {
        if (it->id == id) {
            studentList.erase(it);
            cout << "=> Da xoa sinh vien khoi he thong!\n";
            return;
        }
    }
    cout << "Loi: Khong tim thay sinh vien co MSSV " << id << "\n";
}

// ==========================================
// MODULE 3: TÌM KIẾM
// ==========================================
// Áp dụng tìm kiếm tuần tự (Linear Search) theo tên 
void findByName() {
    string keyword;
    cout << "Nhap ten can tim: ";
    cin.ignore(); getline(cin, keyword);
    trim(keyword);
    
    bool found = false;
    cout << "\nKET QUA TIM KIEM:\n";
    for (const auto& st : studentList) {
        if (st.name.find(keyword) != string::npos) {
            cout << "- MSSV: " << st.id << " | Ten: " << st.name << " | Lop: " << st.className << "\n";
            found = true;
        }
    }
    if (!found) cout << "Khong tim thay sinh vien nao phu hop.\n";
}

// ==========================================
// MODULE 4: SẮP XẾP & THỐNG KÊ 
// ==========================================
void sortByGPA() {
    // Sử dụng thuật toán sắp xếp của thư viện algorithm (ưu tiên O(n log n) tương tự Merge Sort) [cite: 818]
    sort(studentList.begin(), studentList.end(), [](const Student& a, const Student& b) {
        return a.gpa > b.gpa; // Giảm dần
    });
    cout << "=> Da sap xep danh sach theo GPA giam dan!\n";
    displayStudents();
}

void statistics() {
    if (studentList.empty()) {
        cout << "\nKhong co du lieu de thong ke.\n";
        return;
    }
    int xs = 0, gioi = 0, kha = 0, tb = 0, yeu = 0;
    for(const auto& st : studentList) {
        string loai = classifyStudent(st.gpa);
        if(loai == "Xuat sac") xs++;
        else if(loai == "Gioi") gioi++;
        else if(loai == "Kha") kha++;
        else if(loai == "Trung binh") tb++;
        else yeu++;
    }
    
    int total = studentList.size();
    cout << "\n--- THONG KE HOC LUC ---\n";
    cout << "Tong so sinh vien: " << total << "\n";
    cout << "- Xuat sac  : " << xs << " (" << (xs*100.0/total) << "%)\n";
    cout << "- Gioi      : " << gioi << " (" << (gioi*100.0/total) << "%)\n";
    cout << "- Kha       : " << kha << " (" << (kha*100.0/total) << "%)\n";
    cout << "- Trung binh: " << tb << " (" << (tb*100.0/total) << "%)\n";
    cout << "- Yeu       : " << yeu << " (" << (yeu*100.0/total) << "%)\n";
}

// ==========================================
// MODULE 6: GIAO DIỆN MENU 
// ==========================================
void showMenu() {
    cout << "\n========================================\n";
    cout << "      HE THONG QUAN LY SINH VIEN        \n";
    cout << "========================================\n";
    cout << "1. Them sinh vien\n";
    cout << "2. Hien thi danh sach\n";
    cout << "3. Xoa sinh vien\n";
    cout << "4. Tim kiem theo Ten\n";
    cout << "5. Sap xep danh sach theo GPA\n";
    cout << "6. Thong ke hoc luc\n";
    cout << "0. Luu va Thoat\n";
    cout << "========================================\n";
    cout << "Nhap lua chon cua ban (0-6): ";
}

int main() {
    loadFromFile(); // Tự động nạp dữ liệu khi khởi động chương trình
    int choice;
    
    do {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Lua chon khong hop le. Vui long nhap so!\n";
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: deleteStudent(); break;
            case 4: findByName(); break;
            case 5: sortByGPA(); break;
            case 6: statistics(); break;
            case 0: 
                saveToFile(); // Lưu dữ liệu trước khi thoát
                cout << "Da luu du lieu. Thoat chuong trinh...\n"; 
                break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);

    return 0;
}