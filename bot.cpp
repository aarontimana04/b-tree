#include <iostream>

template<typename T>
struct Node {
    T data;
    Node *prev, *next;
};

template<typename T>
class DoubleLinkedList {
    Node<T> *head = nullptr, *tail = nullptr;
public:

    Node<T> *get_head() { return head; }

    Node<T> *get_tail() { return tail; }

    T front() const {
        if (head == nullptr) {
            throw std::out_of_range("La lista esta vacia");
        } else {
            return head->data;
        }
    }

    T back() const {
        if (tail == nullptr) {
            throw std::out_of_range("La lista esta vacia");
        } else {
            return tail->data;
        }
    }

    void push_back(int value) {
        auto *nodo = new Node<T>;
        nodo->data = value;
        if (head == nullptr) {
            head = nodo;
            head->next = nullptr;
            head->prev = nullptr;
            tail = head;
        } else {
            nodo->next = nullptr;
            nodo->prev = tail;
            tail->next = nodo;
            tail = nodo;
        }
    }

    void push_front(int value) {
        auto *nodo = new Node<T>;
        nodo->data = value;
        if (head == nullptr) {
            head = nodo;
            head->next = nullptr;
            head->prev = nullptr;
            tail = head;
        } else {
            nodo->prev = nullptr;
            nodo->next = head;
            head->prev = nodo;
            head = nodo;
        }
    }

    T pop_front() {
        if (head != nullptr) {
            auto temp = head;
            head = head->next;

            if (head == nullptr) {
                delete temp;
                return T{};
            }

            head->prev = nullptr;
            T value = temp->data;
            delete temp;
            return value;
        }
        return T{};
    }

    T pop_back() {
        if (tail != nullptr) {
            auto temp = tail;
            tail = tail->prev;

            if (tail == nullptr) {
                delete temp;
                return T{};
            }

            tail->next = nullptr;
            T value = temp->data;
            delete temp;
            return value;
        }
        return T{};
    }

    void insert(T value, int pos) {
        if (pos < 0 || head == nullptr) {
            throw std::out_of_range("Posicion no permitida");
        }

        if (head == tail) {
            if (pos > 0) {
                throw std::out_of_range("Posicion no permitida");
            }
        }

        if (pos == 0) {
            this->push_front(value);
            return;
        }


        auto *nodo = new Node<T>;
        nodo->data = value;
        auto temp = head;
        for (int i = 0; i < pos - 1; i++) {
            if (temp->next == nullptr || temp->next == tail) throw std::out_of_range("Posicion no permitida");
            temp = temp->next;
        }

        nodo->prev = temp;
        nodo->next = temp->next;

        temp->next->prev = nodo;
        temp->next = nodo;

    }


    void remove(int pos) {
        if (head == nullptr) {
            throw std::out_of_range("La lista está vacía.");
        }

        if (pos < 0) {
            throw std::out_of_range("La posición es inválida.");
        }

        if (head == tail) {
            if (pos == 0) {
                delete head;
                head = nullptr;
                tail = nullptr;
            } else {
                throw std::out_of_range("La posición es inválida.");
            }
            return;
        }

        if (pos == 0) {
            Node<T> *temp = head;
            head = head->next;
            head->prev = nullptr;
            delete temp;
            return;
        }

        Node<T> *temp = head;
        int i = 0;
        while (i < pos) {
            temp = temp->next;
            i++;
            if (temp == nullptr) {
                throw std::out_of_range("La posición es inválida.");
            }
        }

        if (temp == tail) {
            tail = tail->prev;
            tail->next = head;
            head->prev = tail;
        } else {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
        }

        delete temp;
    }


    T &operator[](int pos) {
        if (pos < 0 || head == nullptr) throw std::out_of_range("Posicion no permitida");

        if (head == tail) {
            if (pos == 0) return head->data;
            else throw std::out_of_range("Posicion no permitida");
        }

        if (pos == 0) return head->data;

        auto temp = head;
        for (int i = 0; i < pos; i++) {
            temp = temp->next;
            if (temp == nullptr) throw std::out_of_range("Posicion no permitida");
        }

        int value = temp->data;
        temp = nullptr;
        delete temp;

        return value;
    }

    T operator[](int pos) const {
        if (pos < 0 || head == nullptr) throw std::out_of_range("Posicion no permitida");

        if (head == tail) {
            if (pos == 0) return head->data;
            else throw std::out_of_range("Posicion no permitida");
        }

        if (pos == 0) return head->data;

        auto temp = head;
        for (int i = 0; i < pos; i++) {
            temp = temp->next;
            if (temp == nullptr) throw std::out_of_range("Posicion no permitida");
        }

        int value = temp->data;
        temp = nullptr;
        delete temp;

        return value;
    }

    bool empty() const {
        return ((head == nullptr) && (tail == nullptr));
    }

    int size() const {
        if (head == nullptr) return 0;
        else if (head == tail) return 1;

        int i = 0;
        auto temp = head;
        while (temp != nullptr) {
            temp = temp->next;
            i++;
        }

        return i;
    }

    void clear() {
        while (head != nullptr) {
            this->pop_back();
        }
    }

    void reverse() { //Va a invertir los valores de la lista
        if (head == nullptr) return;

        auto first = head, last = tail;

        while (first != last && first->prev != last) {
            std::swap(first->data, last->data);
            first = first->next;
            last = last->prev;
        }
    }

    void print() {
        Node<T> *temp = head;
        while (temp != nullptr) {
            std::cout << temp->data << ' ';
            temp = temp->next;
        }
    }

};

template<typename T>
struct BPlusNode {
    bool isLeaf;
    DoubleLinkedList<T> keys;               // Lista de claves
    DoubleLinkedList<BPlusNode<T>*> children; // Lista de hijos
    BPlusNode* next;                        // Puntero al siguiente nodo hoja

    BPlusNode(bool leaf = false) : isLeaf(leaf), next(nullptr) {}
};

template<typename T>
class BPlusTree {
private:
    BPlusNode<T>* root;
    int maxKeys;

public:
    BPlusTree(int maxKeys) : root(new BPlusNode<T>(true)), maxKeys(maxKeys) {}

    void insert(const T& value) {
        BPlusNode<T>* leaf = findLeaf(value);
        insertInLeaf(leaf, value);
        if (leaf->keys.size() > maxKeys) {
            splitLeaf(leaf);
        }
    }

    void printTree() const {
        printNode(root);
    }

private:
    BPlusNode<T>* findLeaf(const T& value) const {
        BPlusNode<T>* current = root;
        while (!current->isLeaf) {
            int i = 0;
            for (Node<T>* it = current->keys.get_head(); it != nullptr; it = it->next, ++i) {
                if (value < it->data) {
                    current = current->children[i];
                    break;
                }
                if (it->next == nullptr) current = current->children.back();
            }
        }
        return current;
    }

    void insertInLeaf(BPlusNode<T>* leaf, const T& value) {
        if (leaf->keys.empty() || value >= leaf->keys.back()) {
            leaf->keys.push_back(value);
        } else {
            int pos = 0;
            for (Node<T>* it = leaf->keys.get_head(); it != nullptr; it = it->next, ++pos) {
                if (value < it->data) {
                    leaf->keys.insert(value, pos);
                    break;
                }
            }
        }
    }

    void splitLeaf(BPlusNode<T>* leaf) {
        BPlusNode<T>* newLeaf = new BPlusNode<T>(true);
        int splitPos = (leaf->keys.size() + 1) / 2;

        // Mover elementos a la nueva hoja
        for (int i = splitPos; i < leaf->keys.size(); i++) {
            newLeaf->keys.push_back(leaf->keys.pop_back());
        }

        // Conectar las hojas
        newLeaf->next = leaf->next;
        leaf->next = newLeaf;

        // Aquí debes insertar la clave media en el nodo padre
        insertInParent(leaf, newLeaf->keys.front(), newLeaf);
    }

    void insertInParent(BPlusNode<T>* left, const T& key, BPlusNode<T>* right) {
        if (left == root) {
            BPlusNode<T>* newRoot = new BPlusNode<T>();
            newRoot->keys.push_back(key);
            newRoot->children.push_back(left);
            newRoot->children.push_back(right);
            root = newRoot;
            return;
        }

        // Encuentra el padre de left
        BPlusNode<T>* parent = findParent(root, left);

        // Inserta la clave y el puntero right en el padre
        int pos = 0;
        for (Node<T>* it = parent->keys.get_head(); it != nullptr; it = it->next, ++pos) {
            if (key < it->data) {
                parent->keys.insert(key, pos);
                parent->children.insert(right, pos + 1);
                break;
            }
            if (it->next == nullptr) { // Insertar al final si es mayor que todas las claves
                parent->keys.push_back(key);
                parent->children.push_back(right);
                break;
            }
        }

        // Si el padre se llena, dividimos el nodo
        if (parent->keys.size() > maxKeys) {
            splitInternalNode(parent);
        }
    }

    BPlusNode<T>* findParent(BPlusNode<T>* current, BPlusNode<T>* child) {
        if (current == nullptr || current->isLeaf) return nullptr;

        // Recorre los hijos del nodo actual
        for (Node<BPlusNode<T>> it = current->children.get_head(); it != nullptr; it = it->next) {
            if (it->data == child) {
                return current;
            }

            BPlusNode<T>* potentialParent = findParent(it->data, child);
            if (potentialParent != nullptr) {
                return potentialParent;
            }
        }
        return nullptr;
    }

    void splitInternalNode(BPlusNode<T>* node) {
        BPlusNode<T>* newInternal = new BPlusNode<T>();
        int splitPos = (node->keys.size() + 1) / 2;

        // Mueve las claves y punteros de la mitad superior al nuevo nodo
        for (int i = splitPos + 1; i < node->keys.size();) {
            newInternal->keys.push_back(node->keys.pop_back());
        }
        for (int i = splitPos + 1; i <= node->children.size();) {
            newInternal->children.push_back(node->children.pop_back());
        }

        // La clave media se sube al nodo padre
        T middleKey = node->keys.pop_back();
        insertInParent(node, middleKey, newInternal);
    }


    void printNode(BPlusNode<T>* node) const {
        if (node == nullptr) return;
        if (node->isLeaf) {
            node->keys.print();
            std::cout << " -> ";
            if (node->next) printNode(node->next);
        } else {
            for (Node<T>* it = node->children.get_head(); it != nullptr; it = it->next) {
                printNode(it->data);
            }
        }
    }
};
