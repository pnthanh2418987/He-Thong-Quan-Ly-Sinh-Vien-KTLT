#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cctype>

using namespace std;

// ==========================================
// 1. CẤU TRÚC DỮ LIỆU
// ==========================================
struct Student {
    string id;          // MSSV (Giữ kiểu string để tránh tràn số và giữ số 0 ở đầu)
    string name;        // Họ và tên
    string className;   // Lớp
    string schoolName;  // Tên trường
    string dob;         // Ngày sinh
    double gpa;         // Điểm GPA
};

vector<Student> studentList;

// ==========================================
// 2. CÁC HÀM TIỆN ÍCH & NGHIỆP VỤ
// ==========================================
string classifyStudent(double gpa) {
    if (gpa >= 3.6) return "Xuat sac";
    if (gpa >= 3.2) return "Gioi";
    if (gpa >= 2.5) return "Kha";
    if (gpa >= 2.0) return "Trung binh";
    return "Yeu";
}

void trim(string &s) {
    s.erase(0, s.find_first_not_of(" \t\n\r\f\v"));
    s.erase(s.find_last_not_of(" \t\n\r\f\v") + 1);
}

// Hàm kiểm tra một chuỗi có chứa toàn số hay không
bool isAllDigits(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// ==========================================
// MODULE 5: FILE I/O (LƯU TRỮ CHÍNH)
// ==========================================
void loadFromFile() {
    ifstream file("students.txt");
    if (!file.is_open()) return;

    studentList.clear();
    Student st;
    
    // Đọc MSSV, nếu thành công thì chạy vào trong vòng lặp
    while (file >> st.id) {
        file.ignore(10000, '\n');
        getline(file, st.name);      trim(st.name);
        getline(file, st.className); trim(st.className);
        getline(file, st.schoolName);trim(st.schoolName);
        getline(file, st.dob);       trim(st.dob);
        file >> st.gpa;
        file.ignore(10000, '\n');
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
             << st.schoolName << "\n" 
             << st.dob << "\n"
             << st.gpa << "\n";
    }
    file.close();
}

// ==========================================
// MODULE MỞ RỘNG: IMPORT DỮ LIỆU TỪ FILE KHÁC
// ==========================================
void importFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Loi: Khong the mo file " << filename << "!\n";
        return;
    }

    Student st;
    int addedCount = 0;
    while (file >> st.id) {
        file.ignore();
        getline(file, st.name);
        getline(file, st.className);
        getline(file, st.schoolName);
        getline(file, st.dob);
        file >> st.gpa;
        file.ignore();

        // Kiểm tra trùng lặp trước khi thêm
        bool isDuplicate = false;
        for (const auto& s : studentList) {
            if (s.id == st.id) {
                isDuplicate = true;
                break;
            }
        }

        // Chỉ thêm sinh viên nếu MSSV chưa tồn tại trong danh sách
        if (!isDuplicate) {
            studentList.push_back(st);
            addedCount++;
        }
    }
    file.close();
    cout << "=> Da them thanh cong " << addedCount << " sinh vien moi tu file " << filename << "!\n";
}

// ==========================================
// MODULE 1: QUẢN LÝ SINH VIÊN (CRUD) 
// ==========================================
void addStudent() {
    cout << "\n--- THEM SINH VIEN ---\n";
    cout << "Chon phuong thuc nhap:\n";
    cout << "1. Nhap tu ban phim\n";
    cout << "2. Nhap them tu file (.txt)\n";
    cout << "Lua chon (1-2): ";
    
    int choice;
    while (!(cin >> choice) || (choice < 1 || choice > 2)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Lua chon khong hop le. Vui long nhap lai (1-2): ";
    }

    // Lựa chọn 2: Sử dụng tính năng Import
    if (choice == 2) {
        string filename;
        cout << "Nhap ten file can tai len (vd: input_lopA.txt): ";
        cin >> filename;
        importFromFile(filename);
        return;
    }

    // Lựa chọn 1: Nhập thủ công từ bàn phím
    Student st;
    while (true) {
        cout << "Nhap MSSV (chi gom so, vd 202418987): "; 
        cin >> st.id;
        
        if (!isAllDigits(st.id)) {
            cout << "Loi: MSSV chi duoc chua cac chu so! Vui long nhap lai.\n";
            continue;
        }

        bool isDuplicate = false;
        for (const auto& s : studentList) {
            if (s.id == st.id) {
                cout << "Loi: MSSV da ton tai! Vui long nhap ma khac.\n";
                isDuplicate = true;
                break;
            }
        }
        
        if (!isDuplicate) break;
    }
    
    cin.ignore();
    cout << "Nhap Ho va Ten: "; getline(cin, st.name); trim(st.name);
    cout << "Nhap Lop: "; getline(cin, st.className); trim(st.className);
    cout << "Nhap Khoa/Vien/Truong truc thuoc: "; 
    getline(cin, st.schoolName); trim(st.schoolName);
    cout << "Nhap Ngay sinh (DD/MM/YYYY): "; getline(cin, st.dob); trim(st.dob);
    
    while(true) {
        cout << "Nhap GPA (0.0 - 4.0): "; cin >> st.gpa;
        if(st.gpa >= 0.0 && st.gpa <= 4.0) break;
        cout << "Loi: GPA khong hop le! Vui long nhap lai.\n";
    }

    studentList.push_back(st);
    cout << "=> Them sinh vien thanh cong!\n";
}

void displayStudents() {
    if (studentList.empty()) {
        cout << "\nDanh sach sinh vien trong!\n";
        return;
    }
    cout << "\n" << string(135, '-') << "\n";
    cout << left << setw(15) << "MSSV" 
         << setw(25) << "Ho va Ten" 
         << setw(15) << "Lop" 
         << setw(45) << "Khoa/Vien/Truong" 
         << setw(15) << "Ngay Sinh" 
         << setw(10) << "GPA" 
         << "Xep loai" << "\n";
    cout << string(135, '-') << "\n";
    
    for (const auto& st : studentList) {
        cout << left << setw(15) << st.id 
             << setw(25) << st.name 
             << setw(15) << st.className 
             << setw(45) << st.schoolName 
             << setw(15) << st.dob 
             << setw(10) << fixed << setprecision(2) << st.gpa 
             << classifyStudent(st.gpa) << "\n";
    }
    cout << string(135, '-') << "\n";
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

void updateStudent() {
    string id;
    cout << "\n--- CAP NHAT THONG TIN SINH VIEN ---\n";
    cout << "Nhap MSSV can sua: ";
    cin >> id;

    for (auto &st : studentList) {
        if (st.id == id) {

            cin.ignore();

            cout << "Nhap Ho va Ten moi: ";
            getline(cin, st.name);
            trim(st.name);

            cout << "Nhap Lop moi: ";
            getline(cin, st.className);
            trim(st.className);

            cout << "Nhap Ngay sinh moi (DD/MM/YYYY): ";
            getline(cin, st.dob);
            trim(st.dob);

            while (true) {
                cout << "Nhap GPA moi (0.0 - 4.0): ";

                if (!(cin >> st.gpa)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "GPA phai la so!\n";
                    continue;
                }

                if (st.gpa >= 0.0 && st.gpa <= 4.0)
                    break;

                cout << "GPA phai trong khoang 0.0 - 4.0!\n";
            }

            cout << "=> Cap nhat sinh vien thanh cong!\n";
            return;
        }
    }

    cout << "Loi: Khong tim thay sinh vien co MSSV " << id << "\n";
}
// ==========================================
// MODULE 3: TÌM KIẾM
// ==========================================
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
    sort(studentList.begin(), studentList.end(), [](const Student& a, const Student& b) {
        return a.gpa > b.gpa;
    });
    cout << "=> Da sap xep danh sach theo GPA giam dan!\n";
    displayStudents();
}

void sortByName() {
    sort(studentList.begin(), studentList.end(), [](const Student& a, const Student& b) {
        return a.name < b.name;
    });
    cout << "=> Da sap xep danh sach theo Ten tang dan!\n";
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
    cout << "6. Sap xep danh sach theo Ten\n";
    cout << "7. Thong ke hoc luc\n";
    cout << "8. Sua sinh vien\n";
    cout << "0. Luu va Thoat\n";
    cout << "========================================\n";
    cout << "Nhap lua chon cua ban (0-8): ";
}

int main() {
    loadFromFile(); // Luôn lấy dữ liệu gốc từ students.txt
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
            case 6: sortByName(); break;
            case 7: statistics(); break;
            case 8: updateStudent(); break;
            case 0: 
                saveToFile(); // Chốt sổ toàn bộ về lại students.txt
                cout << "Da luu du lieu. Thoat chuong trinh...\n"; 
                break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);

    return 0;
}
