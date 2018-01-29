/************************************************
* Dugga 2                                       *
* ***********************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <map>

using namespace std;

typedef map<string, vector<string>> string_map;
typedef pair<string, vector<string>> the_pair;


/********************
* Class Exam        *
*********************/
class Exam{
public:

    // Constructors.
    explicit Exam();
    explicit Exam(const Exam& E);

    // Friend in-and outstreams.
    friend ifstream& operator>>(ifstream& in, Exam& E);
    friend ostream& operator<<(ostream &os, const Exam& E);

    // Member functions.
    string get_name() const;
    int get_total_points(const int threshold);

    // Overloaded operators.
    const Exam& operator=(const Exam &E);

    // Destructor.
    ~Exam(){};

private:
    string name_student;        // Student's name.
    vector<int> points_vector;  // The points the student received.
};

// Default constructor.
Exam::Exam()
{
    name_student = "";
    points_vector = {};
}

// Copy constructor.
Exam::Exam(const Exam& E)
{
    name_student = E.name_student;
    points_vector = E.points_vector;
}

// Overloaded input stream operator ">>" to read from a file.
ifstream& operator>>(ifstream& in, Exam& E)
{
    string temp;

    // Read the first line which is the name.
    getline(in, E.name_student);
    // Read the second line which is the points.
    getline(in, temp);

    // Split the string with the points into separate ints.
    // Code taken from http://stackoverflow.com/questions/3421817/splitting-int-from-a-string
    // The code can be found in the first comment, in the third answer to the original poster.
    istringstream iss(temp);
    copy(istream_iterator<int>(iss), istream_iterator<int>(), std::back_inserter(E.points_vector));

    return in;
}

// Overloaded output operator to display an object of the class Exam.
ostream& operator<<(ostream& os, const Exam& E)
{
    cout << E.name_student << endl;
    copy(E.points_vector.begin(), E.points_vector.end(), ostream_iterator<int>(os, " "));
    cout << endl;
    return os;
}

// Getter for the name.
string Exam::get_name() const { return name_student;}

// Sum values above and equal to the threshold.
int Exam::get_total_points(const int threshold)
{
    int sum = 0;

    // Code taken from the first post here http://stackoverflow.com/questions/20028936/is-there-a-accummulate-if
    // Only sums values above the threshold.
    for_each(points_vector.begin(), points_vector.end(), [&](int x){if (x >= threshold) sum += x;});

    return sum;
}

// Assignment operator.
const Exam& Exam::operator=(const Exam &E)
{
//    // If they are identical return the same object.
//    if(*this == E) return *this;

    name_student = E.name_student;
    points_vector = E.points_vector;

    return *this;
}




/*****************************
* Class Exam_Grader          *
******************************/
// Abtract class because it has a virtual function.
// Not possible to create instances of this class in the main.
class Exam_Grader
{
public:
    // Destructor.
    virtual ~Exam_Grader(){};

    // Getter.
    int get_code() const;

    // Outstreams.
    friend ostream& operator<<(ostream &os, const Exam_Grader& E);

    // Functions.
    void write_grading_report(ostream& os) const;
    int number_of_students(const string& grade) const;

    // It should not be possible to create copies of instances of the class.
    Exam_Grader& operator=(const Exam_Grader&) = delete;
    Exam_Grader(const Exam_Grader& e) = delete;

private:
    string name_examiner;           // The examiner's name.
    unsigned exam_code;             // Unique unsigned exam code.

protected:

    // Constructor, placed here to only allow sub-classes to create instances.
    Exam_Grader() = delete;
    explicit Exam_Grader(const string& name, ifstream& in, const int max_points);

    // Non-public member functions. These functions are not necessary,
    // their purpose is to limit the repetition of code.
    // If functions like these are not allowed, then the only step to
    // remove these is just to copy the code in these voids back to the
    // same place as function calls are made (in the sub-classes).
    void assign_the_grade(int& temp_points, const string& name);
    void init_grades_and_map(vector<int>& grade_limits);

    // Variables.
    vector<Exam> exam_vector;       // All the exams.
    bool report_has_generated;      // If the report has been generated.
    string_map grade_report_map;    // The grading report.
    vector<string> grade_types;     // The different grades available.
    vector<string> grade_system;    // Contains the grading system.

    // Virtual function.
    virtual void do_grading_report() = 0;
};

// Constructor that takes the examiner's name and the instream.
Exam_Grader::Exam_Grader(const string& name, ifstream& in, const int max_points)
    : name_examiner(name), grade_system(max_points)
{
    // Unique code for the exam.
    static int unique_code;
    exam_code = ++unique_code;

    // A grading report has not yet been generated.
    report_has_generated = false;

    // Add all of the students' results into a vector. The first loop used in this program.
    while(!in.eof())
    {
        Exam temp;
        in >> temp;
        if(temp.get_name() != "")
        {
            exam_vector.push_back(temp);
        }
    }
}

// Getter to return the exam code.
int Exam_Grader::get_code() const
{
    const int code = exam_code;
    return code;
}

// Overloaded output operator to display an object of the class Exam_Grader.
ostream& operator<<(ostream& os, const Exam_Grader& E)
{
    // Print the general information.
    cout << "Examiner: " << E.name_examiner << endl
         << "Exam's code: " << E.exam_code << endl
         << "** Students' exams **" << endl;

    // Print each students' name and points.
    copy(E.exam_vector.begin(), E.exam_vector.end(), ostream_iterator<Exam>(os));


    return os;
}

// Print the grading report to the given ostream.
void Exam_Grader::write_grading_report(ostream& os) const
{
    // Print the grading report if it has been generated.
    if(report_has_generated)
    {
        os << "Grading Report: " << get_code() << endl;

        // Iterate through the keys in the map.
        for_each(grade_report_map.begin(), grade_report_map.end(), [&] (const the_pair& p)
        {
            int n_students = grade_report_map.at(p.first).size();

            // Display if there were students that received the grade, else skip it.
            if(n_students > 0)
            {
                os << "*** Grade " << p.first << ": " << n_students << " student(s)" << endl;

                // Iterate through the vector<string> at the given key and print the names.
                for_each(grade_report_map.at(p.first).begin(), grade_report_map.at(p.first).end(), [&] (const string& s)
                {
                    os << s << endl;
                });

                os << endl;
            }
        });
    }
    else
    {
        // Print message if a grading report hasn't been generated.
        os << "Grading Report: " << get_code() << " <-- not created!!" << endl;
    }
}

// Return the number of students that received a given grade.
int Exam_Grader::number_of_students(const string& grade) const
{
    bool is_grade = false;
    // Check if "grade" is a grade available.
    for_each(grade_types.begin(), grade_types.end(), [&] (const string& s)
            {
                if(grade == s)
                {
                    is_grade = true;
                }
            });

    // If the report has been generated.
    if(report_has_generated && is_grade)
    {
        // Calculate how many students failed the exam.
        int n_students = grade_report_map.at(grade).size();
        return n_students;
    }
    else
    {
        // Return zero if the grading report has not been generated.
        return 0;
    }
}

// Assigns grades to the students.
// Is used in the function "do_grading_report" in subclasses.
void Exam_Grader::assign_the_grade(int& temp_points, const string& name)
{
    // Retrieve the grade that will be assigned.
    string temp_string;
    if(temp_points > 0)
    {
        // "-1" because the vector "grade_system" starts at the index 0.
        temp_string = grade_system.at(temp_points-1);
    }
    else
    {
        temp_string = grade_system.at(temp_points);
    }

    // Add the name to the map for the grading report.
    grade_report_map[temp_string].push_back(name);

    // Sort the vector with the names for later when it's time to print.
    sort(grade_report_map[temp_string].begin(), grade_report_map[temp_string].end());
}

// Called in the constructors in the sub-classes.
// The function initializes the vector for the grading system
// and the map for the grading report.
void Exam_Grader::init_grades_and_map(vector<int>& grade_limits)
{
    vector<string> empty_vector;
    bool first_execute = true;

    // Initialize the grading system and the map for the grading report.
    // The second loop used in this program.
    for(unsigned int idx = 0; idx < grade_limits.size(); idx++)
    {   if(first_execute)
        {
            // Fill the vector with the grade type within the range of points needed for that grade.
            std::fill (grade_system.begin(), grade_system.begin() + grade_limits[idx] - 1, grade_types[idx]);
            first_execute = false;
        }
        else
        {
            // Fill the vector with the grade type within the range of points needed for that grade.
            std::fill (grade_system.begin() + grade_limits[idx-1] - 1, grade_system.begin() + grade_limits[idx], grade_types[idx]);
        }

        // The different grade types are keys for the map used for the grading report.
        grade_report_map.insert(the_pair(grade_types[idx], empty_vector));
    }
}




/****************************************
* Class Exam_Grader_Level1              *
*****************************************/
class Exam_Grader_Level1 : public Exam_Grader
{
public:
    // Constructor. Optional variable "max_points", which is the max nr of points on the exam.
    // The variable is used to make the "grade_system" in the base-class.
    // It exists to allow scalability to the program to have more grade types.
    explicit Exam_Grader_Level1(const string name, ifstream& in, int bonus = 0, int max_points = 60)
        : Exam_Grader(name, in, max_points), bonus_points(bonus)
        {
            /*
                In the dugga instructions it says: " your solution should scale easily (i.e. require small code changes)".
                I interpreted that as 2 lines of code is a "small code change", so to allow the program to be scalable
                and easily extended, the two code lines below can be edited if one would like to add new grades and
                grade limits. The two vectors need to have the same size, but that is not a problem because there will
                always exist as many different types of grades, as there are grade limits.

                The last slot in the vector "grade_limits" is the maximum points one can receive in the exam.
                All of this applies to the sub-class "Exam_Grader_Level2".

                I did not include initialization of these two vectors in the constructor but that could allow
                more easier scalability to be achieved.
            */
            vector<int> grade_limits = {10, 15, 20, 60};    // Edit to add more grade limits.
            grade_types = {"F", "3", "4", "5"};             // Edit to add more kinds of grades.

            // Print error message if the two vectors differ in size.
            if(grade_limits.size() == grade_types.size())
            {
                // Initializes the vector for the grading system
                // and the map for the grading report.
                init_grades_and_map(grade_limits);
            }
            else
            {
                // Error message.
                cout << "Error: The two vectors have different dimensions. Please input the vectors with the same size." << endl
                     << "The program is likely to crash." << endl;
             }
            // The grading report has not been generated.
            report_has_generated = false;
        }

    // Destructor.
    ~Exam_Grader_Level1(){};

    // Setters and getters.
    void set_bonus(int bonus);
    int get_bonus() const;

    // Override function..
    void do_grading_report() override;

private:
    int bonus_points;
    vector<string> students_names;
};


// Set the nr of bonus points.
void Exam_Grader_Level1::set_bonus(int bonus) { bonus_points = bonus; }

// Getter for the bonus points.
int Exam_Grader_Level1::get_bonus() const { return bonus_points; }

// Construct the grading report.
void Exam_Grader_Level1::do_grading_report()
{
    // Iterate through every Exam-object in the exam_vector
    for_each(exam_vector.begin(), exam_vector.end(), [&] (Exam& e)
    {
        // Get the calculated points.
        int temp_points = e.get_total_points(0) + bonus_points;

        // Function in base-class that assigns the grades.
        assign_the_grade(temp_points, e.get_name());
    });

    // The grading report has been generated.
    report_has_generated = true;
}




/****************************************
* Class Exam_Grader_Level2              *
*****************************************/
class Exam_Grader_Level2 : public Exam_Grader
{
public:
    // Constructor.
    explicit Exam_Grader_Level2(const string name, ifstream& in, int threshold = 2, int max_points = 60)
        : Exam_Grader(name, in, max_points), min_points(threshold)
        {
            // Scalability is explained in the constructor in the sub-class "Exam_Grader_Level2".
            vector<int> grade_limits = {10, 18, 25, 60};
            grade_types = {"F", "3", "4", "5"};

            // Print error message if the two vectors differ in size.
            if(grade_limits.size() == grade_types.size())
            {
                // Initializes the vector for the grading system
                // and the map for the grading report.
                init_grades_and_map(grade_limits);
            }
            else
            {
                // Error message.
                cout << "Error: The two vectors have different dimensions. Please input the vectors with the same size." << endl
                     << "The program is likely to crash." << endl;
            }

            // The grading report has not been generated.
            report_has_generated = false;
        }

    // Destructor.
    ~Exam_Grader_Level2(){};

    // Getter.
    int get_min_points() const;

    // Override function.
    void do_grading_report() override;

private:
    int min_points;
    vector<string> students_names;
};


// Getter for the minimum points.
int Exam_Grader_Level2::get_min_points() const { return min_points; }

// Construct the grading report.
void Exam_Grader_Level2::do_grading_report()
{
    // Iterate through every Exam-object in the exam_vector
    for_each(exam_vector.begin(), exam_vector.end(), [&] (Exam& e)
    {
        // Get the calculated points.
        int temp_points = e.get_total_points(min_points);

        // Function in base-class that assigns the grades.
        assign_the_grade(temp_points, e.get_name());
    });

    // The grading report has been generated.
    report_has_generated = true;
}




/*******************************
* MAIN: deliver unmodified     *
********************************/

int main()
{
    /*************************************
    * TEST PHASE 0                       *
    * Class Exam                         *
    **************************************/
    cout << "\nTEST PHASE 0\n" << endl;

    ifstream in_File("tentor1.txt");

    if (!in_File)
    {
        cout << "Error opening file!!" << endl;
        return 0;
    }

    Exam e;

    in_File >> e; //read first exam's data from file

    cout << e << endl;

    cout << e.get_name() << "\t"
         << "Sum points = " << e.get_total_points(0) << endl;

    in_File.close();


    /*************************************
    * TEST PHASE 1                       *
    * Exam_Grader_Level1                 *
    **************************************/
    cout << "\nTEST PHASE 1\n" << endl;

    in_File.open("tentor1.txt");

    if (!in_File)
    {
        cout << "Error opening file!!" << endl;
        return 0;
    }

    Exam_Grader_Level1 g1("Aida Nordman", in_File, 1);

    cout << g1 << endl;

    in_File.close();

    cout << "Grading with bonus = " << g1.get_bonus()
         << "p ..." << endl;
    cout << "===============================\n\n";

    g1.do_grading_report();
    g1.write_grading_report(cout);


    /*************************************
    * TEST PHASE 3                       *
    * Exam_Grader_Level2                 *
    **************************************/
    cout << "\nTEST PHASE 3\n" << endl;

    in_File.open("tentor2.txt");

    if (!in_File)
    {
        cout << "Error opening file!!" << endl;
        return 0;
    }

    Exam_Grader_Level2 g2("Jens Tor", in_File, 3);

    cout << g2 << endl;

    in_File.close();

    cout << "Grading with min points = " << g2.get_min_points()
         << " ..." << endl;
    cout << "==============================="  << endl;

    g2.do_grading_report();
    g2.write_grading_report(cout);

    /*************************************
    * TEST PHASE 4                       *
    * Exam_Grader_Level2                 *
    **************************************/
    cout << "\nTEST PHASE 4\n" << endl;

    in_File.open("tentor3.txt");

    if (!in_File)
    {
        cout << "Error opening file!!" << endl;
        return 0;
    }

    Exam_Grader_Level2 g3("Nicklas Lundin", in_File);

    cout << g3 << endl;

    in_File.close();

    //Note: no grading report created for g3

    g3.write_grading_report(cout);

    /*************************************
    * TEST PHASE 5                       *
    * Exam_Grader::number_of_students    *
    **************************************/
    cout << "\nTEST PHASE 5\n" << endl;

    cout << "Exam Code " << setw(10) << "Grade 5 "
         << setw(10) << "Grade 4 "
         << setw(10) << "Grade 3 "
         << setw(10) << "Grade F " << endl;

    cout << setw(5) << g1.get_code()
         << setw(10) << g1.number_of_students("5")
         << setw(10) << g1.number_of_students("4")
         << setw(10) << g1.number_of_students("3")
         << setw(10) << g1.number_of_students("F") << endl;

    cout << setw(5) << g2.get_code()
         << setw(10) << g2.number_of_students("5")
         << setw(10) << g2.number_of_students("4")
         << setw(10) << g2.number_of_students("3")
         << setw(10) << g2.number_of_students("F") << endl;

    //Note: no grading report created for g3
    cout << setw(5) << g3.get_code()
         << setw(10) << g3.number_of_students("5")
         << setw(10) << g3.number_of_students("4")
         << setw(10) << g3.number_of_students("3")
         << setw(10) << g3.number_of_students("F") << endl;

    cout << "\nBye ..." << endl;

    return 0;
}


