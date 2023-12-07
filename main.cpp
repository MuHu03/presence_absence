#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class Student {
private:
    string firstName;
    string lastName;
    string studentID;
    vector<int> attendanceList;
public:
    Student(string fname,string lname,string sid,vector<int> atl){
        firstName = fname;
        lastName = lname;
        studentID = sid;
        attendanceList = atl;
    }
    string getFirstName() {
        return firstName;
    }

    string getLastName() {
        return lastName;
    }

    string getStudentId() {
        return studentID;
    }

    void addCheck(int val) {
        attendanceList.push_back(val);
    }

    vector<int> getAttendantList() {
        return attendanceList;
    }

    int getNofPresents() {
        int ans = 0;
        for(int i : attendanceList) {
            ans += i;
        }
        return ans;
    }
};

class Class {
private:
    string name;
    int sessionsCount;
    vector<Student*> students;
public:
    Class(string cfg_path) {
        ifstream fin;
        fin.open(cfg_path);

        fin >> name;
        fin >> sessionsCount;

        students = vector<Student*>();

        string line;
        getline(fin, line);
        while(getline(fin, line)) {
            stringstream ss(line);

            int n;
            ss >> n;

            string stFname;
            string stLname;
            string stId;
            ss >> stFname;
            ss >> stLname;
            ss >> stId;

            vector<int> atl;

            int atNum;
            while(ss >> atNum) {
                atl.push_back(atNum);
            }

            students.push_back(new Student(stFname,stLname,stId,atl));
        }

        fin.close();
    }

    vector<Student*> getStudents() {
        return students;
    }

    void increaseSessionsByOne() {
        sessionsCount++;
    }

    string getName() {
        return this->name;
    }

    int getSessionsCount() {
        return sessionsCount;
    }
};

class ClassAttendanceSystem {
private:
    Class *cls;
public:
    void load() {
        string cfg_path;
        printf("Enter the config file: \n");
        cin >> cfg_path;
        printf("Loading...\n");
        cls = new Class(cfg_path);
        printf("Loaded successfully.\n");
    }

    void add_session() {
        printf("Adding a new session:\n");
        printf("Check each student ([p]resent/[a]bsent)\n");
        char ans;
        for(Student *st : cls->getStudents()) {
            printf("%s %s p/a? ", st->getFirstName().c_str(), st->getLastName().c_str());
            cin >> ans;

            switch (ans) {
                case 'p': case 'P':
                    st->addCheck(1);
                    break;
                case 'a': case 'A':
                    st->addCheck(0);
                    break;
                default:
                    printf("Wrong! p or a??? ");
                    break;
            }
        }
        cls->increaseSessionsByOne();
        printf("Session added successfully!\n");
    }

    void check() {
        printf("\t\t==[ Attendance Table ]==\n");
        printf("| N |\tFull Name\t| Attendance List\n");
        int n = 1;
        for(Student *st : cls->getStudents()) {
            printf("| %d | ", n++);
            printf("%s\t%s\t| ", st->getFirstName().c_str(), st->getLastName().c_str());
            for(int v : st->getAttendantList()) {
                printf("%d", v);
            }
            printf(" |\n");
        }
    }

    void student_information() {
        printf("\t\t==[ Attendance Table ]==\n");
        printf("| N |\tFull Name\t| Student ID | Number of Presents\n");
        int n = 1;
        for(Student *st : cls->getStudents()) {
            printf("| %d | ", n++);
            printf("%s\t%s\t| ", st->getFirstName().c_str(), st->getLastName().c_str());
            printf("%s", st->getStudentId().c_str());
            printf("   |        %d", st->getNofPresents());
            printf("\n");
        }
    }

    void save() {
        string cfg_path;
        printf("Enter the config file to save: ");
        cin >> cfg_path;
        ofstream fout;
        fout.open(cfg_path);

        fout << cls->getName() << " " << cls->getSessionsCount() << endl;

        int n = 1;
        for (Student *st : cls->getStudents()) {
            fout << n++ << " "
                 << st->getFirstName() << " "
                 << st->getLastName() << " "
                 << st->getStudentId();

            for (int v : st->getAttendantList()) {
                fout << v << " ";
            }
            fout << "\n";
        }
        fout.close();
    }
};

bool run(ClassAttendanceSystem *system) {
    printf("--> Welcome to Class Attendance Program <--\n");
    printf("What do you want to do?\n");
    printf("\n");
    printf("1. Load Class Information\n");
    printf("2. Add a new Session\n");
    printf("3. Check the Attendance\n");
    printf("4. Show Student Information\n");
    printf("5. Save and Exit\n");
    printf("\n");
    printf("\n");
    printf("Enter the number: ");
    int n;
    cin >> n;

    switch(n) {
        case 1:
            system -> load();
            break;
        case 2:
            system -> add_session();
            break;
        case 3:
            system -> check();
            break;
        case 4:
            system -> student_information();
            break;
        case 5:
            system -> save();
            return false;
        default:
            printf("Wrong number! Try again.\n");
            break;
    }

    return true;
}

int main() {
    ClassAttendanceSystem *system;
    system = new ClassAttendanceSystem();
    while(run(system)) {}
}