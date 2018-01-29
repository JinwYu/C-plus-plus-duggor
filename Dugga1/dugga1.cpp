/*****************************************************************************
* Dugga 1                                                                    *                                        *
* ****************************************************************************/

#include <iostream>
#include <string>

using namespace std;

/****************************************
* Class Date                            *
*****************************************/
class Date
{
public:
    // Constructors.
    Date();
    Date(int theDay, int theMonth, int theYear);
    Date(const Date& D);

    // Overloaded operators.
    Date& operator++();
    Date operator++(int);
    Date& operator--();
    Date operator--(int);
    const Date& operator=(const Date &d);
    bool operator<(const Date& d) const;
    bool operator>(const Date& d) const;
    bool operator<=(const Date& d) const;
    bool operator>=(const Date& d) const;
    bool operator==(const Date& d) const;
    bool operator!=(const Date& d) const;
    int operator-(const Date& d) const;
    Date operator+(int n) const;

    friend ostream& operator<<(ostream &os, const Date& D);

private:
    int day;
    int month;
    int year;

    bool is_leap_year();
    bool is_last_day();
    int get_nr_days_previous_month();
};

// Default constructor.
Date::Date()
{
    day = month = 1;
    year = 2000;
}

// Overloaded constructor taking the date.
// For simplicity, I did not validate that the
// given integers form a valid date because
// the dugga instructions said that it was not mandatory.
Date::Date(int theDay, int theMonth, int theYear)
: day(theDay), month(theMonth), year(theYear)
{ }


// Copy constructor.
// This code might not be needed, because if this copy constructor is
// removed, the compiler will automatically make a shallow copy.
// I've only kept this code because the dugga-instructions said
// that the Date-class should have a copy constructor.
Date::Date(const Date& D)
{
    day = D.day;
    month = D.month;
    year = D.year;
}

// Overloaded output operator to display a date.
ostream& operator<< (ostream& os, const Date& D)
{
    cout << "[" << D.day << "-" << D.month << "-" << D.year << "]= ";
    return os;
}

// Assignment operator.
const Date& Date::operator=(const Date &d)
{
    // If they are identical return the same object.
    if(*this == d) return *this;

    day = d.day;
    month = d.month;
    year = d.year;

    return *this;
}

// Returns true if it is a leap year and false if it is not.
// Taken from http://www.sunshine2k.de/articles/coding/datediffindays/calcdiffofdatesindates.html
// Scroll down to the title "2. Increment-Day Algorithm" under "a" to find the code.
bool Date::is_leap_year()
{
    // Calculate if it's a leap year.
    return (year%4 == 0 && year%100 != 0) || (year%400 == 0);
}

// Checks if it is the last day of the month.
// Taken from: http://www.sunshine2k.de/articles/coding/datediffindays/calcdiffofdatesindates.html
// Scroll down to the title "2. Increment-Day Algorithm" under "b" to find the code.
bool Date::is_last_day()
{
    // Checks every month if it's the last day of the month.
    return ((month == 1 && day == 31) ||
            (is_leap_year() ? (month == 2 && day == 29) : (month == 2 && day == 28)) ||
            (month == 3 && day == 31) ||
            (month == 4 && day == 30) ||
            (month == 5 && day == 31) ||
            (month == 6 && day == 30) ||
            (month == 7 && day == 31) ||
            (month == 8 && day == 31) ||
            (month == 9 && day == 30) ||
            (month == 10 && day == 31) ||
            (month == 11 && day == 30) ||
            (month == 12 && day == 31));
}

// Pre-increment operator.
Date& Date::operator++()
{
    // If it's the last day of the month.
    if (is_last_day())
    {
        // Special case, if it is december.
        if (month == 12)
        {
            month = 1;
            day = 1;
            year++;
        }
        else
        {
            // Increment month and reset the day.
            day = 1;
            month++;
        }
    }
    else
    {
        // Increment day.
        day++;
    }
    return *this;
}

// Pos-increment operator.
Date Date::operator++(int)
{
    Date temp(*this);
    ++(*this);
    return temp;
}

// Pre-increment operator.
Date& Date::operator--()
{
    // If it is the first day of the month.
    if (day == 1)
    {
        // Special case, if it is january.
        if (month == 1)
        {
            month = 12;
            day = 31;
            year--;
        }
        else
        {
            // If it's a month with 31 or 30 days.
            switch(month)
            {
                case 1 :
                    day = 31;
                    break;
                case 2 :
                    day = 31;
                    break;
                case 3 :
                    if(is_leap_year())
                        day = 29;
                    else
                        day = 28;
                    break;
                case 4 :
                    day = 31;
                    break;
                case 5 :
                    day = 30;
                    break;
                case 6 :
                    day = 31;
                    break;
                case 7 :
                    day = 30;
                    break;
                case 8 :
                    day = 31;
                    break;
                case 9 :
                    day = 31;
                    break;
                case 10 :
                    day = 30;
                    break;
                case 11 :
                    day = 31;
                    break;
                case 12 :
                    day = 30;
                    break;
                default :
                    break;
            }
            month--;
        }
    }
    else
    {
        // Decrement the day.
        day--;
    }
    return *this;
}

// Pos-decrement operator.
Date Date::operator--(int)
{
    Date temp(*this);
    --(*this);
    return temp;
}

// Overloaded operator "<", checks if a date is earlier.
bool Date::operator<(const Date& d) const
{
    if(year > d.year)
    {
        return false;
    }
    else if(year < d.year)
    {
        return true;
    }
    else // If year == d.year.
    {

        // Check months.
        if(month > d.month)
        {
            return false;
        }
        else if(month < d.month)
        {
            return true;
        }
        else // If dat == d.day.
        {
            // Check days.
            if(day > d.day)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
}

// Overloaded operator ">".
bool Date::operator>(const Date& d) const
{
    // Check the years.
    if(year > d.year)
    {
        return true;
    }
    else if(year < d.year)
    {
        return false;
    }
    else // If year == d.year.
    {
        // Check the months.
        if(month > d.month)
        {
            return true;
        }
        else if(month < d.month)
        {
            return false;
        }
        else // If dat == d.day.
        {
            // Check the days.
            if(day > d.day)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

// Overloaded operator "==", returns true if the Date objects are identical.
bool Date::operator==(const Date& d) const
{
    return (day == d.day &&
            month == d.month &&
            year == d.year);
}

// Overloaded operator "<=".
bool Date::operator<=(const Date& d) const
{
    Date temp(*this);
    // Check if they are equal.
    if(temp == d)
    {
        return true;
    }
    else    // Check "<".
    {
        if(temp < d)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

// Overloaded operator ">".
bool Date::operator>=(const Date& d) const
{
    Date temp(*this);
    // Check if they are equal.
    if(temp == d)
    {
        return true;
    }
    else    // Check ">".
    {
        if(temp > d)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool Date::operator!=(const Date& d) const
{
    Date temp(*this);
    if(temp == d)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Operator "-" returns the difference in number of days between two dates.
int Date::operator-(const Date& d) const
{
    int nr_days = 0;
    Date temp(*this);

    // If the "this" date is smaller than "d".
    if(*this < d)
    {
        // Keep incrementing until the date is the same.
        while(temp != d)
        {
            ++(temp);
            nr_days++;
        }
    }
    else
    {
        // Keep incrementing until the date is the same.
        while(temp != d)
        {
            --(temp);
            nr_days++;
        }
    }

    return nr_days; // Return the counter.
}

// Operator "+" returns the number of days between two dates.
Date Date::operator+(int n) const
{
    Date temp(*this);

    if(n > 0)
    {
        // Increment the date n-times.
        for(int idx = 1; idx <= n; idx++)
        {
            ++temp;
        }
        // Returns the incremented date.
        return temp;
    }
    else
    {
        // n < 0 is not allowed. If n < 0, then return "this".
        return *this;
    }
}

/****************************************
* Class Boosted_Array                   *
*****************************************/
class Boosted_Array
{
public:
    // Constructors.
    Boosted_Array();
    Boosted_Array(const Date& d, int n_slots);
    Boosted_Array(const Date& d, int n_slots, string text);
    Boosted_Array(const Boosted_Array& B);

    // Destructor.
    ~Boosted_Array();

    // Functions.
    const Date begin() const;
    const Date end() const;
    Boosted_Array& resize_to(const Date &newDate);

    // Overloaded operators.
    bool operator==(const Boosted_Array& b) const;
    string& operator[](const Date& d) const;  // Allows indexing of Date-objects.
    const Boosted_Array operator+(const Boosted_Array& b) const;
    const Boosted_Array& operator=(const Boosted_Array &b);

    friend ostream& operator<<(ostream &os, const Boosted_Array& b); // Friend because it belongs to std, but now it can see private members in the class Boosted_Array.

private:
    string *p_string_array;
    Date start_date;
    int n_slots;
};

// Default constructor.
Boosted_Array::Boosted_Array()
{
    // Dynamically allocates memory for a string array with one slot.
    p_string_array = new string;
    n_slots = 1;
    p_string_array[0] = "";
}

// Overloaded constructor, taking date and size.
Boosted_Array::Boosted_Array(const Date& d, int n_size)
{
    n_slots = n_size;
    p_string_array = new string[n_slots];
    start_date = d;

    // Initialize with empty strings.
    for(int idx = 0; idx < n_slots; idx++)
    {
        p_string_array[idx] = "";
    }
}

// Overloaded constructor, with a text input parameter.
Boosted_Array::Boosted_Array(const Date& d, int n_size, string text)
{
    n_slots = n_size;
    p_string_array = new string[n_slots];
    start_date = d;

    // Initialize the string array with the text parameter.
    for(int idx = 0; idx < n_slots; idx++)
    {
        p_string_array[idx] = text;
    }
}

// Copy constructor.
Boosted_Array::Boosted_Array(const Boosted_Array& b)
{
    start_date = b.start_date;
    n_slots = b.n_slots;
    p_string_array = new string[n_slots];

    // Copies the whole string array.
    for(int idx = 0; idx < n_slots; idx++)
    {
        p_string_array[idx] = b.p_string_array[idx];
    }
}

// Destructor.
Boosted_Array::~Boosted_Array()
{
    // Removes the dynamically allocated memory.
    delete[] p_string_array;
}

// Overloaded operator "==".
bool Boosted_Array::operator==(const Boosted_Array& b) const
{
    bool identical_arrays = false;

    // Check if the string arrays are the same.
    for(int idx = 0; idx < n_slots; idx++)
    {
        if(p_string_array[idx] == b.p_string_array[idx])
        {
            identical_arrays = true;
        }
        else
        {
            return false;
        }
    }

    return (identical_arrays &&
            start_date == b.start_date &&
            n_slots == b.n_slots);
}

// Assignment operator.
const Boosted_Array& Boosted_Array::operator=(const Boosted_Array& b)
{

    start_date = b.start_date;
    n_slots = b.n_slots;

    // Delete the previous pointer to the string array.
    delete[] p_string_array;
    p_string_array = new string[n_slots];

    // Copying every slot.
    for(int idx = 0; idx < n_slots; idx++)
    {
        p_string_array[idx] = b.p_string_array[idx];
    }

    return *this;
}

// Overloaded "<<" to output a boosted array.
ostream& operator<<(ostream &os, const Boosted_Array& B)
{
    // Used to print the dates out.
    Date temp(B.start_date);

    // Print all of the elements.
    for(int idx = 0; idx < B.n_slots; idx++)
    {
        cout << temp << B.p_string_array[idx] << endl;
        ++temp;
    }
    return os;
}

// Returns the date associated with the first slot of the array.
const Date Boosted_Array::begin() const
{
    return start_date;
}

// Returns the date associated with the last slot of the array.
const Date Boosted_Array::end() const
{
    Date temp(start_date);
    // Iterate to the last slot in the array.
    for(int idx = 0; idx < n_slots-1 ; idx++)
    {
        ++temp;
    }
    return temp;
}

// Overload subscript operator.
string& Boosted_Array::operator[](const Date& d) const
{
    int index_counter = 0;

    for(Date temp = begin(); temp < d; ++temp)
    {
        ++index_counter;
    }

    // So the index won't be out of bounds.
    --index_counter;

    return p_string_array[index_counter];
}

// Overload operator "+".
const Boosted_Array Boosted_Array::operator+(const Boosted_Array& b) const
{
    int new_size = 1;

    // Check which of the two arrays has the earliest/latest date or if equal.
    if(begin() <= b.begin())
    {
        // Calculate the new size.
        if(end() <= b.end())
        {
            new_size += b.end() - begin();
        }
        else
        {
            new_size += end() - begin();
        }

        // Create the new larger empty boost array.
        Boosted_Array temp_boost_array(begin(), new_size, "");

        // Add the strings from the first array.
        for(Date temp = begin(); temp < end(); ++temp)
        {
            temp_boost_array[temp] = (*this)[temp];
        }

        // Add the strings from the second array.
        for(Date temp = b.begin(); temp < b.end(); ++temp)
        {
            temp_boost_array[temp] = temp_boost_array[temp] + b[temp];
        }

        return temp_boost_array;
    }
    else    // If b.begin() is smaller than begin().
    {
        if(end() <= b.end())
        {
            new_size += b.end() - b.begin();
        }
        else
        {
            new_size += end() - b.begin();
        }

        // Create the new larger empty boost array.
        Boosted_Array temp_boost_array(b.begin(), new_size, "");

        // Add the strings from the first array.
        for(Date temp = b.begin(); temp < b.end(); ++temp)
        {
            temp_boost_array[temp] = b[temp];
        }

        // Add the strings from the second array.
        for(Date temp = begin(); temp < end(); ++temp)
        {
            temp_boost_array[temp] = temp_boost_array[temp] + (*this)[temp];
        }

        return temp_boost_array;
    }
}

// Resize the boosted array.
Boosted_Array& Boosted_Array::resize_to(const Date& d)
{
    string *temp_old_string_array = new string[n_slots];
    int n_slots_old = n_slots;

    // Dates earlier than the start date should not modify the
    // Boosted_Array-object. Returns the object untouched.
    if(d < start_date)
    {
//        cout << "Debug: The array was not resized."<< endl;
        delete[] temp_old_string_array;
        return *this;
    }

    // Copy all of the strings from "this" pointer to string array.
    for(int idx = 0; idx < n_slots; idx++)
    {
        temp_old_string_array[idx] = p_string_array[idx];
    }

    // Prevent memory leaks.
    delete[] p_string_array;

    // Check if the received date, "d", is less than the last date.
    if(d < end())
    {
        // Subtract the number of slots.
        // Using overloaded operator "-" in the Date class
        // to get the difference between two dates.
        n_slots = n_slots - (d - end());
        p_string_array = new string[n_slots];
    }
    else // If the date, "d", is larger or equal to the ending date.
    {
        // Add to the total number of slots.
        n_slots = n_slots + (d - end());
        p_string_array = new string[n_slots];
    }

    // Add the strings that should be in the resized array.
    for(int idx = 0; idx < n_slots; idx++)
    {
        if(idx < n_slots_old)
        {
            p_string_array[idx] = temp_old_string_array[idx];
        }
        else    // If idx exceeds the old length of the array.
        {
            p_string_array[idx] = "";
        }
    }

    delete[] temp_old_string_array;

    return *this;
}


/***************************************
* MAIN: Please, deliver unmodified     *
****************************************/

int main()
{
    /*************************************
    * TEST PHASE 0                       *
    * Constructor and operator<<         *
    **************************************/
    cout << "\nTEST PHASE 0\n" << endl;

    Boosted_Array AA0(Date(), 2);

    cout << "** AA0 ** " << endl;
    cout << AA0 << endl;

    Boosted_Array AA1(Date(1,6, 2016), 11, "Hej!! ");

    cout << "\n** AA1 ** " << endl;
    cout << AA1 << endl;

    /*************************************
    * TEST PHASE 1                       *
    * Copy constructor                   *
    **************************************/
    cout << "\nTEST PHASE 1\n" << endl;

    const Boosted_Array AA2(AA1);

    cout << "** AA2 ** " << endl;
    cout << AA2 << endl;

    /*************************************
    * TEST PHASE 3                       *
    * Assignment operator                *
    **************************************/
    cout << "\nTEST PHASE 3\n" << endl;

    AA0 = AA2;

    cout << "** AA0 ** " << endl;
    cout << AA0 << endl;

    /*************************************
    * TEST PHASE 4                       *
    * Subscript operator: operator[]     *
    * begin(), end()                     *
    **************************************/
    cout << "\nTEST PHASE 4\n" << endl;

    AA0[Date(1,6,2016)] = "Anna's birthday";
    AA0[Date(2,6,2016)] = "Ole's birthday";
    AA0[Date(3,6,2016)] = "Justin's birthday";
    AA0[Date(4,6,2016)] = "Ruth's birthday";
    AA0[Date(5,6,2016)] = "Rita's birthday";
    AA0[Date(6,6,2016)] = "Shop socks";
    AA0[Date(7,6,2016)] = "Cinema";
    AA0[Date(8,6,2016)] = "Reading";
    AA0[Date(9,6,2016)] = "Grade duggor";
    AA0[Date(10,6,2016)] = "Party";
    AA0[Date(11,6,2016)] = "Party";

    cout << "** AA0 ** " << endl;
    for(Date d = AA0.begin(); d <= AA0.end(); ++d)
    {
        cout << AA0[d] << endl;
    }

    //Test if it works
//    for(Date d = AA0.begin(); d <= AA0.end(); d++)
//    {
//        cout << AA0[d] << endl;
//    }

    cout << endl << AA2[Date(5,6,2016)] << endl;

    /*************************************
    * TEST PHASE 5                       *
    * operator+                          *
    **************************************/
    cout << "\nTEST PHASE 5\n" << endl;

    cout << "** AA2 + AA0 ** " << endl;
    cout << AA2 + AA0 << endl;

    /*************************************
    * TEST PHASE 6                       *
    * resize_to()                        *
    **************************************/
    cout << "\nTEST PHASE 6\n" << endl;

    AA0.resize_to(Date(14,6,2016));

    AA0[Date(12,6,2016)] = "Lecture in TNG033";
    AA0[Date(13,6,2016)] = "Lecture in TND012";
    AA0[Date(14,6,2016)] = "Read exjobb";

    cout << "** AA0 ** " << endl;
    cout << AA0 << endl;

    //Note: first resize_to should not modify AA0
    AA0.resize_to(Date(1,1,2015)).resize_to(Date(15,6,2016));

    AA0[Date(15,6,2016)] = "Holiday";

    cout << "** AA0 ** " << endl;
    cout << AA0 << endl;

    /*************************************
    * TEST PHASE 7                       *
    * operator+                          *
    **************************************/

    Boosted_Array AA4(Date(12,6,2016), 14, "<--");

    AA1 = AA0 + AA4;

    cout << "\n** AA1 ** " << endl;
    cout << AA1 << endl;


    cout << "\nBye ..." << endl;

    return 0;
}
