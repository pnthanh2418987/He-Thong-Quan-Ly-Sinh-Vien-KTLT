#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

// ==========================================
// 1. CẤU TRÚC DỮ LIỆU TỰ CÀI ĐẶT (4 BẢNG LINKED LIST)
// ==========================================
struct ClassInfo { string classID, className, homeroomTeacher, teacherID, teacherEmail; };
struct ClassNode { ClassInfo data; ClassNode* next; };

struct Student { string id, name, classID, schoolName, dob; }; // Không còn lưu GPA tĩnh
struct StudentNode { Student data; StudentNode* next; };

struct Course { string courseID, courseName; int credits; };
struct CourseNode { Course data; CourseNode* next; };

struct Grade { string studentID, courseID; double score10; };
struct GradeNode { Grade data; GradeNode* next; };

// Con trỏ Head của 4 bảng
ClassNode* classHead = nullptr;
StudentNode* studentHead = nullptr;
CourseNode* courseHead = nullptr;
GradeNode* gradeHead = nullptr;

// ==========================================
// 2. CÁC HÀM TIỆN ÍCH THỦ CÔNG (KHÔNG DÙNG THƯ VIỆN)
// ==========================================
bool isAllDigits(const string& str) {
    if (str.length() == 0) return false;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

void trim(string &s) {
    int start = 0;
    while (start < s.length() && (s[start] == ' ' || s[start] == '\t' || s[start] == '\n' || s[start] == '\r')) start++;
    int end = s.length() - 1;
    while (end >= 0 && (s[end] == ' ' || s[end] == '\t' || s[end] == '\n' || s[end] == '\r')) end--;
    if (start > end) s = "";
    else s = s.substr(start, end - start + 1);
}

bool containsSubstring(const string& text, const string& keyword) {
    if (keyword.length() == 0) return true;
    if (text.length() < keyword.length()) return false;
    for (int i = 0; i <= text.length() - keyword.length(); i++) {
        bool match = true;
        for (int j = 0; j < keyword.length(); j++) {
            char t = text[i + j], k = keyword[j];
            if (t >= 'A' && t <= 'Z') t += 32;
            if (k >= 'A' && k <= 'Z') k += 32;
            if (t != k) { match = false; break; }
        }
        if (match) return true;
    }
    return false;
}

string generateEmail(string name, string id) {
    string lowerName = name;
    for (int i = 0; i < lowerName.length(); i++) {
        if (lowerName[i] >= 'A' && lowerName[i] <= 'Z') lowerName[i] += 32;
    }
    string words[50]; int wordCount = 0; string currentWord = "";
    for (int i = 0; i < lowerName.length(); i++) {
        if (lowerName[i] == ' ') {
            if (currentWord != "") { words[wordCount++] = currentWord; currentWord = ""; }
        } else currentWord += lowerName[i];
    }
    if (currentWord != "") words[wordCount++] = currentWord;
    if (wordCount == 0) return "";

    string emailPrefix = words[wordCount - 1] + ".";
    for (int i = 0; i < wordCount - 1; ++i) emailPrefix += words[i][0];
    string idSuffix = id; if (id.length() > 2) idSuffix = id.substr(2);
    return emailPrefix + idSuffix + "@sis.hust.edu.vn";
}

double convert10To4(double score10) {
    if (score10 >= 8.5) return 4.0; if (score10 >= 8.0) return 3.5;
    if (score10 >= 7.0) return 3.0; if (score10 >= 6.5) return 2.5;
    if (score10 >= 5.5) return 2.0; if (score10 >= 5.0) return 1.5;
    if (score10 >= 4.0) return 1.0; return 0.0;
}

string classifyGPA(double gpa4) {
    if (gpa4 >= 3.6) return "Xuat sac"; if (gpa4 >= 3.2) return "Gioi";
    if (gpa4 >= 2.5) return "Kha";      if (gpa4 >= 2.0) return "Trung binh";
    if (gpa4 > 0) return "Yeu";         return "Chua co diem";
}

// ==========================================
// 3. LOGIC QUẢN LÝ BỘ NHỚ VÀ TÍNH ĐIỂM
// ==========================================
void addClass(ClassInfo c) {
    ClassNode* newNode = new ClassNode{c, nullptr};
    if (!classHead) classHead = newNode;
    else { ClassNode* t = classHead; while (t->next) t = t->next; t->next = newNode; }
}

void addStudentNode(Student s) {
    StudentNode* newNode = new StudentNode{s, nullptr};
    if (!studentHead) studentHead = newNode;
    else { StudentNode* t = studentHead; while (t->next) t = t->next; t->next = newNode; }
}

void addCourse(Course c) {
    CourseNode* newNode = new CourseNode{c, nullptr};
    if (!courseHead) courseHead = newNode;
    else { CourseNode* t = courseHead; while (t->next) t = t->next; t->next = newNode; }
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
    else { t = gradeHead; while (t->next) t = t->next; t->next = newNode; }
}

Course* getCourse(string courseID) {
    CourseNode* t = courseHead;
    while (t) { if (t->data.courseID == courseID) return &(t->data); t = t->next; }
    return nullptr;
}

ClassInfo getClassDetails(string classID) {
    ClassNode* t = classHead;
    while (t) { if (t->data.classID == classID) return t->data; t = t->next; }
    return {classID, "Chua cap nhat", "Chua cap nhat", "Chua cap nhat", "Chua cap nhat"};
}

// Hàm JOIN động: Tính GPA từ 2 bảng Điểm và Môn học
void calculateGPA(string studentID, double &gpa10, double &gpa4, int &totalCredits) {
    gpa10 = 0.0; gpa4 = 0.0; totalCredits = 0;
    double sum10 = 0.0, sum4 = 0.0;
    
    GradeNode* g = gradeHead;
    while (g) {
        if (g->data.studentID == studentID) {
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

// ==========================================
// 4. MODULE FILE I/O (4 FILE)
// ==========================================
void loadData() {
    clearAllData();
    ifstream fClass("classes.txt");
    if (fClass.is_open()) {
        ClassInfo c;
        while (getline(fClass, c.classID)) {
            getline(fClass, c.className); getline(fClass, c.homeroomTeacher);
            getline(fClass, c.teacherID); getline(fClass, c.teacherEmail); addClass(c);
        }
        fClass.close();
    }
    ifstream fStudent("students.txt");
    if (fStudent.is_open()) {
        Student st;
        while (fStudent >> st.id) {
            fStudent.ignore(10000, '\n');
            getline(fStudent, st.name); trim(st.name);
            getline(fStudent, st.classID); trim(st.classID);
            getline(fStudent, st.schoolName); trim(st.schoolName);
            getline(fStudent, st.dob); trim(st.dob); addStudentNode(st);
        }
        fStudent.close();
    }
    ifstream fCourse("courses.txt");
    if (fCourse.is_open()) {
        Course c;
        while (fCourse >> c.courseID) {
            fCourse.ignore(10000, '\n'); getline(fCourse, c.courseName); trim(c.courseName);
            fCourse >> c.credits; fCourse.ignore(10000, '\n'); addCourse(c);
        }
        fCourse.close();
    }
    ifstream fGrade("grades.txt");
    if (fGrade.is_open()) {
        Grade g; while (fGrade >> g.studentID >> g.courseID >> g.score10) addOrUpdateGrade(g);
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
        fGrade << t->data.studentID << "\n" << t->data.courseID << "\n" << t->data.score10 << "\n";
    fGrade.close();
}

// ==========================================
// 5. NGHIỆP VỤ CRUD DANH MỤC
// ==========================================
void addStudent() {
    cout << "\n--- THEM SINH VIEN ---\n";
    Student st;
    while (true) {
        cout << "Nhap MSSV: "; cin >> st.id;
        if (!isAllDigits(st.id)) { cout << "Loi: MSSV chi chua so!\n"; continue; }
        bool dup = false;
        for (StudentNode* t = studentHead; t; t = t->next) if (t->data.id == st.id) dup = true;
        if (dup) cout << "Loi: MSSV da ton tai!\n"; else break;
    }
    cin.ignore(); cout << "Nhap Ho va Ten: "; getline(cin, st.name); trim(st.name);
    cout << "Nhap Ma lop (VD: IT1-01): "; getline(cin, st.classID); trim(st.classID);
    
    bool cExists = false;
    for (ClassNode* t = classHead; t; t = t->next) if (t->data.classID == st.classID) cExists = true;
    if (!cExists) {
        ClassInfo nc; nc.classID = st.classID;
        cout << "=> Lop chua co. Nhap Ten lop: "; getline(cin, nc.className); trim(nc.className);
        cout << "   Nhap Ten GVCN: "; getline(cin, nc.homeroomTeacher); trim(nc.homeroomTeacher);
        cout << "   Nhap Ma GVCN: "; getline(cin, nc.teacherID); trim(nc.teacherID);
        cout << "   Nhap Email GVCN: "; getline(cin, nc.teacherEmail); trim(nc.teacherEmail);
        addClass(nc);
    }
    cout << "Nhap Truong truc thuoc: "; getline(cin, st.schoolName); trim(st.schoolName);
    cout << "Nhap Ngay sinh (DD/MM/YYYY): "; getline(cin, st.dob); trim(st.dob);
    addStudentNode(st); cout << "=> Them sinh vien thanh cong!\n";
}

void updateStudent() {
    string id; cout << "Nhap MSSV can sua: "; cin >> id; cin.ignore(10000, '\n');
    for (StudentNode* s = studentHead; s; s = s->next) {
        if (s->data.id == id) {
            cout << "Nhap Ho Ten moi: "; getline(cin, s->data.name); trim(s->data.name);
            cout << "Nhap Ma lop moi: "; getline(cin, s->data.classID); trim(s->data.classID);
            cout << "Nhap Ngay sinh: "; getline(cin, s->data.dob); trim(s->data.dob);
            cout << "=> Cap nhat ho so xong!\n"; return;
        }
    }
    cout << "Loi: Khong tim thay sinh vien!\n";
}

void deleteStudent() {
    string id; cout << "Nhap MSSV can xoa: "; cin >> id;
    if (!studentHead) return;
    
    if (studentHead->data.id == id) {
        StudentNode* temp = studentHead; studentHead = studentHead->next; delete temp;
        cout << "=> Da xoa sinh vien!\n"; return;
    }
    for (StudentNode* curr = studentHead; curr->next; curr = curr->next) {
        if (curr->next->data.id == id) {
            StudentNode* temp = curr->next; curr->next = curr->next->next; delete temp;
            cout << "=> Da xoa sinh vien!\n"; return;
        }
    }
    cout << "Khong tim thay!\n";
}

void updateClassInfo() {
    string id; cout << "Nhap Ma lop can sua: "; cin >> id; cin.ignore(10000, '\n');
    for (ClassNode* c = classHead; c; c = c->next) {
        if (c->data.classID == id) {
            cout << "Nhap Ten lop moi: "; getline(cin, c->data.className); trim(c->data.className);
            cout << "Nhap Ten GVCN: "; getline(cin, c->data.homeroomTeacher); trim(c->data.homeroomTeacher);
            cout << "=> Sua lop thanh cong!\n"; return;
        }
    }
    cout << "Loi: Khong tim thay Lop!\n";
}

void inputCourse() {
    Course c; cout << "\n--- THEM MON HOC ---\nNhap Ma mon: "; cin >> c.courseID;
    if (getCourse(c.courseID)) { cout << "Loi: Mon nay da co!\n"; return; }
    cin.ignore(); cout << "Nhap Ten mon: "; getline(cin, c.courseName); trim(c.courseName);
    cout << "Nhap So tin chi: "; cin >> c.credits; addCourse(c); cout << "=> Da them mon hoc!\n";
}

void inputGrade() {
    Grade g; cout << "\n--- NHAP DIEM ---\nNhap MSSV: "; cin >> g.studentID;
    bool found = false; for(StudentNode* t=studentHead; t; t=t->next) if(t->data.id==g.studentID) found=true;
    if (!found) { cout << "Loi: Khong co sinh vien nay!\n"; return; }
    cout << "Nhap Ma mon hoc: "; cin >> g.courseID;
    if (!getCourse(g.courseID)) { cout << "Loi: Mon hoc chua ton tai!\n"; return; }
    cout << "Nhap Diem he 10: "; cin >> g.score10;
    addOrUpdateGrade(g); cout << "=> Da cap nhat diem!\n";
}

// ==========================================
// 6. MODULE BÁO CÁO & XỬ LÝ
// ==========================================
void displayStudents() {
    if (!studentHead) { cout << "\nDanh sach trong!\n"; return; }
    cout << "\n" << string(110, '-') << "\n";
    cout << left << setw(15) << "MSSV" << setw(25) << "Ho va Ten" << setw(15) << "Ma Lop" 
         << setw(15) << "Ngay Sinh" << setw(10) << "GPA 10" << setw(10) << "GPA 4" << "Xep loai\n";
    cout << string(110, '-') << "\n";
    
    for (StudentNode* s = studentHead; s; s = s->next) {
        double gpa10, gpa4; int tc; calculateGPA(s->data.id, gpa10, gpa4, tc);
        cout << left << setw(15) << s->data.id << setw(25) << s->data.name << setw(15) << s->data.classID 
             << setw(15) << s->data.dob << setw(10) << fixed << setprecision(2) << gpa10 
             << setw(10) << gpa4 << classifyGPA(gpa4) << "\n";
    }
    cout << string(110, '-') << "\n";
}

void findAndPrintTranscript() {
    string kw; cout << "Nhap Ten hoac MSSV can tim: "; cin.ignore(); getline(cin, kw); trim(kw);
    bool found = false;
    for (StudentNode* s = studentHead; s; s = s->next) {
        if (containsSubstring(s->data.name, kw) || containsSubstring(s->data.id, kw)) {
            ClassInfo ci = getClassDetails(s->data.classID);
            double gpa10, gpa4; int tc; calculateGPA(s->data.id, gpa10, gpa4, tc);
            
            cout << "\n" << string(60, '=') << "\n";
            cout << "HO SO SINH VIEN: " << s->data.id << " - " << s->data.name << "\n";
            cout << "Email: " << generateEmail(s->data.name, s->data.id) << "\n";
            cout << "Lop: " << ci.className << " (GVCN: " << ci.homeroomTeacher << ")\n";
            cout << string(60, '-') << "\n";
            cout << left << setw(15) << "Ma Mon" << setw(25) << "Ten Mon" << setw(10) << "TC" << "Diem\n";
            cout << string(60, '-') << "\n";
            
            for (GradeNode* g = gradeHead; g; g = g->next) {
                if (g->data.studentID == s->data.id) {
                    Course* c = getCourse(g->data.courseID);
                    cout << left << setw(15) << c->courseID << setw(25) << c->courseName 
                         << setw(10) << c->credits << fixed << setprecision(1) << g->data.score10 << "\n";
                }
            }
            cout << string(60, '-') << "\n";
            cout << "Tong TC: " << tc << " | GPA 10: " << fixed << setprecision(2) << gpa10 
                 << " | GPA 4: " << gpa4 << " (" << classifyGPA(gpa4) << ")\n";
            cout << string(60, '=') << "\n";
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
            double gpa10, gpa4; int tc; calculateGPA(s->data.id, gpa10, gpa4, tc);
            cout << left << setw(15) << s->data.id << setw(25) << s->data.name << setw(10) << tc 
                 << setw(10) << fixed << setprecision(2) << gpa10 << setw(10) << gpa4 << classifyGPA(gpa4) << "\n";
            found = true;
        }
    }
    if (!found) cout << "Khong co sinh vien lop nay.\n";
    cout << string(80, '-') << "\n";
}

void sortByGPA() {
    if (!studentHead) return;
    bool swapped; StudentNode *p1, *lptr = nullptr;
    do {
        swapped = false; p1 = studentHead;
        while (p1->next != lptr) {
            double g10_1, g4_1, g10_2, g4_2; int tc;
            calculateGPA(p1->data.id, g10_1, g4_1, tc);
            calculateGPA(p1->next->data.id, g10_2, g4_2, tc);
            
            if (g4_1 < g4_2) { // Sap xep Giam dan
                Student temp = p1->data; p1->data = p1->next->data; p1->next->data = temp; swapped = true;
            }
            p1 = p1->next;
        }
        lptr = p1;
    } while (swapped);
    cout << "=> Da sap xep theo GPA! Xem lai o Muc 2.\n";
}

void sortByName() {
    if (!studentHead) return;
    bool swapped; StudentNode *p1, *lptr = nullptr;
    do {
        swapped = false; p1 = studentHead;
        while (p1->next != lptr) {
            if (p1->data.name > p1->next->data.name) { 
                Student temp = p1->data; p1->data = p1->next->data; p1->next->data = temp; swapped = true;
            }
            p1 = p1->next;
        }
        lptr = p1;
    } while (swapped);
    cout << "=> Da sap xep theo Ten! Xem lai o Muc 2.\n";
}

void statistics() {
    int xs = 0, gioi = 0, kha = 0, tb = 0, yeu = 0, cd = 0, total = 0;
    for (StudentNode* s = studentHead; s; s = s->next) {
        double gpa10, gpa4; int tc; calculateGPA(s->data.id, gpa10, gpa4, tc);
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

// ==========================================
// 7. MENU & MAIN
// ==========================================
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