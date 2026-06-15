void loadFromFile() {
    ifstream file("students.txt");
    
    // Nếu không tìm thấy file, in ra chữ to đùng để báo động
    if (!file.is_open()) {
        cout << "\n[DEBUG] LOI: KHONG TIM THAY FILE 'students.txt' o thu muc hien tai!\n";
        return;
    }

    // Nếu tìm thấy, báo cho chúng ta biết
    cout << "\n[DEBUG] DA TIM THAY FILE! Dang doc du lieu...\n";

    studentList.clear();
    Student st;
    while (file >> st.id) {
        file.ignore();
        getline(file, st.name);
        getline(file, st.className);
        getline(file, st.schoolName);
        getline(file, st.dob);
        file >> st.gpa;
        file.ignore(); 
    }
    file.close();
}