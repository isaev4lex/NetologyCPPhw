#include <iostream>
#include <memory>
#include <vector>


namespace first_task {
    class tridiagonal_matrix
    {
    public:
        std::vector<double> m_down;
        std::vector<double> m_upper;
        std::vector<double> m_middle;
        tridiagonal_matrix(
            const std::vector<double>& down,
            const std::vector<double>& upper,
            const std::vector<double>& middle) :
            m_down{ down }, m_upper{ upper }, m_middle{ middle }
        {
        };
        ~tridiagonal_matrix() { std::cout << "destructor called\n"; }

        std::unique_ptr<tridiagonal_matrix> clone() {
            return std::unique_ptr<tridiagonal_matrix>(new tridiagonal_matrix(m_down, m_upper, m_middle));
        }
    };

    int start()
    {
        auto down = std::vector<double>{ -4.0, 5.0 };
        auto upper = std::vector<double>{ 14.0, 8.0 };
        auto middle = std::vector<double>{ 3.0, 1.0, 7.0 };
        auto matrix = std::make_unique<tridiagonal_matrix>(
            down,
            upper,
            middle
        );

        auto matrix_clone = matrix->clone();

        return 0;
    }
}

namespace second_task {
    class node
    {
    public:
        int m_value;
        std::weak_ptr<node> parent;
        node(int value) : m_value{ value } {};
        ~node() { std::cout << "destructor called\n"; }
    };

    int start()
    {
        {
            auto node1 = std::make_shared<node>(1);
            auto node2 = std::make_shared<node>(2);
            node1->parent = node2;
            node2->parent = node1;
        }

        return 0;
    }
}

namespace third_task {
    template <typename T>
    class myUniquePtr {
    private:
        T* ptr;
    public:
        myUniquePtr(T* ptr) : ptr(ptr) {

        }

        T& operator*() {
            return *ptr;
        }

        void operator=(const myUniquePtr&) = delete;

        T* release() {

            T* returnable_ptr = ptr;

            ptr = nullptr;

            return returnable_ptr;
        }

        myUniquePtr(const myUniquePtr&) = delete;

        ~myUniquePtr() {
            delete ptr;
        }
    };

    void start() {
        myUniquePtr<int> myuniq1(new int(324));
        std::cout << *myuniq1 << std::endl;

        int* newptr = myuniq1.release();

        std::cout << *newptr << std::endl;


        //myUniquePtr myuniq2(myuniq1); // НЕ СРАБОТАЕТ

        //myuniq2 = myuniq1; // НЕ СРАБОТАЕТ

        delete newptr;




        myUniquePtr<char> myuniq323(new char('e'));
        std::cout << *myuniq323 << std::endl;

        char* newptr23213 = myuniq323.release();

        std::cout << *newptr23213 << std::endl;

        delete newptr23213;
    }
}


int main(int argc, char** argv) {

    //first_task::start();
    //second_task::start();
    //third_task::start();

	return 0;
}