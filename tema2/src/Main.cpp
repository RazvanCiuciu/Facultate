#include <iostream>
#include <algorithm> // Pentru std::swap


class SmartArray {
private:
    int* data;
    size_t size;

public:

    SmartArray(size_t s) : size(s) {
        std::cout << "Constructor: Aloc memorie pentru " << size << " elemente.\n";
        data = new int[size];

        for (size_t i = 0; i < size; ++i) {
            data[i] = static_cast<int>(i + 1);
        }
    }


    ~SmartArray() {
        std::cout << "Destructor: Eliberez memorie.\n";
        delete[] data;
    }

    SmartArray(const SmartArray& other) : size(other.size) {
        std::cout << "Constructor de Copiere: Copiez " << size << " elemente.\n";
        data = new int[size];

        std::copy(other.data, other.data + other.size, data);
    }

    SmartArray& operator=(SmartArray other) { 
        std::cout << "Operator de Atribuire (Copy-and-Swap):\n";
        
        using std::swap;
        swap(data, other.data);
        swap(size, other.size);
        
        return *this;
    }

    /*
    // Implementarea Tradițională a Operatorului de Atribuire
    // Acoperă explicit Item 10, 11, 12
    SmartArray& operator=(const SmartArray& other) {
        std::cout << "Operator de Atribuire (Tradițional):\n";
        
        if (this == &other) {
            std::cout << "   -> Atribuire la sine, returnez.\n";
            return *this; 
        }

        std::cout << "   -> Eliberez resursa veche.\n";
        delete[] data;


        size = other.size; // Copierea părții 'size'
        std::cout << "   -> Aloc și copiez " << size << " elemente noi.\n";
        data = new int[size]; // Atenție la excepții AICI!
        std::copy(other.data, other.data + other.size, data); // Copierea părții 'data'
        
        return *this;
    }
    */
    
    void print() const {
        std::cout << "Tablou [size=" << size << "]: ";
        for (size_t i = 0; i < size; ++i) {
            std::cout << data[i] << (i == size - 1 ? "" : ", ");
        }
        std::cout << "\n";
    }
};

int main() {
    std::cout << "--- CREARE OBIECTE ---\n";
    SmartArray a(5);
    SmartArray b(10);
    a.print(); 
    b.print();
    
    std::cout << "\n--- TEST ITEM 10: Returnează referință ---\n";
    SmartArray c(3);
    c.print();
    
    std::cout << "a = b (Atribuire normală)\n";
    a = b; 
    a.print(); 
    
    std::cout << "\n--- TEST ITEM 11: Auto-atribuire (a = a) ---\n";
    std::cout << "a = a\n";
    a = a; 
    a.print();
    
    std::cout << "\n--- TEST ITEM 12: Copierea tuturor părților ---\n";
    
    std::cout << "a după a = a:\n";
    a.print(); 
    
    std::cout << "\n--- TERMINARE OBIECTE ---\n";
    return 0;
}
