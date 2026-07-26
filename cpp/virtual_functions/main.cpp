#include <vector>
#include <iostream>
#include <sstream>
#include <array>
#include <memory>
#include <numeric>

class Person {
  public:
    Person() = default;
    virtual ~Person() = default;
    virtual void getdata() = 0;
    virtual void putdata() const = 0;

  protected:
    std::string m_name;
    int m_age{};
    int m_cur_id{};
};

class Professor : public Person {
  public:
    Professor() = default;
    void getdata() override {
        std::cin >> std::ws;
        std::getline(std::cin, m_name, ' ');
        std::cin >> m_age;
        std::cin >> m_publications;
        m_cur_id = next_available_id;
        next_available_id++;
    }
    void putdata() const override {
        std::cout << m_name << " " << m_age << " " << m_publications << " " << m_cur_id << std::endl;
    }
    static int next_available_id;

  private:
    int m_publications = -1;
};

class Student : public Person {
  public:
    Student() = default;
    void getdata() override {
        std::cin >> std::ws;
        std::getline(std::cin, m_name, ' ');
        std::cin >> m_age;
        for (int i_mark = 0; i_mark < 6; ++i_mark) {
            std::cin >> m_marks[i_mark];
        }
        m_cur_id = next_available_id;
        next_available_id++;
    }
    void putdata() const override {
        std::cout << m_name << " " << m_age << " " << std::accumulate(m_marks.begin(), m_marks.end(), 0)
                  << " " << m_cur_id << std::endl;
    }
    static int next_available_id;

  private:
    std::array< int, 6 > m_marks{};
};

// initialize static members
int Professor::next_available_id = 1;
int Student::next_available_id = 1;

int main() {
    int n, val;
    std::cin >> n; // The number of objects that is going to be created.
    std::vector< std::unique_ptr< Person > > per_vec;

    for (int i = 0; i < n; i++) {

        std::cin >> val;
        if (val == 1) {
            // If val is 1 current object is of type Professor
            per_vec.emplace_back(std::make_unique< Professor >());
        } else {
            per_vec.emplace_back(std::make_unique< Student >()); // Else the current object is of type Student
        }

        per_vec[i]->getdata(); // Get the data from the user.
    }

    for (int i = 0; i < n; i++) {
        per_vec[i]->putdata(); // Print the required output for each object.
    }

    return 0;
}
