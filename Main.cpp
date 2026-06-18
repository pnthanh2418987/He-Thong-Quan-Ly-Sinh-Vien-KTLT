#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cctype>

using namespace std;

// 1. CẤU TRÚC DỮ LIỆU (4 LINKED LIST) 

// Áp dụng kiến thức môn CSDL để tạo 4 bảng: ClassInfo (Lớp học),Student (Sinh viên), Course (Môn học), Grade (Điểm).
// Dùng Linked List để lưu trữ dữ liệu.

struct ClassInfo { 
    string classID;
    string className;
    string homeroomTeacher;
    string teacherID;
    string teacherEmail;
};

struct ClassNode {
    ClassInfo data; 
    ClassNode* next;
};

struct Student { 
    string id;
    string name;
    string classID;
    string schoolName;
    string dob;
};

struct StudentNode { 
    Student data;
    StudentNode* next; 
};

struct Course { 
    string courseID;
    string courseName;
    int credits; 
};

struct CourseNode {
    Course data; 
    CourseNode* next;
};

struct Grade {
    string studentID;
    string courseID;
    string semester;
    double score10;
};

struct GradeNode {
    Grade data;
    GradeNode* next;
};

// Con trỏ Head của 4 Linked List
ClassNode* classHead = nullptr;
StudentNode* studentHead = nullptr;
CourseNode* courseHead = nullptr;
GradeNode* gradeHead = nullptr;

// Đã ổn

// 2. CÁC HÀM TIỆN ÍCH

bool isAllDigits(const string& str) {
    if (str.length() == 0) return false;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

void trim(string &s) {
    int left = 0;
    while (left < s.length() && (s[left] == ' ' || s[left] == '\t' || s[left] == '\n' || s[left] == '\r')) left++;
    
    int right = s.length() - 1;
    while (right >= 0 && (s[right] == ' ' || s[right] == '\t' || s[right] == '\n' || s[right] == '\r')) right--;
    if (left > right) s = "";
    else s = s.substr(left, right - left + 1);
}

bool hasNoSpaces(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') return false;
    }
    return true;
}

void formatName(string &name) {
    trim(name);
    if (name.empty()) return;
    string result = "";
    bool newWord = true;
    for (char c : name) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (!result.empty() && result.back() != ' ') {
                result += ' ';
            }
            newWord = true;
        } else {
            if (newWord) {
                result += (c >= 'a' && c <= 'z') ? (c - 32) : c;
                newWord = false;
            } else {
                result += (c >= 'A' && c <= 'Z') ? (c + 32) : c;
            }
        }
    }
    if (!result.empty() && result.back() == ' ') result.pop_back();
    name = result;
}

void cleanSpaces(string &s) {
    trim(s);
    if (s.empty()) return;
    string result = "";
    for (char c : s) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (!result.empty() && result.back() != ' ') {
                result += ' ';
            }
        } else {
            result += c;
        }
    }
    if (!result.empty() && result.back() == ' ') result.pop_back();
    s = result;
}

bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (char c : name) {
        if ((c >= '0' && c <= '9') || ispunct((unsigned char)c)) return false;
    }
    return true;
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDate(const string& date) {
    if (date.length() != 10 || date[2] != '/' || date[5] != '/') return false;
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5) {
            if (date[i] < '0' || date[i] > '9') return false;
        }
    }
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;

    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (isLeapYear(year)) daysInMonth[2] = 29;

    if (day < 1 || day > daysInMonth[month]) return false;
    return true;
}
bool containsSubstring(const string& text, const string& keyword) {
    if (keyword.length() == 0) return true;
    if (text.length() < keyword.length()) return false;
    for (int i = 0; i <= text.length() - keyword.length(); i++) {
        bool match = true;
        for (int j = 0; j < keyword.length(); j++) {
            char t = text[i + j], k = keyword[j];
            if (t >= 'A' && t <= 'Z') t += 32; // Đưa về chữ thường để thuận tiện so sánh
            if (k >= 'A' && k <= 'Z') k += 32;
            if (t != k) { match = false; break; }
        }
        if (match) return true;
    }
    return false;
}

string generateEmail(string name, string id) { // Email mẫu: thanh.pn2418987@sis.hust.edu.vn
    string lowerName = name;
    for (int i = 0; i < lowerName.length(); i++) {
        if (lowerName[i] >= 'A' && lowerName[i] <= 'Z') lowerName[i] += 32; // Đưa tên về chữ thường
    }
    
    string words[50];
    int wordCount = 0;
    string currentWord = "";
    for (int i = 0; i < lowerName.length(); i++) {
        if (lowerName[i] == ' ') {
            if (currentWord != "") {
                words[wordCount++] = currentWord;
                currentWord = ""; }
        } else currentWord += lowerName[i]; // Cắt bỏ họ và tên đệm, chỉ lấy tên chính và chữ cái đầu của họ tên đệm
    } 
    if (currentWord != "") words[wordCount++] = currentWord; 
    if (wordCount == 0) return "";

    string emailPrefix = words[wordCount - 1] + "."; // thanh.
    for (int i = 0; i < wordCount - 1; ++i) emailPrefix += words[i][0]; // thanh.pn
    string idSuffix = id; if (id.length() > 2) idSuffix = id.substr(2); // Bỏ 2 số đầu của MSSV
    return emailPrefix + idSuffix + "@sis.hust.edu.vn"; // thanh.pn2418987@sis.hust.edu.vn
}

double convert10To4(double score10) {
    if (score10 >= 8.5) return 4.0;
    if (score10 >= 8.0) return 3.5;
    if (score10 >= 7.0) return 3.0;
    if (score10 >= 6.5) return 2.5;
    if (score10 >= 5.5) return 2.0;
    if (score10 >= 5.0) return 1.5;
    if (score10 >= 4.0) return 1.0;
    return 0.0;
}

string classifyGPA(double gpa4) {
    if (gpa4 >= 3.6) return "Xuat sac";
    if (gpa4 >= 3.2) return "Gioi";
    if (gpa4 >= 2.5) return "Kha";
    if (gpa4 >= 2.0) return "Trung binh";
    if (gpa4 > 0) return "Yeu";
    return "Chua co diem";
}

// Đã ổn

// 3. LOGIC QUẢN LÝ BỘ NHỚ VÀ TÍNH ĐIỂM

void addClass(ClassInfo c) {
    ClassNode* newNode = new ClassNode{c, nullptr};
    if (!classHead) classHead = newNode;
    else { 
        ClassNode* t = classHead;
        while (t->next) t = t->next;
        t->next = newNode;
    }
}

void addStudentNode(Student s) {
    StudentNode* newNode = new StudentNode{s, nullptr};
    if (!studentHead) studentHead = newNode;
    else {
        StudentNode* t = studentHead;
        while (t->next) t = t->next;
        t->next = newNode;
    }
}

void addCourse(Course c) {
    CourseNode* newNode = new CourseNode{c, nullptr};
    if (!courseHead) courseHead = newNode;
    else { 
        CourseNode* t = courseHead; 
        while (t->next) t = t->next; 
        t->next = newNode; 
    }
}

void addOrUpdateGrade(Grade g) {
    GradeNode* t = gradeHead;
    while (t) {
        if (t->data.studentID == g.studentID && t->data.courseID == g.courseID) {
            t->data.score10 = g.score10; return;
        }
        t = t->next;
    }
    GradeNode* newNode = new GradeNode{g, nullptr};
    if (!gradeHead) gradeHead = newNode;
    else {
        t = gradeHead; while (t->next) t = t->next;
        t->next = newNode;
    }
}

Course* getCourse(string courseID) {
    CourseNode* t = courseHead;
    while (t) {
        if (t->data.courseID == courseID) {
            return &(t->data);
        }
        t = t->next;
    }
    return nullptr;
}

ClassInfo getClassDetails(string classID) {
    ClassNode* t = classHead;
    while (t) {
        if (t->data.classID == classID) return t->data;
        t = t->next;
    }
    return {classID, "Chua cap nhat", "Chua cap nhat", "Chua cap nhat", "Chua cap nhat"};
}

// Áp dụng kiến thức môn CSDL để tính toán GPA từ bảng Grade và bảng Course, sau đó phân loại GPA theo thang 4.0.
// Hàm JOIN động: Tính GPA từ 2 bảng Điểm và Môn học.

void calculateGPA(string studentID, string targetSemester, double &gpa10, double &gpa4, int &totalCredits) {
    gpa10 = 0.0; gpa4 = 0.0; totalCredits = 0;
    double sum10 = 0.0, sum4 = 0.0;
    
    GradeNode* g = gradeHead;
    while (g) {
        // Kiểm tra xem có thuộc học kỳ mục tiêu không (hoặc đang tính tích lũy)
        if (g->data.studentID == studentID && 
           (targetSemester == "" || g->data.semester == targetSemester)) {
            
            Course* c = getCourse(g->data.courseID);
            if (c) {
                sum10 += g->data.score10 * c->credits;
                sum4 += convert10To4(g->data.score10) * c->credits;
                totalCredits += c->credits;
            }
        }
        g = g->next;
    }
    if (totalCredits > 0) {
        gpa10 = sum10 / totalCredits;
        gpa4 = sum4 / totalCredits;
    }
}

void clearAllData() {
    while(classHead) { ClassNode* t = classHead; classHead = classHead->next; delete t; }
    while(studentHead) { StudentNode* t = studentHead; studentHead = studentHead->next; delete t; }
    while(courseHead) { CourseNode* t = courseHead; courseHead = courseHead->next; delete t; }
    while(gradeHead) { GradeNode* t = gradeHead; gradeHead = gradeHead->next; delete t; }
}

// Đã ổn

// 4. MODULE FILE I/O

void loadData() {
    clearAllData();

    ifstream fClass("classes.txt");
    if (fClass.is_open()) {
        ClassInfo c;
        while (getline(fClass, c.classID)) {
            getline(fClass, c.className);
            getline(fClass, c.homeroomTeacher);
            getline(fClass, c.teacherID);
            getline(fClass, c.teacherEmail); addClass(c);
        }
        fClass.close();
    }

    ifstream fStudent("students.txt");
    if (fStudent.is_open()) {
        Student st;
        while (fStudent >> st.id) {
            fStudent.ignore(10000, '\n');
            getline(fStudent, st.name);
            trim(st.name);

            getline(fStudent, st.classID);
            trim(st.classID);

            getline(fStudent, st.schoolName);
            trim(st.schoolName);

            getline(fStudent, st.dob); trim(st.dob); addStudentNode(st);
        }
        fStudent.close();
    }

    ifstream fCourse("courses.txt");
    if (fCourse.is_open()) {
        Course c;
        while (fCourse >> c.courseID) {
            fCourse.ignore(10000, '\n');
            getline(fCourse, c.courseName);
            trim(c.courseName);
            fCourse >> c.credits; fCourse.ignore(10000, '\n');
            addCourse(c);
        }
        fCourse.close();
    }

    ifstream fGrade("grades.txt");
    if (fGrade.is_open()) {
        Grade g; 
        // Đọc liên tiếp 3 chuỗi không khoảng trắng và 1 số thực số điểm
        while (fGrade >> g.studentID >> g.courseID >> g.semester >> g.score10) {
            addOrUpdateGrade(g);
        }
        fGrade.close();
    }
}

void saveData() {
    ofstream fClass("classes.txt");

    for (ClassNode* t = classHead; t; t = t->next)
        fClass << t->data.classID << "\n" << t->data.className << "\n" << t->data.homeroomTeacher << "\n" << t->data.teacherID << "\n" << t->data.teacherEmail << "\n";
    fClass.close();

    ofstream fStudent("students.txt");

    for (StudentNode* t = studentHead; t; t = t->next)
        fStudent << t->data.id << "\n" << t->data.name << "\n" << t->data.classID << "\n" << t->data.schoolName << "\n" << t->data.dob << "\n";
    fStudent.close();

    ofstream fCourse("courses.txt");

    for (CourseNode* t = courseHead; t; t = t->next)
        fCourse << t->data.courseID << "\n" << t->data.courseName << "\n" << t->data.credits << "\n";
    fCourse.close();

    ofstream fGrade("grades.txt");

    for (GradeNode* t = gradeHead; t; t = t->next)
        fGrade << t->data.studentID << "\n" << t->data.courseID << "\n" << t->data.semester << "\n" << t->data.score10 << "\n";
    fGrade.close();
}

//Tạm ổn

// 5. NGHIỆP VỤ CRUD (Create, Read, Update, Delete) DANH MỤC

// CÁC HÀM IMPORT DỮ LIỆU TỪ FILE

void importClassesFromFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) { cout << "Loi: Khong the mo file " << filename << "!\n"; return; }
    ClassInfo c; int count = 0;
    while (getline(file, c.classID)) {
        trim(c.classID); if (c.classID.empty()) continue;
        getline(file, c.className); trim(c.className);
        getline(file, c.homeroomTeacher); trim(c.homeroomTeacher);
        getline(file, c.teacherID); trim(c.teacherID);
        getline(file, c.teacherEmail); trim(c.teacherEmail);
        
        bool dup = false;
        for (ClassNode* t = classHead; t; t=t->next) if (t->data.classID == c.classID) dup=true;
        if (!dup) { addClass(c); count++; }
    }
    file.close(); cout << "=> Da import thanh cong " << count << " lop hoc!\n";
}

void importStudentsFromFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) { cout << "Loi: Khong the mo file " << filename << "!\n"; return; }
    Student st; int count = 0;
    while (file >> st.id) {
        file.ignore(10000, '\n'); trim(st.id);
        getline(file, st.name); trim(st.name);
        getline(file, st.classID); trim(st.classID);
        getline(file, st.schoolName); trim(st.schoolName);
        getline(file, st.dob); trim(st.dob);
        
        bool dup = false;
        for (StudentNode* t = studentHead; t; t=t->next) if (t->data.id == st.id) dup=true;
        if (!dup) {
            // Tự động tạo lớp rỗng nếu mã lớp chưa tồn tại
            bool cExists = false;
            for (ClassNode* t = classHead; t; t=t->next) if (t->data.classID == st.classID) cExists = true;
            if (!cExists) addClass({st.classID, "Chua cap nhat", "Chua cap nhat", "Chua cap nhat", "Chua cap nhat"});
            addStudentNode(st); count++;
        }
    }
    file.close(); cout << "=> Da import thanh cong " << count << " sinh vien!\n";
}

void importCoursesFromFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) { cout << "Loi: Khong the mo file " << filename << "!\n"; return; }
    Course c; int count = 0;
    while (file >> c.courseID) {
        file.ignore(10000, '\n'); trim(c.courseID);
        getline(file, c.courseName); trim(c.courseName);
        file >> c.credits; file.ignore(10000, '\n');
        
        bool dup = false;
        for (CourseNode* t = courseHead; t; t=t->next) if (t->data.courseID == c.courseID) dup=true;
        if (!dup) { addCourse(c); count++; }
    }
    file.close(); cout << "=> Da import thanh cong " << count << " mon hoc!\n";
}

void importGradesFromFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) { cout << "Loi: Khong the mo file " << filename << "!\n"; return; }
    Grade g; int count = 0;
    while (file >> g.studentID >> g.courseID >> g.semester >> g.score10) {
        addOrUpdateGrade(g); count++;
    }
    file.close(); cout << "=> Da import/cap nhat thanh cong " << count << " cot diem!\n";
}

void addStudent() { // Tạm ổn phần ràng buộc dữ liệu
    cout << "\n--- THEM SINH VIEN ---\n";
    cout << "1. Nhap thu cong\n2. Import tu File (.txt)\nLua chon: ";
    int choice; cin >> choice; cin.ignore(10000, '\n');
    if (choice == 2) {
        string fname; cout << "Nhap ten file: "; cin >> fname; 
        importStudentsFromFile(fname); 
        return;
    }
    Student st;
    cin.ignore(10000, '\n'); 

    while (true) {
        cout << "Nhap MSSV: "; getline(cin, st.id); trim(st.id);
        if (!isAllDigits(st.id) || !hasNoSpaces(st.id)) { 
            cout << "Loi: MSSV chi duoc chua so va khong chua khoang trang!\n"; 
            continue; 
        }
        bool dup = false;
        for (StudentNode* t = studentHead; t; t = t->next) if (t->data.id == st.id) dup = true;
        if (dup) cout << "Loi: MSSV da ton tai!\n"; else break;
    }
    
    while (true) {
        cout << "Nhap Ho va Ten: ";
        getline(cin, st.name);
        formatName(st.name);
        if (!isValidName(st.name)) {
            cout << "Loi: Ten khong hop le (khong chua so/ky tu dac biet)!\n";
            continue;
        }
        break;
    }

    while (true) {
        cout << "Nhap Ma lop (VD: IT1-01): ";
        getline(cin, st.classID); trim(st.classID);
        if (!hasNoSpaces(st.classID)) {
            cout << "Loi: Ma lop khong duoc phep chua khoang trang!\n";
            continue;
        }
        break;
    }
    bool cExists = false;
    for (ClassNode* t = classHead; t; t = t->next) if (t->data.classID == st.classID) cExists = true;
    if (!cExists) {
        ClassInfo nc; nc.classID = st.classID;
        cout << "=> Lop chua co. Nhap Ten lop: ";
        getline(cin, nc.className); trim(nc.className);

        cout << "   Nhap Ten GVCN: ";
        getline(cin, nc.homeroomTeacher); trim(nc.homeroomTeacher);

        cout << "   Nhap Ma GVCN: ";
        getline(cin, nc.teacherID); trim(nc.teacherID);

        cout << "   Nhap Email GVCN: ";
        getline(cin, nc.teacherEmail); trim(nc.teacherEmail);
        addClass(nc);
    }
    cout << "Nhap Truong truc thuoc: ";
    getline(cin, st.schoolName); trim(st.schoolName);

    cout << "Nhap Ngay sinh (DD/MM/YYYY): ";
    getline(cin, st.dob); trim(st.dob);

    addStudentNode(st); cout << "=> Them sinh vien thanh cong!\n";
}

void updateStudent() {
    string id; cout << "Nhap MSSV can sua: "; cin >> id; cin.ignore(10000, '\n');
    for (StudentNode* s = studentHead; s; s = s->next) {
        if (s->data.id == id) {
            cout << "--- CHON MUC CAN CAP NHAT TAI KHOAN TRUONG ---\n"
                 << "1. Cap nhat Ho va Ten\n"
                 << "2. Cap nhat Ma lop\n"
                 << "3. Cap nhat Ngay sinh\n"
                 << "4. Cap nhat Truong truc thuoc\n"
                 << "Chon chuc nang: ";
            int sub; cin >> sub; cin.ignore(10000, '\n');
            
            if (sub == 1) {
                while (true) {
                    cout << "Nhap Ho Ten moi: ";
                    getline(cin, s->data.name);
                    formatName(s->data.name);
                    if (isValidName(s->data.name)) break;
                    cout << "Loi: Ten khong hop le!\n";
                }
            } else if (sub == 2) {
                while (true) {
                    cout << "Nhap Ma lop moi: ";
                    getline(cin, s->data.classID);
                    trim(s->data.classID);
                    if (hasNoSpaces(s->data.classID)) break;
                    cout << "Loi: Ma lop khong duoc chua khoang trang!\n";
                }
            } else if (sub == 3) {
                while (true) {
                    cout << "Nhap Ngay sinh moi (DD/MM/YYYY): ";
                    getline(cin, s->data.dob); trim(s->data.dob);
                    if (isValidDate(s->data.dob)) break;
                    cout << "Loi: Ngay sinh sai logic hoac dinh dang!\n";
                }
            } else if (sub == 4) {
                while (true) {
                    cout << "Nhap Truong truc thuoc moi: ";
                    getline(cin, s->data.schoolName);
                    cleanSpaces(s->data.schoolName);
                    if (!s->data.schoolName.empty()) break;
                }
            }
            cout << "=> Cap nhat va chuan hoa ho so xong!\n"; return;
        }
    }
    cout << "Loi: Khong tim thay sinh vien!\n";
}

void deleteGradesByStudentID(string studentID) {
    while (gradeHead && gradeHead->data.studentID == studentID) {
        GradeNode* temp = gradeHead;
        gradeHead = gradeHead->next;
        delete temp;
    }
    GradeNode* curr = gradeHead;
    while (curr && curr->next) {
        if (curr->next->data.studentID == studentID) {
            GradeNode* temp = curr->next;
            curr->next = curr->next->next;
            delete temp;
        } else {
            curr = curr->next;
        }
    }
}

void deleteStudent() {
    string id; cout << "Nhap MSSV can xoa: "; cin >> id;
    if (!studentHead) return;
    
    if (studentHead->data.id == id) {
        StudentNode* temp = studentHead; studentHead = studentHead->next; delete temp;
        deleteGradesByStudentID(id); // Dọn dẹp cả điểm của sinh viên bị xóa
        cout << "=> Da xoa sinh vien va cac diem lien quan!\n"; return;
    }
    for (StudentNode* curr = studentHead; curr->next; curr = curr->next) {
        if (curr->next->data.id == id) {
            StudentNode* temp = curr->next; curr->next = curr->next->next; delete temp;
            deleteGradesByStudentID(id);
            cout << "=> Da xoa sinh vien va cac diem lien quan!\n"; return;
        }
    }
    cout << "Khong tim thay!\n";
}

void updateClassInfo() {
    cout << "\n--- QUAN LY LOP HOC ---\n";
    cout << "1. Them Lop thu cong\n2. Import tu File (.txt)\n3. Sua thong tin lop\nLua chon: ";
    int choice; cin >> choice; cin.ignore(10000, '\n');
    
    if (choice == 2) {
        string fname; cout << "Nhap ten file: "; cin >> fname; 
        importClassesFromFile(fname); 
        return;
    } else if (choice == 1) {
        ClassInfo nc;
        cout << "Nhap Ma lop: ";
        getline(cin, nc.classID); trim(nc.classID);
        for(ClassNode* t = classHead; t; t = t->next) 
            if(t->data.classID == nc.classID) { cout << "Loi: Lop da ton tai!\n"; return; }
        
        cout << "Nhap Ten lop: ";
        getline(cin, nc.className);
        cleanSpaces(nc.className);

        cout << "Nhap Ten GVCN: ";
        getline(cin, nc.homeroomTeacher);
        formatName(nc.homeroomTeacher);

        cout << "Nhap Ma GVCN: ";
        getline(cin, nc.teacherID); trim(nc.teacherID);

        cout << "Nhap Email GVCN: ";
        getline(cin, nc.teacherEmail); trim(nc.teacherEmail);
        addClass(nc); 

        cout << "=> Them lop thanh cong!\n";
        return;
    } else if (choice == 3) {
        string id; cout << "Nhap Ma lop can sua: ";
        getline(cin, id); trim(id);
        for (ClassNode* c = classHead; c; c = c->next) {
            if (c->data.classID == id) {
                while (true) {
                    cout << "Nhap Ten lop moi: ";
                    getline(cin, c->data.className);
                    cleanSpaces(c->data.className);
                    if (!c->data.className.empty()) break;
                }
                while (true) {
                    cout << "Nhap Ten GVCN: ";
                    getline(cin, c->data.homeroomTeacher); formatName(c->data.homeroomTeacher);
                    if (isValidName(c->data.homeroomTeacher)) break;
                    cout << "Loi: Ten khong duoc chua ki tu dac biet hoac so!\n";
                }
                cout << "=> Sua lop va chuan hoa thong tin thanh cong!\n"; return;
            }
        }
        cout << "Loi: Khong tim thay Lop!\n";
    }
}

void inputCourse() {
    Course c; 
    cout << "\n--- THEM MON HOC ---\n";
    cout << "1. Nhap thu cong\n2. Import tu File (.txt)\nLua chon: ";
    int choice; cin >> choice; cin.ignore(10000, '\n');
    if (choice == 2) {
        string fname; cout << "Nhap ten file: "; cin >> fname; 
        importCoursesFromFile(fname); 
        return;
    }
    while (true) {
        cout << "Nhap Ma mon (Khong khoang trang): ";
        getline(cin, c.courseID); trim(c.courseID);
        if (!hasNoSpaces(c.courseID)) {
            cout << "Loi: Ma mon hoc khong duoc phep chua khoang trang!\n";
            continue;
        }
        if (getCourse(c.courseID)) { cout << "Loi: Mon nay da co!\n"; return; }
        break;
    }
    while (true) {
        cout << "Nhap Ten mon: ";
        getline(cin, c.courseName);
        cleanSpaces(c.courseName);
        if (!c.courseName.empty()) break;
    }
    while (true) {
        cout << "Nhap So tin chi (1-10): ";
        string credStr; getline(cin, credStr); trim(credStr);
        if (isAllDigits(credStr) && !credStr.empty()) {
            c.credits = 0;
            for(char ch : credStr) c.credits = c.credits * 10 + (ch - '0');
            if (c.credits > 0 && c.credits <= 10) break;
        }
        cout << "Loi: Tin chi phai la so thuc te (1 - 10)!\n";
    }
    addCourse(c); 
    cout << "=> Da them va chuan hoa mon hoc!\n";
}

void inputGrade() {
    Grade g; 
    cout << "\n--- NHAP DIEM ---\n";
    
    // --- CHÈN ĐOẠN NÀY VÀO ---
    cout << "1. Nhap thu cong\n2. Import tu File (.txt)\nLua chon: ";
    int choice; cin >> choice; cin.ignore(10000, '\n');
    if (choice == 2) {
        string fname; cout << "Nhap ten file: "; cin >> fname; 
        importGradesFromFile(fname); 
        return;
    }
    cout << "Nhap MSSV: "; cin >> g.studentID; cin.ignore(10000, '\n');
    
    bool found = false; 
    for(StudentNode* t = studentHead; t; t = t->next) {
        if(t->data.id == g.studentID) found = true;
    }
    if (!found) { 
        cout << "Loi: Khong co sinh vien nay!\n"; 
        return; 
    }
    
    cout << "Nhap Ma mon hoc: "; cin >> g.courseID; cin.ignore(10000, '\n');
    if (!getCourse(g.courseID)) { 
        cout << "Loi: Mon hoc chua ton tai!\n"; 
        return; 
    }
    
    while (true) {
        cout << "Nhap Hoc ky (VD: 20251): "; 
        getline(cin, g.semester); trim(g.semester);
        if (!g.semester.empty() && hasNoSpaces(g.semester)) {
            break;
        }
        cout << "Loi: Hoc ky khong duoc de trong va khong chua khoang trang!\n";
    }
    
    while (true) {
        cout << "Nhap Diem he 10 (0.0 - 10.0): ";
        if (cin >> g.score10 && g.score10 >= 0.0 && g.score10 <= 10.0) {
            cin.ignore(10000, '\n');
            break;
        }
        cin.clear(); cin.ignore(10000, '\n');
        cout << "Loi: Diem so phai la chuoi so hop le tu 0.0 den 10.0!\n";
    }
    
    addOrUpdateGrade(g); 
    cout << "=> Da cap nhat diem cho hoc ky " << g.semester << " thanh cong!\n";
}

// Tạm ổn

// 6. MODULE BÁO CÁO & XỬ LÝ

void displayStudents() {
    if (!studentHead) { cout << "\nDanh sach trong!\n"; return; }
    cout << "\n" << string(115, '-') << "\n";
    cout << left << setw(12) << "MSSV" << setw(25) << "Ho va Ten" << setw(15) << "Ma Lop" 
         << setw(15) << "Ngay Sinh" << setw(10) << "GPA 10" << setw(10) << "GPA 4" << "Xep loai\n";
    cout << string(115, '-') << "\n";
    
    for (StudentNode* s = studentHead; s; s = s->next) {
        double gpa10, gpa4; int tc; 
        calculateGPA(s->data.id, "", gpa10, gpa4, tc); 
        
        cout << left << setw(12) << s->data.id << setw(25) << s->data.name << setw(15) << s->data.classID 
             << setw(15) << s->data.dob << setw(10) << fixed << setprecision(2) << gpa10 
             << setw(10) << gpa4 << classifyGPA(gpa4) << "\n";
    }
    cout << string(115, '-') << "\n";
}

void findAndPrintTranscript() {
    string kw; cout << "Nhap Ten hoac MSSV can tim: "; cin.ignore(10000, '\n'); getline(cin, kw); trim(kw);
    bool found = false;
    for (StudentNode* s = studentHead; s; s = s->next) {
        if (containsSubstring(s->data.name, kw) || containsSubstring(s->data.id, kw)) {
            ClassInfo ci = getClassDetails(s->data.classID);
            double gpa10, gpa4; int tc; calculateGPA(s->data.id, "", gpa10, gpa4, tc);
            
            cout << "\n" << string(80, '=') << "\n";
            cout << "HO SO SINH VIEN: " << s->data.id << " - " << s->data.name << "\n";
            cout << "Email: " << generateEmail(s->data.name, s->data.id) << "\n";
            cout << "Lop: " << ci.className << " (GVCN: " << ci.homeroomTeacher << ")\n";
            cout << string(80, '-') << "\n";
            cout << left << setw(15) << "Ma Mon" 
                 << setw(25) << "Ten Mon" 
                 << setw(15) << "Hoc Ky"
                 << setw(10) << "TC" 
                 << "Diem\n";
            cout << string(80, '-') << "\n";
            
            for (GradeNode* g = gradeHead; g; g = g->next) {
                if (g->data.studentID == s->data.id) {
                    Course* c = getCourse(g->data.courseID);
                    if (c) {
                        cout << left << setw(15) << c->courseID 
                             << setw(25) << c->courseName 
                             << setw(15) << g->data.semester
                             << setw(10) << c->credits 
                             << fixed << setprecision(1) << g->data.score10 << "\n";
                    }
                }
            }
            cout << string(80, '-') << "\n"; // Cập nhật độ rộng
            cout << "Tong TC: " << tc << " | GPA 10: " << fixed << setprecision(2) << gpa10 
                 << " | GPA 4: " << gpa4 << " (" << classifyGPA(gpa4) << ")\n";
            cout << string(80, '=') << "\n"; // Cập nhật độ rộng
            found = true;
        }
    }
    if (!found) cout << "Khong tim thay ket qua!\n";
}
void printClassTranscript() {
    string cid; cout << "Nhap Ma Lop can xem (VD: IT1-01): "; cin >> cid;
    cout << "\nBANG DIEM TONG HOP LOP: " << cid << "\n";
    cout << string(80, '-') << "\n";
    cout << left << setw(15) << "MSSV" << setw(25) << "Ho va Ten" << setw(10) << "So TC" 
         << setw(10) << "GPA 10" << setw(10) << "GPA 4" << "Xep loai\n";
    cout << string(80, '-') << "\n";

    bool found = false;
    for (StudentNode* s = studentHead; s; s = s->next) {
        if (s->data.classID == cid) {
            double gpa10, gpa4; int tc; calculateGPA(s->data.id, "", gpa10, gpa4, tc);
            cout << left << setw(15) << s->data.id << setw(25) << s->data.name << setw(10) << tc 
                 << setw(10) << fixed << setprecision(2) << gpa10 << setw(10) << gpa4 << classifyGPA(gpa4) << "\n";
            found = true;
        }
    }
    if (!found) cout << "Khong co sinh vien lop nay.\n";
    cout << string(80, '-') << "\n";
}

// Áp dụng kiến thức Câu trúc dữ liệu và giải thuật để sắp xếp danh sách sinh viên theo các tiêu chí khác nhau (Tên, GPA, MSSV) sử dụng thuật toán Merge Sort hoán đổi con trỏ.

void splitList(StudentNode* source, StudentNode** frontRef, StudentNode** backRef) {
    StudentNode* fast;
    StudentNode* slow;
    slow = source;
    fast = source->next;
    
    while (fast != nullptr) {
        fast = fast->next;
        if (fast != nullptr) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *frontRef = source;
    *backRef = slow->next;
    slow->next = nullptr;
}

StudentNode* sortedMerge(StudentNode* a, StudentNode* b, int criteria) {
    if (!a) return b;
    if (!b) return a;
    
    StudentNode* result = nullptr;
    bool condition = false; 
    
    if (criteria == 1) {
        condition = (a->data.name <= b->data.name);
    } 
    else if (criteria == 2) {
        double g10_a, g4_a, g10_b, g4_b; int tc;
        calculateGPA(a->data.id, "", g10_a, g4_a, tc);
        calculateGPA(b->data.id, "", g10_b, g4_b, tc);
        condition = (g4_a >= g4_b); 
    } 
    else if (criteria == 3) {
        condition = (a->data.id <= b->data.id);
    }

    if (condition) {
        result = a;
        result->next = sortedMerge(a->next, b, criteria);
    } else {
        result = b;
        result->next = sortedMerge(a, b->next, criteria);
    }
    return result;
}

void mergeSort(StudentNode** headRef, int criteria) {
    StudentNode* head = *headRef;
    StudentNode* a;
    StudentNode* b;
    
    if ((head == nullptr) || (head->next == nullptr)) return;
    
    splitList(head, &a, &b);
    
    mergeSort(&a, criteria);
    mergeSort(&b, criteria);
    
    *headRef = sortedMerge(a, b, criteria);
}

void sortByName() {
    mergeSort(&studentHead, 1);
    cout << "=> Da sap xep theo Ten! (Dung Merge Sort hoan doi con tro)\n";
}

void sortByGPA() {
    mergeSort(&studentHead, 2);
    cout << "=> Da sap xep theo GPA Giam dan!\n";
}

void sortByID() {
    mergeSort(&studentHead, 3);
    cout << "=> Da sap xep theo MSSV!\n";
}

void statistics() {
    int xs = 0, gioi = 0, kha = 0, tb = 0, yeu = 0, cd = 0, total = 0;
    for (StudentNode* s = studentHead; s; s = s->next) {
        double gpa10, gpa4; int tc; 
        // SỬA TẠI ĐÂY: Thêm "" để tính GPA tích lũy phục vụ thống kê
        calculateGPA(s->data.id, "", gpa10, gpa4, tc); 
        
        string loai = classifyGPA(gpa4);
        if (loai == "Xuat sac") xs++; else if (loai == "Gioi") gioi++;
        else if (loai == "Kha") kha++; else if (loai == "Trung binh") tb++;
        else if (loai == "Yeu") yeu++; else cd++;
        total++;
    }
    if (total == 0) { cout << "Chua co sinh vien!\n"; return; }
    cout << "\n--- THONG KE HOC LUC (" << total << " SV) ---\n";
    cout << "- Xuat sac   : " << xs << " (" << (xs*100.0/total) << "%)\n";
    cout << "- Gioi       : " << gioi << " (" << (gioi*100.0/total) << "%)\n";
    cout << "- Kha        : " << kha << " (" << (kha*100.0/total) << "%)\n";
    cout << "- Trung binh : " << tb << " (" << (tb*100.0/total) << "%)\n";
    cout << "- Yeu        : " << yeu << " (" << (yeu*100.0/total) << "%)\n";
    cout << "- Chua co diem: " << cd << "\n";
}


// 7. MENU

// 7. MENU

void showMenu() {
    cout << "\n========================================\n";
    cout << "   HE THONG QUAN LY DAO TAO (MINI SIS)  \n";
    cout << "========================================\n";
    cout << "--- HO SO DANH MUC ---\n";
    cout << "1. Them Sinh vien moi\n";
    cout << "2. Sua / Xoa Sinh vien\n";
    cout << "3. Cap nhat thong tin Lop hoc\n";
    cout << "4. Them Mon hoc vao He thong\n";
    cout << "--- HOC TAP & BAO CAO ---\n";
    cout << "5. Nhap / Sua Diem cho Sinh vien\n";
    cout << "6. Hien thi Danh sach Sinh vien (Co GPA)\n";
    cout << "7. Xem Ho so chi tiet & Bang diem ca nhan\n";
    cout << "8. Xem Bang diem Tong hop cua Lop\n";
    cout << "9. Sap xep danh sach SV (Theo Ten / GPA)\n";
    cout << "10. Thong ke hoc luc toan truong\n";
    cout << "0. Luu du lieu va Thoat\n";
    cout << "========================================\n";
    cout << "Nhap lua chon: ";
}

// 8. MAIN

int main() {
    loadData(); 
    int choice;
    do {
        showMenu();
        if (!(cin >> choice)) { cin.clear(); cin.ignore(10000, '\n'); continue; }
        switch (choice) {
            case 1: addStudent(); break;
            case 2: 
                int sub; cout << "1-Sua, 2-Xoa: "; cin >> sub;
                if (sub==1) updateStudent(); else if (sub==2) deleteStudent();
                break;
            case 3: updateClassInfo(); break;
            case 4: inputCourse(); break;
            case 5: inputGrade(); break;
            case 6: displayStudents(); break;
            case 7: findAndPrintTranscript(); break;
            case 8: printClassTranscript(); break;
            case 9:
                int s; cout << "1-Theo Ten, 2-Theo GPA: "; cin >> s;
                if (s==1) sortByName(); else if (s==2) sortByGPA();
                break;
            case 10: statistics(); break;
            case 0: saveData(); clearAllData(); cout << "Da luu va giai phong bo nho. Thoat!\n"; break;
            default: cout << "Lua chon khong hop le!\n";
        }
    } while (choice != 0);
    return 0;
}