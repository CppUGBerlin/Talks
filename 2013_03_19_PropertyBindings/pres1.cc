

/*
    Property bindings in C++

        Olivier Goffart

            1013-03-19
*/














//-------------------------------------//








class bug { bug * foo = [&] { return this; }(); };




/*  Plan

     - Introduction to QML

     - C++11 reminders

     - Property bindings in C++

     - More syntaxetic sugar

     - Bonus: String processing in meta template

*/































#include <string>

/*
   C++11 initialisation
*/

struct X {
    int i = 0;
    std::string str = "hello";


    X() = default;
    X(int i) : i (i) {}
};

















#include <map>
#include <vector>
#include <algorithm>
#include <functional>




/* Lambda  */
void my_sort(int x, std::vector<int> &v) {
    std::map<int, int> map /* = ... */;

    auto L1 = [map] (int x, int y) -> bool
              { return map.at(x) < map.at(y); };

    std::sort(v.begin(), v.end(), L1);










    struct MyL {
        std::map<int, int> map;

        bool operator() (int x, int y) {
            return map.at(x) < map.at(y);
        }
    };

    MyL L2 {map};









    std::sort(v.begin(), v.end(), [=](int x, int y) {
        return map.at(x) < map.at(y);
    });
















    std::function<bool(int, int)> myFunction = L1;













    std::function<int()> foo = [=] { return map.size(); };





#if 0 // C++1y  generic lambda (N3418)
    std::sort(v.begin(), v.end(), [=](x, y) {
        return map[x] < map[y];
    });
#endif

}













using std::function;
template <typename T> function<T(T)> Y(
            const function<function<T(T)>(function<T(T)>)> &F)
{ return [F](T t){ return F(Y<T>(F))(t); }; }











#include <QtCore/QObject>
#include <iostream>

void qt_example() {
    QObject o;
    QObject::connect(&o, &QObject::destroyed, [](){
        std::cout << "object has been destroyed" << std::endl;
    });
}


























/*
 * Property bindings in C++
 */
