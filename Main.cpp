#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cctype>

using namespace std;

// ==========================================
// 1. CẤU TRÚC DỮ LIỆU (RELATIONAL MODEL V3)
// ==========================================
// Bảng 1: Lớp học (Đã mở rộng thêm Schema)
struct ClassInfo {
    string classID;         // Khóa chính
    string className;       // Tên lớp
    string homeroomTeacher; // Tên Giáo viên chủ nhiệm
    string teacherID;       // Mã GVCN
    string teacherEmail;    // Email GVCN
};

// Bảng 2: Sinh viên
struct Student {
    string id;          // MSSV (Khóa chính)
    string name;        // Họ và tên
    string classID;     // Khóa ngoại
    string schoolName;  // Tên Khoa/Viện/Trường
    string dob;         // Ngày sinh
    double gpa;         // Điểm GPA
};

vector<ClassInfo> classList;
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

bool isAllDigits(const string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

string generateEmail(string name, string id) {
    for (char &c : name) c = tolower(c);
    stringstream ss(name);
    string word;
    vector<string> words;
    
    while (ss >> word) words.push_back(word);
    if (words.empty()) return "";

    string emailPrefix = words.back() + ".";
    for (size_t i = 0; i < words.size() - 1; ++i) emailPrefix += words[i][0];

    string idSuffix = id;
    if (id.length() > 2) idSuffix = id.substr(2);
    
    return emailPrefix + idSuffix + "@sis.hust.edu.vn";
}

// Hàm JOIN: Kéo toàn bộ thông tin Lớp
ClassInfo getClassDetails(string classID) {
    for (const auto& c : classList) {
        if (c.classID == classID) {
            return c; 
        }
    }
    return {classID, "Chua cap nhat", "Chua cap nhat", "Chua cap nhat", "Chua cap nhat"};
}

// ==========================================
// MODULE 5: FILE I/O 
// ==========================================
void loadData() {
    // 1. Nạp danh sách Lớp (Đọc 5 dòng/lớp)
    ifstream fileClass("classes.txt");
    if (fileClass.is_open()) {
        classList.clear();
        ClassInfo c;
        while (getline(fileClass, c.classID)) {
            getline(fileClass, c.className);
            getline(fileClass, c.homeroomTeacher);
            getline(fileClass, c.teacherID);
            getline(fileClass, c.teacherEmail);
            classList.push_back(c);
        }
        fileClass.close();
    }

    // 2. Nạp danh sách Sinh viên
    ifstream fileStudent("students.txt");
    if (fileStudent.is_open()) {
        studentList.clear();
        Student st;
        while (fileStudent >> st.id) {
            fileStudent.ignore(10000, '\n');
            getline(fileStudent, st.name);        trim(st.name);
            getline(fileStudent, st.classID);     trim(st.classID); 
            getline(fileStudent, st.schoolName);  trim(st.schoolName);
            getline(fileStudent, st.dob);         trim(st.dob);
            fileStudent >> st.gpa;
            fileStudent.ignore(10000, '\n');
            studentList.push_back(st);
        }
        fileStudent.close();
    }
}

void saveData() {
    // 1. Lưu danh sách Lớp (5 dòng)
    ofstream fileClass("classes.txt");
    for (const auto& c : classList) {
        fileClass << c.classID << "\n"
                  << c.className << "\n"
                  << c.homeroomTeacher << "\n"
                  << c.teacherID << "\n"
                  << c.teacherEmail << "\n";
    }
    fileClass.close();

    // 2. Lưu danh sách Sinh viên
    ofstream fileStudent("students.txt");
    for (const auto& st : studentList) {
        fileStudent << st.id << "\n"
                    << st.name << "\n"
                    << st.classID << "\n"
                    << st.schoolName << "\n" 
                    << st.dob << "\n"
                    << st.gpa << "\n";
    }
    fileStudent.close();
}

// ==========================================
// MODULE QUẢN LÝ LỚP HỌC (DANH MỤC)
// ==========================================
void updateClassInfo() {
    string id;
    cout << "\n--- CAP NHAT THONG TIN LOP HOC ---\n";
    cout << "Nhap Ma lop can sua (VD: IT1-01): ";
    cin >> id;
    cin.ignore(10000, '\n');

    for (auto &c : classList) {
        if (c.classID == id) {
            cout << "-> Dang sua thong tin cho lop: " << c.className << "\n";
            
            cout << "Nhap Ten lop moi: ";
            getline(cin, c.className); trim(c.className);
            
            cout << "Nhap Ten GVCN moi: ";
            getline(cin, c.homeroomTeacher); trim(c.homeroomTeacher);
            
            cout << "Nhap Ma GVCN moi: ";
            getline(cin, c.teacherID); trim(c.teacherID);
            
            cout << "Nhap Email GVCN moi: ";
            getline(cin, c.teacherEmail); trim(c.teacherEmail);

            cout << "=> Cap nhat thong tin lop thanh cong!\n";
            return;
        }
    }
    cout << "Loi: Khong tim thay Ma lop " << id << " tren he thong!\n";
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
        file.ignore(10000, '\n');
        getline(file, st.name);          trim(st.name);
        getline(file, st.classID);       trim(st.classID);
        getline(file, st.schoolName);    trim(st.schoolName);
        getline(file, st.dob);           trim(st.dob);
        file >> st.gpa;
        file.ignore(10000, '\n');

        bool classExists = false;
        for (const auto& c : classList) {
            if (c.classID == st.classID) { classExists = true; break; }
        }
        if (!classExists) {
            ClassInfo newClass = {st.classID, "Chua cap nhat", "Chua cap nhat", "Chua cap nhat", "Chua cap nhat"};
            classList.push_back(newClass);
        }

        bool isDuplicate = false;
        for (const auto& s : studentList) {
            if (s.id == st.id) { isDuplicate = true; break; }
        }

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
        cin.clear(); cin.ignore(10000, '\n');
        cout << "Lua chon khong hop le. Vui long nhap lai (1-2): ";
    }

    if (choice == 2) {
        string filename;
        cout << "Nhap ten file can tai len (vd: input.txt): ";
        cin >> filename;
        importFromFile(filename);
        return;
    }

    Student st;
    while (true) {
        cout << "Nhap MSSV (chi gom so, vd 202418987): "; 
        cin >> st.id;
        
        if (!isAllDigits(st.id)) { cout << "Loi: MSSV chi duoc chua cac chu so!\n"; continue; }

        bool isDuplicate = false;
        for (const auto& s : studentList) {
            if (s.id == st.id) {
                cout << "Loi: MSSV da ton tai!\n";
                isDuplicate = true; break;
            }
        }
        if (!isDuplicate) break;
    }
    
    cin.ignore();
    cout << "Nhap Ho va Ten: "; getline(cin, st.name); trim(st.name);
    
    // Nhập Mã Lớp (classID)
    cout << "Nhap Ma lop (VD: IT1-01): "; 
    getline(cin, st.classID); trim(st.classID);
    
    bool classExists = false;
    for (const auto& c : classList) {
        if (c.classID == st.classID) { classExists = true; break; }
    }
    
    // Yêu cầu nhập đủ 5 trường thông tin nếu là lớp mới
    if (!classExists) {
        ClassInfo newClass;
        newClass.classID = st.classID;
        cout << "=> Ma lop nay chua co tren he thong.\n";
        cout << "   Nhap Ten lop (VD: Cong nghe Thong tin 1): ";
        getline(cin, newClass.className); trim(newClass.className);
        cout << "   Nhap Ten GVCN: ";
        getline(cin, newClass.homeroomTeacher); trim(newClass.homeroomTeacher);
        cout << "   Nhap Ma GVCN: ";
        getline(cin, newClass.teacherID); trim(newClass.teacherID);
        cout << "   Nhap Email GVCN: ";
        getline(cin, newClass.teacherEmail); trim(newClass.teacherEmail);
        classList.push_back(newClass);
    }
    
    cout << "Nhap Khoa/Vien/Truong truc thuoc: "; getline(cin, st.schoolName); trim(st.schoolName);
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
         << setw(15) << "Ma Lop" 
         << setw(45) << "Khoa/Vien/Truong" 
         << setw(15) << "Ngay Sinh" 
         << setw(10) << "GPA" 
         << "Xep loai" << "\n";
    cout << string(135, '-') << "\n";
    
    for (const auto& st : studentList) {
        cout << left << setw(15) << st.id 
             << setw(25) << st.name 
             << setw(15) << st.classID 
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
            getline(cin, st.name); trim(st.name);

            cout << "Nhap Ma lop moi: ";
            getline(cin, st.classID); trim(st.classID);

            bool classExists = false;
            for (const auto& c : classList) {
                if (c.classID == st.classID) { classExists = true; break; }
            }
            
            if (!classExists) {
                ClassInfo newClass;
                newClass.classID = st.classID;
                cout << "=> Ma lop nay chua co tren he thong.\n";
                cout << "   Nhap Ten lop: ";
                getline(cin, newClass.className); trim(newClass.className);
                cout << "   Nhap Ten GVCN: ";
                getline(cin, newClass.homeroomTeacher); trim(newClass.homeroomTeacher);
                cout << "   Nhap Ma GVCN: ";
                getline(cin, newClass.teacherID); trim(newClass.teacherID);
                cout << "   Nhap Email GVCN: ";
                getline(cin, newClass.teacherEmail); trim(newClass.teacherEmail);
                classList.push_back(newClass);
            }

            cout << "Nhap Ngay sinh moi (DD/MM/YYYY): ";
            getline(cin, st.dob); trim(st.dob);

            while (true) {
                cout << "Nhap GPA moi (0.0 - 4.0): ";
                if (!(cin >> st.gpa)) {
                    cin.clear(); cin.ignore(10000, '\n');
                    cout << "GPA phai la so!\n";
                    continue;
                }
                if (st.gpa >= 0.0 && st.gpa <= 4.0) break;
                cout << "GPA phai trong khoang 0.0 - 4.0!\n";
            }

            cout << "=> Cap nhat sinh vien thanh cong!\n";
            return;
        }
    }
    cout << "Loi: Khong tim thay sinh vien co MSSV " << id << "\n";
}

// ==========================================
// MODULE 3: TÌM KIẾM CHI TIẾT
// ==========================================
void findByName() {
    string keyword;
    cout << "Nhap ten can tim: ";
    cin.ignore(); getline(cin, keyword);
    trim(keyword);
    
    bool found = false;
    cout << "\nKET QUA TIM KIEM CHI TIET:\n";
    for (const auto& st : studentList) {
        if (st.name.find(keyword) != string::npos) {
            string email = generateEmail(st.name, st.id);
            ClassInfo ci = getClassDetails(st.classID); 
            
            cout << string(60, '-') << "\n"
                 << "MSSV      : " << st.id << "\n"
                 << "Ho va Ten : " << st.name << "\n"
                 << "Email SV  : " << email << "\n"
                 << "---- THONG TIN LOP HOC ----\n"
                 << "Ma Lop    : " << st.classID << "\n"
                 << "Ten Lop   : " << ci.className << "\n"
                 << "GVCN      : " << ci.homeroomTeacher << " (Ma: " << ci.teacherID << ")\n"
                 << "Email GV  : " << ci.teacherEmail << "\n"
                 << string(60, '-') << "\n";
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
    cout << "4. Tim kiem chi tiet (Ho so ca nhan)\n";
    cout << "5. Sap xep danh sach theo GPA\n";
    cout << "6. Sap xep danh sach theo Ten\n";
    cout << "7. Thong ke hoc luc\n";
    cout << "8. Sua thong tin Sinh vien\n";
    cout << "9. Sua thong tin Lop hoc (Danh muc)\n"; // Tùy chọn mới
    cout << "0. Luu va Thoat\n";
    cout << "========================================\n";
    cout << "Nhap lua chon cua ban (0-9): ";
}

int main() {
    loadData(); 
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
            case 9: updateClassInfo(); break; // Gọi hàm mới
            case 0: 
                saveData(); 
                cout << "Da luu du lieu an toan. Thoat chuong trinh...\n"; 
                break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);

    return 0;
}