#include <iostream>
#include <cmath>
using namespace std;

struct Node {
    int coef; // Coefficient
    int exp;  // Exponent
    Node* link;

    Node(int c = 0, int e = 0, Node* l = nullptr) : coef(c), exp(e), link(l) {}
};

class Polynomial {
private:
    Node* head; // Header node for circular linked list

    void clear() {
        if (!head) return;
        Node* current = head->link;
        while (current != head) {
            Node* temp = current;
            current = current->link;
            delete temp;
        }
        delete head;
        head = nullptr;
    }

public:
    Polynomial() {
        head = new Node();
        head->link = head;
    }

    ~Polynomial() {
        clear();
    }

    Polynomial(const Polynomial& a) {
        head = new Node();
        head->link = head;

        Node* current = a.head->link;
        Node* tail = head;
        while (current != a.head) {
            tail->link = new Node(current->coef, current->exp);
            tail = tail->link;
            current = current->link;
        }
        tail->link = head;
    }

    const Polynomial& operator=(const Polynomial& a) {
        if (this != &a) {
            clear();
            head = new Node();
            head->link = head;

            Node* current = a.head->link;
            Node* tail = head;
            while (current != a.head) {
                tail->link = new Node(current->coef, current->exp);
                tail = tail->link;
                current = current->link;
            }
            tail->link = head;
        }
        return *this;
    }

    friend istream& operator>>(istream& is, Polynomial& x) {
        int n;
        is >> n;
        Node* tail = x.head;
        for (int i = 0; i < n; ++i) {
            int coef, exp;
            is >> coef >> exp;
            tail->link = new Node(coef, exp);
            tail = tail->link;
        }
        tail->link = x.head;
        return is;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        Node* current = x.head->link;
        while (current != x.head) {
            os << current->coef << "x^" << current->exp;
            current = current->link;
            if (current != x.head) os << " + ";
        }
        return os;
    }

    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Node* tail = result.head;
        Node* p1 = head->link;
        Node* p2 = b.head->link;

        while (p1 != head && p2 != b.head) {
            if (p1->exp > p2->exp) {
                tail->link = new Node(p1->coef, p1->exp);
                p1 = p1->link;
            }
            else if (p1->exp < p2->exp) {
                tail->link = new Node(p2->coef, p2->exp);
                p2 = p2->link;
            }
            else {
                int newCoef = p1->coef + p2->coef;
                if (newCoef != 0) {
                    tail->link = new Node(newCoef, p1->exp);
                }
                p1 = p1->link;
                p2 = p2->link;
            }
            if (tail->link) tail = tail->link;
        }

        while (p1 != head) {
            tail->link = new Node(p1->coef, p1->exp);
            p1 = p1->link;
            tail = tail->link;
        }

        while (p2 != b.head) {
            tail->link = new Node(p2->coef, p2->exp);
            p2 = p2->link;
            tail = tail->link;
        }

        tail->link = result.head;
        return result;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        Node* tail = result.head;
        Node* p1 = head->link;
        Node* p2 = b.head->link;

        while (p1 != head && p2 != b.head) {
            if (p1->exp > p2->exp) {
                tail->link = new Node(p1->coef, p1->exp);
                p1 = p1->link;
            }
            else if (p1->exp < p2->exp) {
                tail->link = new Node(-p2->coef, p2->exp);
                p2 = p2->link;
            }
            else {
                int newCoef = p1->coef - p2->coef;
                if (newCoef != 0) {
                    tail->link = new Node(newCoef, p1->exp);
                }
                p1 = p1->link;
                p2 = p2->link;
            }
            if (tail->link) tail = tail->link;
        }

        while (p1 != head) {
            tail->link = new Node(p1->coef, p1->exp);
            p1 = p1->link;
            tail = tail->link;
        }

        while (p2 != b.head) {
            tail->link = new Node(-p2->coef, p2->exp);
            p2 = p2->link;
            tail = tail->link;
        }

        tail->link = result.head;
        return result;
    }

    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        Node* p1 = head->link;

        while (p1 != head) {
            Polynomial temp;
            Node* tail = temp.head;
            Node* p2 = b.head->link;

            while (p2 != b.head) {
                tail->link = new Node(p1->coef * p2->coef, p1->exp + p2->exp);
                tail = tail->link;
                p2 = p2->link;
            }

            tail->link = temp.head;
            result = result + temp;
            p1 = p1->link;
        }

        return result;
    }

    float Evaluate(float x) const {
        float result = 0;
        Node* current = head->link;
        while (current != head) {
            result += current->coef * pow(x, current->exp);
            current = current->link;
        }
        return result;
    }
};

int main() {
    Polynomial p1, p2;
    cout << "第一個多項式(format: n coef1 exp1 coef2 exp2 ...): ";
    cin >> p1;
    cout << "第二個多項式(format: n coef1 exp1 coef2 exp2 ...): ";
    cin >> p2;

    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial prod = p1 * p2;

    cout << "p1: " << p1 << endl;
    cout << "p2: " << p2 << endl;
    cout << "和: " << sum << endl;
    cout << "差: " << diff << endl;
    cout << "積: " << prod << endl;

    float x;
    cout << "Enter a value for x to evaluate p1: ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Evaluate(x) << endl;
    cout << "p2(" << x << ") = " << p2.Evaluate(x) << endl;
    return 0;
}
