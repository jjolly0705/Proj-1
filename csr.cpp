// CMSC 341 - Fall 2026 - Project 1
#include "csr.h"
///////////////CSR Class Implementation///////////////
CSR::CSR(){
    this->m_values = 0;
    this->m_col_index = 0;
    this->m_row_index = 0;
    this->m_nonzeros = 0;
    this->m_m = 0;
    this->m_n = 0; 
    this->m_next = 0;
}
CSR::~CSR(){
    clear();
}
CSR::CSR(const CSR & rhs){
    this->m_nonzeros = rhs.m_nonzeros;
    this->m_m = rhs.m_m;
    this->m_n = rhs.m_n;

    this->m_next = nullptr;

    if (rhs.m_values != nullptr && rhs.m_nonzeros > 0) {
        this->m_values = new int[rhs.m_nonzeros];
        for (int i = 0; i < rhs.m_nonzeros; i++) {
            this->m_values[i] = rhs.m_values[i];
        }
    }
    else {
        this->m_values = nullptr;
    }

    if (rhs.m_col_index != nullptr && rhs.m_nonzeros > 0) {
        this->m_col_index = new int[rhs.m_nonzeros];
        for (int i = 0; i < rhs.m_nonzeros; i++) {
            this->m_col_index[i] = rhs.m_col_index[i];
        }
    }
    else {
        this->m_col_index = nullptr;
    }

    if (rhs.m_row_index != nullptr && rhs.m_m > 0) {
        this->m_row_index = new int[rhs.m_m + 1];
        for (int i = 0; i < rhs.m_m + 1; i++) {
            this->m_row_index[i] = rhs.m_row_index[i];
        }
    }
    else {
        this->m_row_index = nullptr;
    }
}
void CSR::clear(){
    delete[] m_values;
    delete[] m_col_index;
    delete[] m_row_index;

    m_values = nullptr;
    m_col_index = nullptr;
    m_row_index = nullptr;

    m_nonzeros = 0;
    m_n = 0;
    m_m = 0;
}
bool CSR::empty() const{
    return (m_m == 0 && m_n == 0 && m_nonzeros == 0 && m_values == nullptr && m_col_index == nullptr && m_row_index == nullptr);
}
void CSR::compress(int m, int n, int array[], int arraySize){
    clear();

    if (m <= 0 || n <= 0) {
        return;
    }
    m_m = m;
    m_n = n;

    m_nonzeros = 0;
    if(array != nullptr && arraySize > 0) {
        for (int i = 0; i < m * n && i < arraySize; i++) {
            if (array[i] != 0) {
                m_nonzeros++;
            }
        }
    }

    m_row_index = new int[m + 1];
    for (int i = 0; i <= m; i++) {
        m_row_index[i] = 0;
    }

    if (m_nonzeros > 0) {
        m_values = new int[m_nonzeros];
        m_col_index = new int[m_nonzeros];
    }
    else {
        m_values = nullptr;
        m_col_index = nullptr;
    }
    
    int currentIndex = 0;
    if(array != nullptr && arraySize > 0 && m_nonzeros > 0) {
        for (int row = 0; row < m; row++) {
            m_row_index[row] = currentIndex;
            for (int column = 0; column < n; column++) {
                int index = row * n + column;

                if (index >= arraySize) {
                    break;
                }
                if (array[index] != 0) {
                    m_values[currentIndex] = array[index];
                    m_col_index[currentIndex] = column;
                    currentIndex++;
                }
            }
        }
    }
    m_row_index[m] = currentIndex;
}
int CSR::getAt(int row, int  col) const{
    if (row < 0 || row >= m_m || col < 0 || col >= m_n) {
        throw std::runtime_error("Exception Error: Object is out of bounds!");
    }
    if(m_row_index == nullptr) {
        return 0;
    }
    int startOfRow = m_row_index[row];
    int endOfRow = m_row_index[row + 1];

    for (int i = startOfRow; i < endOfRow; i++) {
        if (m_col_index[i] == col) {
            return m_values[i];
        }
    }
    return 0;
} 
bool CSR::operator==(const CSR & rhs) const{
    if (this->m_m != rhs.m_m || this->m_n != rhs.m_n || this->m_nonzeros != rhs.m_nonzeros) {
        return false;
    }
    if (this->m_nonzeros == 0) {
        return true;
    }
    for (int i = 0; i < this->m_nonzeros; i++) {
        if (this->m_values[i] != rhs.m_values[i]) {
            return false;
        }
    }
    for (int i = 0; i < this->m_nonzeros; i++) {
        if (this->m_col_index[i] != rhs.m_col_index[i]) {
            return false;
        }
    }
    for (int i = 0; i <= this->m_m; i++) {
        if (this->m_row_index[i] != rhs.m_row_index[i]) {
            return false;
        }
    }
    return true;
}
int CSR::sparseRatio(){
    if (empty()) {
        return 0;
    }

    int totalElements = m_m * m_n;
    int totalZeros = totalElements - m_nonzeros;
    int sparsityRatio = (totalZeros * 100) / totalElements;
    
    return sparsityRatio;
}
void CSR::dump(){
    cout << endl;
    if (!empty()){
        for (int i=0;i<m_nonzeros;i++)
            cout << m_values[i] << " ";
        cout << endl;
        for (int i=0;i<m_nonzeros;i++)
            cout << m_col_index[i] << " ";
        cout << endl;
        for (int i=0;i<m_m+1;i++)
            cout << m_row_index[i] << " ";
    }
    else
        cout << "The object is empty!";
    cout << endl;
}

//////////////CSRList Class Implementation///////////////
CSRList::CSRList(){
    m_head = nullptr;
    m_size = 0;
}
CSRList::CSRList(const CSRList & rhs){
    m_head  = nullptr;
    m_size = 0;

    if (rhs.m_head == nullptr || rhs.m_size == 0 ) {
        return;
    }

    CSR *tail = nullptr;
    CSR *currentRhs = rhs.m_head;

    while (currentRhs != nullptr) {
        CSR *newNode = new CSR(*currentRhs);

        if (m_head == nullptr) {
            m_head = newNode;
        }
        else {
            tail->m_next = newNode;
        }
        tail = newNode;
        currentRhs = currentRhs->m_next;
    }
    m_size = rhs.m_size;
}
CSRList::~CSRList(){
    clear();
}
bool CSRList::empty() const{
    return (m_head == nullptr || m_size == 0);
}
void CSRList::insertAtHead(const CSR & matrix){
    CSR* newNode = new CSR(matrix);
    newNode->m_next = m_head;
    m_head = newNode;
    m_size++;
}
void CSRList::clear(){
    CSR *current = m_head;
    while (current != nullptr) {
        CSR *temp = current;
        current = current->m_next;
        delete temp;
    }
    m_head = nullptr;
    m_size = 0;
}

int CSRList::getAt(int CSRIndex, int row, int col) const{
    if (CSRIndex < 0 || CSRIndex >= m_size || m_head == nullptr) {
        throw std::runtime_error("Exception Error: Object is not in the list!");
    }
    CSR* current = m_head;
    for (int i = 0; i < CSRIndex; i++) {
        if (current == nullptr) {
            throw std::runtime_error("Exception Error: Object is not in the list!");
        }
        current = current->m_next;
    }
    return current->getAt(row, col);
}
bool CSRList::operator== (const CSRList & rhs) const{
    if (this->m_size != rhs.m_size) {
        return false;
    } 
    CSR *current1 = this->m_head;
    CSR *current2 = rhs.m_head;

    while (current1 != nullptr && current2 != nullptr) {
        if (!(*current1 == *current2)) {
            return false;
        }
        current1 = current1->m_next;
        current2 = current2->m_next;
    }
    return (current1 == nullptr && current2 == nullptr);
}
const CSRList& CSRList::operator=(const CSRList & rhs){
    if (this != &rhs) {
        clear();
    
        if (rhs.m_head != nullptr && rhs.m_size > 0) {
            CSR *tail = nullptr;
            CSR *currentRhs = rhs.m_head;
            while (currentRhs != nullptr) {
                CSR *newNode = new CSR(*currentRhs);

                if (m_head == nullptr) {
                m_head = newNode;
                }
                else {
                    tail->m_next = newNode;
                }
                tail = newNode;
                currentRhs = currentRhs->m_next;
            }
            m_size = rhs.m_size;
        }
    }
    return *this;
}
int CSRList::averageSparseRatio(){
    if (m_head == nullptr || m_size <= 0) {
        return 0;
    }
    int sum = 0;
    CSR *current = m_head;

    while (current != nullptr) {
        sum += current->sparseRatio();
        current = current->m_next;
    }
    return sum / m_size;
}
void CSRList::dump(){
    if (!empty()){
        CSR* temp = m_head;
        while (temp != nullptr){
            temp->dump();
            temp = temp->m_next;
        }
    }
    else
        cout << "Error: List is empty!" << endl;
}