#include "string"
#include "vector"

using namespace std;

namespace ext {

    class Date {

    private:

        static vector<short> daysInMonth;

        static bool checkDateValidity(const int& day, const int& month, const int& year);

        int day, month, year;

        string date;

    public:

        Date();

        Date(const string& s);

        Date(const int& day, const int& month, const int& year);

        string getDate()const;

        int getDay()const;

        int getMonth()const;

        int getYear()const;

        void setDay(const int& day);

        void setMonth(const int& month);

        void setYear(const int& year);

        void setDate(const string& s);
    };

}
