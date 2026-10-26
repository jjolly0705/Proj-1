// List of Sparse Matrices
#include "csr.h"
#include <iostream>
using namespace std;
 
const char *FAIL_STATEMENT = "*****TEST FAILED: ";
const char *PASS_STATEMENT = "    TEST PASSED: ";
 
class Tester {
    public:
 
    //CSR Class tests
    bool testCSRDefaultConstructor();
 
    bool testCompressNormalNotZero();
    bool testCompressGetAtZero();
 
    bool testCompressSquare();
    bool testCompressMoreRowsThanCols();
    bool testCompressMoreColsThanRows();
 
    bool testCompressAllZeros();
    bool testCompressOneNonzero();
    bool testCompressZeroRow();
    bool testCompressConsecutiveZeroRows();
    bool testCompressNotZeroInFirstSpot();
    bool testCompressNotZeroInLastSpot();
    bool testCompressNegativeNums();
 
    bool testCompressArraySizeLessThanMN();
    bool testCompressArraySizeGreaterThanMN();
    bool testCompressZeroByZeroWithNums();
    bool testCompressPositiveDimensionsNoNums();
    bool testCompressBadDimensions();
 
    bool testCopyConstructorNormal();
    bool testCopyConstructorEmpty();
    bool testCopyConstructorAllZero();
    bool testCopyIndependenceAfterChanges();
    bool testCompressSameObjectTwoTimes();
 
    bool testGetAtNegativeRow();
    bool testGetAtRowTooBig();
    bool testGetAtNegativeCol();
    bool testGetAtColumnTooBig();
 
    bool testCSREqualityNormal();
    bool testCSREqualityDifferentMatrices();
    bool testCSREqualityBothEmpty();
 
    bool testSparseRatioEmpty();
    bool testSparseRatioAllZero();
    bool testSparseRatioNormal();

    //CSR List Class tests
    bool testCSRListDefaultConstructor();
 
    bool testInsertIntoEmptyList();
    bool testInsertCheckOrder();
    bool testInsertBigList();
 
    bool testListCopyConstructorNormal();
    bool testListCopyConstructorWithOneNode();
    bool testListCopyConstructorWithLotsOfNodes();
    bool testListCopyConstructorEmptyList();
 
    bool testAssignNormal();
    bool testSelfAssign();
    bool testAssignNotEmptyToNotEmpty(); 
    bool testAssignNotEmptyToEmpty();
    bool testAssignEmptyToNotEmpty();
 
    bool testListEqualitySameOrder();
    bool testListEqualityDifferentOrder();
    bool testListEqualityDifferentMatrix();
    bool testListEqualityBothEmpty();
    bool testListEqualityEmptyAndNotEmpty();
 
    bool testListGetAtForAllSpots();
    bool testListGetAtNegativeIndex(); 
    bool testListGetAtIndexSameSize();
    bool testListGetAtInvalidRowCol();
    bool testListGetAtEmptyList();
 
    bool testAverageOneMatrix();
    bool testAverageLotsOfMatrices();
 
    bool testClearListWithData();
    bool testClearEmptyListTwoTimes();
    bool testClearThenReuse(); 
};

//CSR Tests Implementation 

//testCSRDefaultConstructor
//Case: creates a CSR object with the default constructor and compress() never called
//Expected: every member variable is initialized, all pointers are null, all ints 0, empty() is true
bool Tester::testCSRDefaultConstructor(){
    CSR matrix;
 
    bool result = (matrix.m_values == nullptr) && (matrix.m_col_index == nullptr) && (matrix.m_row_index == nullptr) && (matrix.m_nonzeros == 0) && (matrix.m_m == 0) && (matrix.m_n == 0) && (matrix.empty() == true);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCSRDefaultConstructor" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCSRDefaultConstructor" << endl;
    return true;
}
 
//testCompressNormalNotZero
//Case: creates normal compress() on a 4x6 array and then calls getAt() on a stored spot that is not zero
//Expected: arrays match the expected CSR, getAt() returns the correct nonzero value
bool Tester::testCompressNormalNotZero(){
    CSR matrix;
    int array[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    matrix.compress(4,6,array,24);
 
    int expectedValues[] = {10,20,30,40,50,60,70,80};
    int expectedCols[] = {0,1,1,3,2,3,4,5};
    int expectedRows[] = {0,2,4,7,8};
 
    bool result = (matrix.m_nonzeros == 8) && (matrix.m_m == 4) && (matrix.m_n == 6);
    if (result){
        for (int i = 0; i < 8; i++){
            if (matrix.m_values[i] != expectedValues[i] || matrix.m_col_index[i] != expectedCols[i]) {
                result = false;
            }
        }
        for (int i = 0; i <= 4; i++){
            if (matrix.m_row_index[i] != expectedRows[i]) result = false;
        }
    }
    if (matrix.getAt(2,4) != 70) result = false;
 
    if (!result){
        cout << FAIL_STATEMENT << "testCompressNormalNotZero" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressNormalNotZero" << endl;
    return true;
}
 
//testCompressGetAtZero
//Case: calls getAt() on a valid, in range spot that was not stored
//Expected: returns 0 with no error
bool Tester::testCompressGetAtZero(){
    CSR matrix;
    int array[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    matrix.compress(4,6,array,24);
 
    bool result = (matrix.getAt(0,2) == 0) && (matrix.getAt(3,0) == 0);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCompressGetAtZero" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressGetAtZero" << endl;
    return true;
}
 
//testCompressSquare
//Case: creates square matrix and values are only on the diagonal
//Expected: matches correctly and one value that is not zero each row
bool Tester::testCompressSquare(){
    CSR matrix;
    int array[] = {5,0,0,0,6,0,0,0,7};
    matrix.compress(3,3,array,9);
 
    bool result = (matrix.m_m == 3) && (matrix.m_n == 3) && (matrix.m_nonzeros == 3);
    if (result){
        int expectedRows[] = {0,1,2,3};
        for (int i = 0; i <= 3; i++){
            if (matrix.m_row_index[i] != expectedRows[i]) {
                result = false;
            }
        }
    }
    if (matrix.getAt(0,0) != 5 || matrix.getAt(1,1) != 6 || matrix.getAt(2,2) != 7) {
        result = false;
    }
    if (!result){
        cout << FAIL_STATEMENT << "testCompressSquare" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressSquare" << endl;
    return true;
}
 
//testCompressMoreRowsThanCols
//Case: m > n which is 4 rows and 2 columns
//Expected: correct row/col index and no row/col swap error
bool Tester::testCompressMoreRowsThanCols(){
    CSR matrix;
    int array[] = {1,0,0,2,3,4,0,0};
    matrix.compress(4,2,array,8);
 
    bool result = (matrix.m_m == 4) && (matrix.m_n == 2) && (matrix.m_nonzeros == 4);
    if (matrix.getAt(0,0) != 1 || matrix.getAt(1,1) != 2 || matrix.getAt(2,0) != 3 || matrix.getAt(2,1) != 4 || matrix.getAt(3,0) != 0 || matrix.getAt(3,1) != 0){
        result = false;
    } 
    if (!result){
        cout << FAIL_STATEMENT << "testCompressMoreRowsThanCols" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressMoreRowsThanCols" << endl;
    return true;
}
 
//testCompressMoreColsThanRows
//Case: n > m which is 4 rows and 6 columns
//Expected: correct row/col index, no row/col swap error
bool Tester::testCompressMoreColsThanRows(){
    CSR matrix;
    int array[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    matrix.compress(4,6,array,24);
 
    bool result = (matrix.m_m == 4) && (matrix.m_n == 6) && (matrix.m_nonzeros == 8);
    if (matrix.getAt(1,3) != 40 || matrix.getAt(3,5) != 80) {
        result = false;
    }
    if (!result){
        cout << FAIL_STATEMENT << "testCompressMoreColsThanRows" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressMoreColsThanRows" << endl;
    return true;
}
 
//testCompressAllZeros
//Case: full array created where arraySize == m * n and every value is 0
//Expected: m_nonzeros == 0, m_row_index allocated and are all 0, m_values/m_col_index null
bool Tester::testCompressAllZeros(){
    CSR matrix;
    int array[] = {0,0,0,0,0,0};
    matrix.compress(2,3,array,6);
 
    bool result = (matrix.m_nonzeros == 0) && (matrix.m_values == nullptr) && (matrix.m_col_index == nullptr) && (matrix.m_row_index != nullptr) && (matrix.empty() == false);
    if (result){
        for (int i = 0; i <= 2; i++){
            if (matrix.m_row_index[i] != 0) {
                result = false;
            }
        }
    }
    if (!result){
        cout << FAIL_STATEMENT << "testCompressAllZeros" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressAllZeros" << endl;
    return true;
}
 
//testCompressOneNonzero
//Case: only one nonzero value anywhere in the array
//Expected: m_nonzeros == 1 and correctly put at its row/col
bool Tester::testCompressOneNonzero(){
    CSR matrix;
    int array[] = {0,0,0,5};
    matrix.compress(2,2,array,4);
 
    bool result = (matrix.m_nonzeros == 1) && (matrix.getAt(1,1) == 5) && (matrix.getAt(0,0) == 0) && (matrix.getAt(0,1) == 0) && (matrix.getAt(1,0) == 0);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCompressOneNonzero" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressOneNonzero" << endl;
    return true;
}
 
//testCompressZeroRowMiddle
//Case: one row is zero and the rows around it are not
//Expected: row_index shows no increase in the zero row
bool Tester::testCompressZeroRow(){
    CSR matrix;
    int array[] = {1,0,0,0,0,2};
    matrix.compress(3,2,array,6);
 
    bool result = (matrix.m_nonzeros == 2) && (matrix.getAt(0,0) == 1) && (matrix.getAt(2,1) == 2);
    if (result){
        int expectedRows[] = {0,1,1,2};
        for (int i = 0; i <= 3; i++){
            if (matrix.m_row_index[i] != expectedRows[i]) 
                result = false;
            }
        }

    if (!result){
        cout << FAIL_STATEMENT << "testCompressZeroRow" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressZeroRow" << endl;
    return true;
}
 
//testCompressConsecutiveZeroRows
//Case: two rows that are next to each other are 0
//Expected: row_index stays same in both zero rows
bool Tester::testCompressConsecutiveZeroRows(){
    CSR matrix;
    int array[] = {1,0,0,0,0,0,0,2};
    matrix.compress(4,2,array,8);
 
    bool result = (matrix.m_nonzeros == 2) && (matrix.getAt(0,0) == 1) && (matrix.getAt(3,1) == 2);
    if (result){
        int expectedRows[] = {0,1,1,1,2};
        for (int i = 0; i <= 4; i++){
            if (matrix.m_row_index[i] != expectedRows[i]) {
                result = false;
            }
        }
    }
 
    if (!result){
        cout << FAIL_STATEMENT << "testCompressConsecutiveZeroRows" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressConsecutiveZeroRows" << endl;
    return true;
}
 
//testCompressNotZeroInFirstSpot
//Case: array[0] is the only value that is not zero
//Expected: correctly put at the first CSR spot
bool Tester::testCompressNotZeroInFirstSpot(){
    CSR matrix;
    int array[] = {9,0,0,0};
    matrix.compress(2,2,array,4);
 
    bool result = (matrix.m_nonzeros == 1) && (matrix.getAt(0,0) == 9) && (matrix.m_row_index[0] == 0) && (matrix.m_col_index[0] == 0) && (matrix.m_values[0] == 9);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCompressNotZeroInFirstSpot" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressNotZeroInFirstSpot" << endl;
    return true;
}
 
//testCompressNotZeroInLastSpot
//Case: array[m * n - 1] or the last postion is the only value that is not zero
//Expected: correctly shows in m_row_index[m] and the last stored entry
bool Tester::testCompressNotZeroInLastSpot(){
    CSR matrix;
    int array[] = {0,0,0,0,0,7};
    matrix.compress(2,3,array,6);
 
    bool result = (matrix.m_nonzeros == 1) && (matrix.getAt(1,2) == 7) && (matrix.m_row_index[2] == 1) && (matrix.m_col_index[0] == 2) && (matrix.m_values[0] == 7);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCompressNotZeroInLastSpot" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressNotZeroInLastSpot" << endl;
    return true;
}
 
//testCompressNegativeNums
//Case: array has negative numbers
//Expected: negative numbers are like nonzero and stored correctly
bool Tester::testCompressNegativeNums(){
    CSR matrix;
    int array[] = {-5,0,0,-3};
    matrix.compress(2,2,array,4);
 
    bool result = (matrix.m_nonzeros == 2) && (matrix.getAt(0,0) == -5) && (matrix.getAt(1,1) == -3);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCompressNegativeNums" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressNegativeNums" << endl;
    return true;
}
 
//testCompressArraySizeLessThanMN
//Case: creates array with arraySize that is less then m * n 
//Expected: array data used for the first 5 spots and the rest of the matrix members are zero
bool Tester::testCompressArraySizeLessThanMN(){
    CSR matrix;
    int array[] = {1,0,0,0,2};
    matrix.compress(3,3,array,5);
 
    bool result = (matrix.m_nonzeros == 2) && (matrix.getAt(0,0) == 1) && (matrix.getAt(1,1) == 2) && (matrix.getAt(2,0) == 0) && (matrix.getAt(2,1) == 0) && (matrix.getAt(2,2) == 0);
    if (result){
        int expectedRows[] = {0,1,2,2};
        for (int i = 0; i <= 3; i++){
            if (matrix.m_row_index[i] != expectedRows[i]) {
                result = false;
            }
        }
    }
    if (!result){
        cout << FAIL_STATEMENT << "testCompressArraySizeLessThanMN" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressArraySizeLessThanMN" << endl;
    return true;
}
 
//testCompressArraySizeGreaterThanMN
//Case: creates array that is arraySize greater than m * n 
//Expected: only the first m * n elements are used and the rest are ignored
bool Tester::testCompressArraySizeGreaterThanMN(){
    CSR matrix;
    int array[] = {1,2,3,4,999,999};
    matrix.compress(2,2,array,6);
 
    bool result = (matrix.m_nonzeros == 4) && (matrix.getAt(0,0) == 1) && (matrix.getAt(0,1) == 2) && (matrix.getAt(1,0) == 3) && (matrix.getAt(1,1) == 4);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCompressArraySizeGreaterThanMN" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressArraySizeGreaterThanMN" << endl;
    return true;
}
 
//testCompressZeroByZeroWithNums
//Case: compresses even though the array has numbers in it
//Expected: object is empty even with the numbers given
bool Tester::testCompressZeroByZeroWithNums(){
    CSR matrix;
    int array[] = {1,2,3,4};
    matrix.compress(0,0,array,4);
 
    bool result = (matrix.empty() == true) && (matrix.m_values == nullptr) && (matrix.m_row_index == nullptr) && (matrix.m_nonzeros == 0);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCompressZeroByZeroWithNums" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressZeroByZeroWithNums" << endl;
    return true;
}
 
//testCompressPositiveDimensionsNoNums
//Case: valid m and n but no numbers given
//Expected: valid m x n object, m_nonzeros == 0, and m_row_index allocated and is all zero
bool Tester::testCompressPositiveDimensionsNoNums(){
    CSR nullMatrix;
    nullMatrix.compress(3,3,nullptr,0);
 
    bool result = (nullMatrix.empty() == false) && (nullMatrix.m_nonzeros == 0) && (nullMatrix.m_values == nullptr) && (nullMatrix.m_col_index == nullptr) && (nullMatrix.m_row_index != nullptr) && (nullMatrix.sparseRatio() == 100);
    if (result){
        for (int i = 0; i <= 3; i++){
            if (nullMatrix.m_row_index[i] != 0) {
                result = false;
            }
        }
    }
 
    CSR zeroMatrix;
    int array[] = {1,2,3,4};
    zeroMatrix.compress(2,2,array,0);
    if (zeroMatrix.empty() != false || zeroMatrix.m_nonzeros != 0 || zeroMatrix.m_row_index == nullptr){
        result = false;
    }
 
    if (!result){
        cout << FAIL_STATEMENT << "testCompressPositiveDimensionsNoNums" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressPositiveDimensionsNoNums" << endl;
    return true;
}

//testCompressBadDimensions
//Case: m or n is negative, or only one of m/n is zero while the other is positive (made sure with a TA on Discord)
//Expected: all four types create an empty object even though theres data
bool Tester::testCompressBadDimensions(){
    CSR negativeM, negativeN, zeroRowsOnly, zeroColsOnly;
    int array[] = {1,2,3,4,5,6};
 
    negativeM.compress(-2, 3, array, 6);
    negativeN.compress(2, -3, array, 6);
    zeroRowsOnly.compress(0, 3, array, 6);
    zeroColsOnly.compress(2, 0, array, 6);
 
    bool result = (negativeM.empty() == true) && (negativeM.m_m == 0) && (negativeM.m_n == 0) && (negativeN.empty() == true) && (negativeN.m_m == 0) && (negativeN.m_n == 0) && (zeroRowsOnly.empty() == true) && (zeroRowsOnly.m_m == 0) && (zeroRowsOnly.m_n == 0) && (zeroColsOnly.empty() == true) && (zeroColsOnly.m_m == 0) && (zeroColsOnly.m_n == 0);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCompressBadDimensions" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressBadDimensions" << endl;
    return true;
}
 
//testCopyConstructorNormal
//Case: copy constructor on a populated CSR
//Expected: deep copy
bool Tester::testCopyConstructorNormal(){
    CSR og;
    int array[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    og.compress(4,6,array,24);
 
    CSR copy(og);
 
    bool result = (copy == og) && (copy.m_values != og.m_values) && (copy.m_col_index != og.m_col_index) && (copy.m_row_index != og.m_row_index);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCopyConstructorNormal" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCopyConstructorNormal" << endl;
    return true;
}
 
//testCopyConstructorEmpty
//Case: calls copy constructor on a default CSR
//Expected: copy is also empty
bool Tester::testCopyConstructorEmpty(){
    CSR og;
    CSR copy(og);
 
    bool result = (copy.empty() == true) && (copy == og);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCopyConstructorEmpty" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCopyConstructorEmpty" << endl;
    return true;
}
 
//testCopyConstructorAllZero
//Case: calls copy constructor on a m x n, all zero CSR
//Expected: deep copy including m_row_index
bool Tester::testCopyConstructorAllZero(){
    CSR og;
    og.compress(3,3,nullptr,0);
 
    CSR copy(og);
 
    bool result = (copy == og) && (copy.empty() == false) && (copy.m_row_index != nullptr) && (copy.m_row_index != og.m_row_index);
    
    if (result){
        for (int i = 0; i <= 3; i++){
            if (copy.m_row_index[i] != 0) {
                result = false;
            }
        }
    }
    if (!result){
        cout << FAIL_STATEMENT << "testCopyConstructorAllZero" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCopyConstructorAllZero" << endl;
    return true;
}
 
//testCopyIndependenceAfterChanges
//Case: copy a CSR, then compresses the original again with different data
//Expected: the copys data is not effected by the change to the original
bool Tester::testCopyIndependenceAfterChanges(){
    CSR og;
    int array1[] = {1,0,0,2};
    og.compress(2,2,array1,4);
 
    CSR copy(og);
 
    int array2[] = {0,9,9,0};
    og.compress(2,2,array2,4);
 
    bool result = (copy.getAt(0,0) == 1) && (copy.getAt(1,1) == 2) && (copy.getAt(0,1) == 0) && (og.getAt(0,1) == 9) && (og.getAt(0,0) == 0);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCopyIndependenceAfterChanges" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCopyIndependenceAfterChanges" << endl;
    return true;
}
 
//testCompressSameObjectTwoTimes
//Case: calls compress() two times on the same CSR object with different numbers
//Expected: old memory released and replaced and final state matches the second compress() call
bool Tester::testCompressSameObjectTwoTimes(){
    CSR matrix;
    int array1[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    matrix.compress(4,6,array1,24);
 
    int array2[] = {1,0,0,2};
    matrix.compress(2,2,array2,4);
 
    bool result = (matrix.m_m == 2) && (matrix.m_n == 2) && (matrix.m_nonzeros == 2) && (matrix.getAt(0,0) == 1) && (matrix.getAt(1,1) == 2) && (matrix.getAt(0,1) == 0);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCompressSameObjectTwoTimes" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCompressSameObjectTwoTimes" << endl;
    return true;
}
 
//testGetAtNegativeRow
//Case: calls getAt() with a row less than 0
//Expected: throws runtime_error
bool Tester::testGetAtNegativeRow(){
    CSR matrix;
    int array[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    matrix.compress(4,6,array,24);
 
    bool threw = false;
    try{
        matrix.getAt(-1,0);
    }
    catch (runtime_error &error){
        threw = true;
    }
 
    if (!threw){
        cout << FAIL_STATEMENT << "testGetAtNegativeRow" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testGetAtNegativeRow" << endl;
    return true;
}
 
//testGetAtRowTooBig
//Case: calls getAt() with a row greater than or equal to m_m
//Expected: throws runtime_error
bool Tester::testGetAtRowTooBig(){
    CSR matrix;
    int array[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    matrix.compress(4,6,array,24);
 
    bool threw = false;
    try{
        matrix.getAt(4,0);
    }
    catch (runtime_error &error){
        threw = true;
    }
 
    if (!threw){
        cout << FAIL_STATEMENT << "testGetAtRowTooBig" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testGetAtRowTooBig" << endl;
    return true;
}
 
//testGetAtNegativeCol
//Case: calls getAt() with col less than 0
//Expected: throws runtime_error
bool Tester::testGetAtNegativeCol(){
    CSR matrix;
    int array[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    matrix.compress(4,6,array,24);
 
    bool threw = false;
    try{
        matrix.getAt(0,-1);
    }
    catch (runtime_error &error){
        threw = true;
    }
 
    if (!threw){
        cout << FAIL_STATEMENT << "testGetAtNegativeCol" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testGetAtNegativeCol" << endl;
    return true;
}
 
//testGetAtColumnTooBig
//Case: calls getAt() with columns greater than or equal to m_n
//Expected: throws runtime_error
bool Tester::testGetAtColumnTooBig(){
    CSR matrix;
    int array[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    matrix.compress(4,6,array,24);
 
    bool threw = false;
    try{
        matrix.getAt(0,6);
    }
    catch (runtime_error &error){
        threw = true;
    }
 
    if (!threw){
        cout << FAIL_STATEMENT << "testGetAtColumnTooBig" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testGetAtColumnTooBig" << endl;
    return true;
}
 
//testCSREqualityNormal
//Case: creates two CSR objects built from similar data
//Expected: operator== returns true
bool Tester::testCSREqualityNormal(){
    CSR a, b;
    int array[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    a.compress(4,6,array,24);
    b.compress(4,6,array,24);
 
    bool result = (a == b);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCSREqualityNormal" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCSREqualityNormal" << endl;
    return true;
}
 
//testCSREqualityDifferentMatrices
//Case: creates two CSR objects with different numbers
//Expected: operator== returns false
bool Tester::testCSREqualityDifferentMatrices(){
    CSR a, b;
    int arrayA[] = {1,0,0,2};
    int arrayB[] = {0,3,4,0};
    a.compress(2,2,arrayA,4);
    b.compress(2,2,arrayB,4);
 
    bool result = !(a == b);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCSREqualityDifferentMatrices" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCSREqualityDifferentMatrices" << endl;
    return true;
}
 
//testCSREqualityBothEmpty
//Case: creates two default constructed CSR objects
//Expected: operator== returns true
bool Tester::testCSREqualityBothEmpty(){
    CSR a, b;
 
    bool result = (a == b);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCSREqualityBothEmpty" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCSREqualityBothEmpty" << endl;
    return true;
}
 
//testSparseRatioEmpty
//Case: calls sparseRatio() on a default CSR
//Expected: returns 0
bool Tester::testSparseRatioEmpty(){
    CSR matrix;
 
    bool result = (matrix.sparseRatio() == 0);
 
    if (!result){
        cout << FAIL_STATEMENT << "testSparseRatioEmpty" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testSparseRatioEmpty" << endl;
    return true;
}
 
//testSparseRatioAllZero
//Case: calls sparseRatio() on a valid, all zero m x n matrix
//Expected: returns 100
bool Tester::testSparseRatioAllZero(){
    CSR matrix;
    matrix.compress(3,3,nullptr,0);
 
    bool result = (matrix.sparseRatio() == 100);
 
    if (!result){
        cout << FAIL_STATEMENT << "testSparseRatioAllZero" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testSparseRatioAllZero" << endl;
    return true;
}
 
//testSparseRatioNormal
//Case: calls sparseRatio() on a 2x2 matrix with only one value the is not zero
//Expected: returns 75
bool Tester::testSparseRatioNormal(){
    CSR matrix;
    int array[] = {0,1,0,0};
    matrix.compress(2,2,array,4);
 
    bool result = (matrix.sparseRatio() == 75);
 
    if (!result){
        cout << FAIL_STATEMENT << "testSparseRatioNormal" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testSparseRatioNormal" << endl;
    return true;
}

// CSR List Tests Implementation

//testCSRListDefaultConstructor
//Case: empty CSRList(), nothing inserted
//Expected: everything is set to zero or nullptr
bool Tester::testCSRListDefaultConstructor(){
    CSRList list;
 
    bool result = (list.m_head == nullptr) && (list.m_size == 0) && (list.empty() == true);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCSRListDefaultConstructor" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCSRListDefaultConstructor" << endl;
    return true;
}
 
//testInsertIntoEmptyList
//Case: calls insertAtHead() on an empty list
//Expected: size == 1, head matches the matrix
bool Tester::testInsertIntoEmptyList(){
    CSRList list;
    CSR matrix;
    int array[] = {5};
    matrix.compress(1,1,array,1);
    list.insertAtHead(matrix);
 
    bool result = (list.m_size == 1) && (list.m_head != nullptr) && (list.getAt(0,0,0) == 5);
 
    if (!result){
        cout << FAIL_STATEMENT << "testInsertIntoEmptyList" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testInsertIntoEmptyList" << endl;
    return true;
}
 
//testInsertCheckOrder
//Case: calls insertAtHead() three times with different 1x1 matrices
//Expected: list order is recently inserted at first
bool Tester::testInsertCheckOrder(){
    CSRList list;
    CSR c1, c2, c3;
    int array1[] = {5}, array2[] = {9}, array3[] = {13};
    c1.compress(1,1,array1,1);
    c2.compress(1,1,array2,1);
    c3.compress(1,1,array3,1);
 
    list.insertAtHead(c1); 
    list.insertAtHead(c2); 
    list.insertAtHead(c3);
 
    bool result = (list.m_size == 3) && (list.getAt(0,0,0) == 13) && (list.getAt(1,0,0) == 9) && (list.getAt(2,0,0) == 5);
 
    if (!result){
        cout << FAIL_STATEMENT << "testInsertCheckOrder" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testInsertCheckOrder" << endl;
    return true;
}
 
//testInsertBigList
//Case: calls insertAtHead() 15 times, each with a different 1x1 matrix
//Expected: correct size, and order matches LIFO insertion
bool Tester::testInsertBigList(){
    CSRList list;
    for (int i = 1; i <= 15; i++){
        CSR c;
        int arr[] = {i};
        c.compress(1,1,arr,1);
        list.insertAtHead(c);
    }
 
    bool result = (list.m_size == 15) && (list.getAt(0,0,0) == 15) && (list.getAt(14,0,0) == 1) && (list.getAt(7,0,0) == 8);
 
    if (!result){
        cout << FAIL_STATEMENT << "testInsertBigList" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testInsertBigList" << endl;
    return true;
}

//testCopyConstructorNormal
//Case: after copying a populated list, inserts a new matrix into the og
//Expected: the copy is doesn't change
bool Tester::testListCopyConstructorNormal(){
    CSRList og;
    CSR c1;
    int array1[] = {5};
    c1.compress(1,1,array1,1);
    og.insertAtHead(c1);
 
    CSRList copy(og);
 
    CSR c2;
    int array2[] = {9};
    c2.compress(1,1,array2,1);
    og.insertAtHead(c2);
 
    bool result = (copy.m_size == 1) && (og.m_size == 2) && (copy.getAt(0,0,0) == 5);
 
    if (!result){
        cout << FAIL_STATEMENT << "testCopyConstructorNormal" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testCopyConstructorNormal" << endl;
    return true;
}
 
//testListCopyConstructorWithOneNode
//Case: copy constructor on a single-node CSRList
//Expected: deep copy, equal contents
bool Tester::testListCopyConstructorWithOneNode(){
    CSRList og;
    CSR matrix;
    int array[] = {7};
    matrix.compress(1,1,array,1);
    og.insertAtHead(matrix);
 
    CSRList copy(og);
 
    bool result = (copy == og) && (copy.m_size == 1) && (copy.getAt(0,0,0) == 7);
 
    if (!result){
        cout << FAIL_STATEMENT << "testListCopyConstructorWithOneNode" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testListCopyConstructorWithOneNode" << endl;
    return true;
}
 
//testListCopyConstructorWithLotsOfNodes
//Case: copy constructor on a list with lots of nodes
//Expected: order stays
bool Tester::testListCopyConstructorWithLotsOfNodes(){
    CSRList og;
    CSR c1, c2, c3;
    int array1[] = {5}, array2[] = {9}, array3[] = {13};
    c1.compress(1,1,array1,1);
    c2.compress(1,1,array2,1);
    c3.compress(1,1,array3,1);
    og.insertAtHead(c1);
    og.insertAtHead(c2);
    og.insertAtHead(c3);
 
    CSRList copy(og);
 
    bool result = (copy == og) && (copy.m_head != og.m_head) && (copy.getAt(0,0,0) == 13) && (copy.getAt(1,0,0) == 9) && (copy.getAt(2,0,0) == 5);
 
    if (!result){
        cout << FAIL_STATEMENT << "testListCopyConstructorWithLotsOfNodes" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testListCopyConstructorWithLotsOfNodes" << endl;
    return true;
}
 
//testListCopyConstructorEmptyList
//Case: calls copy constructor on an empty CSRList
//Expected: copy is empty
bool Tester::testListCopyConstructorEmptyList(){
    CSRList og;
    CSRList copy(og);
 
    bool result = (copy.empty() == true) && (copy == og);
 
    if (!result){
        cout << FAIL_STATEMENT << "testListCopyConstructorEmptyList" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testListCopyConstructorEmptyList" << endl;
    return true;
}

//testAssignNormal
//Case: after list2 = list1, modify list1 (insertAtHead a new matrix)
//Expected: list2 is unaffected (deep copy, not shared nodes)
bool Tester::testAssignNormal(){
    CSRList list1, list2;
    CSR c1;
    int array1[] = {5};
    c1.compress(1,1,array1,1);
    list1.insertAtHead(c1);
 
    list2 = list1;
 
    CSR c2;
    int array2[] = {9};
    c2.compress(1,1,array2,1);
    list1.insertAtHead(c2);
 
    bool result = (list2.m_size == 1) && (list1.m_size == 2) && (list2.getAt(0,0,0) == 5);
 
    if (!result){
        cout << FAIL_STATEMENT << "testAssignNormal" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testAssignNormal" << endl;
    return true;
} 

//testAssignSelf
//Case: self assignment
//Expected: object doesn't change, and no errors
bool Tester::testSelfAssign(){
    CSRList list;
    CSR c1, c2;
    int array1[] = {5}, array2[] = {9};
    c1.compress(1,1,array1,1);
    c2.compress(1,1,array2,1);
    list.insertAtHead(c1);
    list.insertAtHead(c2);
 
    CSR* oldHead = list.m_head;
    int oldSize = list.m_size;
 
    list = list;
 
    bool result = (list.m_head == oldHead) && (list.m_size == oldSize) && (list.getAt(0,0,0) == 9) && (list.getAt(1,0,0) == 5);
 
    if (!result){
        cout << FAIL_STATEMENT << "testSelfAssign" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testSelfAssign" << endl;
    return true;
}
 
//testAssignNotEmptyToNotEmpty
//Case: assign one populated list to different populated list
//Expected: lhs becomes an deep copy of rhs
bool Tester::testAssignNotEmptyToNotEmpty(){
    CSRList list1, list2;
    CSR c1, c2, c3;
    int array1[] = {5}, array2[] = {9}, array3[] = {100};
    c1.compress(1,1,array1,1);
    c2.compress(1,1,array2,1);
    c3.compress(1,1,array3,1);
    list1.insertAtHead(c1);
    list1.insertAtHead(c2);
    list2.insertAtHead(c3); 
 
    list2 = list1;
 
    bool result = (list2 == list1) && (list2.m_size == 2) && (list2.m_head != list1.m_head) && (list2.getAt(0,0,0) == 9) && (list2.getAt(1,0,0) == 5);
 
    if (!result){
        cout << FAIL_STATEMENT << "testAssignNotEmptyToNotEmpty" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testAssignNotEmptyToNotEmpty" << endl;
    return true;
}
 
//testAssignNotEmptyToEmpty
//Case: assign a populated list to an empty list
//Expected: lhs becomes a deep copy of rhs
bool Tester::testAssignNotEmptyToEmpty(){
    CSRList list1, list2;
    CSR c1;
    int array1[] = {5};
    c1.compress(1,1,array1,1);
    list2.insertAtHead(c1);
 
    list1 = list2;
 
    bool result = (list1 == list2) && (list1.m_size == 1) && (list1.getAt(0,0,0) == 5);
 
    if (!result){
        cout << FAIL_STATEMENT << "testAssignNotEmptyToEmpty" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testAssignNotEmptyToEmpty" << endl;
    return true;
}
 
//testAssignEmptyToNotEmpty
//Case: assign an empty list to a populated list
//Expected: lhs is empty which matching rhs
bool Tester::testAssignEmptyToNotEmpty(){
    CSRList list1, list2;
    CSR c1;
    int array1[] = {5};
    c1.compress(1,1,array1,1);
    list1.insertAtHead(c1);
 
    list1 = list2;
 
    bool result = (list1.empty() == true) && (list1.m_head == nullptr) && (list1.m_size == 0) && (list1 == list2);
 
    if (!result){
        cout << FAIL_STATEMENT << "testAssignEmptyToNotEmpty" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testAssignEmptyToNotEmpty" << endl;
    return true;
}
 
//testListEqualitySameOrder
//Case: creates two lists by inserting the same matrices in the same order
//Expected: operator== returns true
bool Tester::testListEqualitySameOrder(){
    CSRList list1, list2;
    CSR c1, c2;
    int array1[] = {5}, array2[] = {9};
    c1.compress(1,1,array1,1);
    c2.compress(1,1,array2,1);
 
    list1.insertAtHead(c1);
    list1.insertAtHead(c2);
    list2.insertAtHead(c1);
    list2.insertAtHead(c2);
 
    bool result = (list1 == list2);
 
    if (!result){
        cout << FAIL_STATEMENT << "testListEqualitySameOrder" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testListEqualitySameOrder" << endl;
    return true;
}
 
//testListEqualityDifferentOrder
//Case: same two matrices but inserted in a different order
//Expected: operator== returns false
bool Tester::testListEqualityDifferentOrder(){
    CSRList list1, list2;
    CSR c1, c2;
    int array1[] = {5}, array2[] = {9};
    c1.compress(1,1,array1,1);
    c2.compress(1,1,array2,1);
 
    list1.insertAtHead(c1);
    list1.insertAtHead(c2);
    list2.insertAtHead(c2);
    list2.insertAtHead(c1);
 
    bool result = !(list1 == list2);
 
    if (!result){
        cout << FAIL_STATEMENT << "testListEqualityDifferentOrder" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testListEqualityDifferentOrder" << endl;
    return true;
}
 
//testListEqualityDifferentMatrix
//Case: same size lists but one matrix numbers are different
//Expected: operator== returns false
bool Tester::testListEqualityDifferentMatrix(){
    CSRList list1, list2;
    CSR c1, c2, c3;
    int array1[] = {5}, array2[] = {9}, array3[] = {999};
    c1.compress(1,1,array1,1);
    c2.compress(1,1,array2,1);
    c3.compress(1,1,array3,1);
 
    list1.insertAtHead(c1);
    list1.insertAtHead(c2);
    list2.insertAtHead(c1);
    list2.insertAtHead(c3);

    bool result = !(list1 == list2);
 
    if (!result){
        cout << FAIL_STATEMENT << "testListEqualityDifferentMatrix" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testListEqualityDifferentMatrix" << endl;
    return true;
}
 
//testListEqualityBothEmpty
//Case: two empty CSRLists
//Expected: operator== returns true
bool Tester::testListEqualityBothEmpty(){
    CSRList list1, list2;
 
    bool result = (list1 == list2);
 
    if (!result){
        cout << FAIL_STATEMENT << "testListEqualityBothEmpty" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testListEqualityBothEmpty" << endl;
    return true;
}
 
//testListEqualityEmptyAndNotEmpty
//Case: creates one empty list and one populated list
//Expected: operator== returns false
bool Tester::testListEqualityEmptyAndNotEmpty(){
    CSRList list1, list2;
    CSR c1;
    int array1[] = {5};
    c1.compress(1,1,array1,1);
    list2.insertAtHead(c1);
 
    bool result = !(list1 == list2);
 
    if (!result){
        cout << FAIL_STATEMENT << "testListEqualityEmptyAndNotEmpty" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testListEqualityEmptyAndNotEmpty" << endl;
    return true;
}

//testClearListWithData
//Case: calls clear() on a list with multiple nodes
//Expected: everything is zero or nullptr with no errors
bool Tester::testClearListWithData(){
    CSRList list;
    CSR c1, c2;
    int array1[] = {5}, array2[] = {9};
    c1.compress(1,1,array1,1);
    c2.compress(1,1,array2,1);
    list.insertAtHead(c1);
    list.insertAtHead(c2);
 
    list.clear();
 
    bool result = (list.m_head == nullptr) && (list.m_size == 0) && (list.empty() == true);
 
    if (!result){
        cout << FAIL_STATEMENT << "testClearListWithData" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testClearListWithData" << endl;
    return true;
}
 
//testClearEmptyListTwoTimes
//Case: calls clear() two times in a row on a list that is already empty
//Expected: no crash, list remains empty
bool Tester::testClearEmptyListTwoTimes(){
    CSRList list;
 
    list.clear();
    list.clear();
 
    bool result = (list.m_head == nullptr) && (list.m_size == 0) && (list.empty() == true);
 
    if (!result){
        cout << FAIL_STATEMENT << "testClearEmptyListTwoTimes" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testClearEmptyListTwoTimes" << endl;
    return true;
}
 
//testClearThenReuse
//Case: calls clear() a list with data and then inserts again
//Expected: list is can be used again with new numbers
bool Tester::testClearThenReuse(){
    CSRList list;
    CSR c1;
    int array1[] = {5};
    c1.compress(1,1,array1,1);
    list.insertAtHead(c1);
 
    list.clear();
 
    CSR c2;
    int array2[] = {42};
    c2.compress(1,1,array2,1);
    list.insertAtHead(c2);
 
    bool result = (list.m_size == 1) && (list.getAt(0,0,0) == 42);
 
    if (!result){
        cout << FAIL_STATEMENT << "testClearThenReuse" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testClearThenReuse" << endl;
    return true;
}
 
//testListGetAtForAllSpots
//Case: valid CSRIndex at each spot of a 3-node list
//Expected: correct values returned for each
bool Tester::testListGetAtForAllSpots(){
    CSRList list;
    CSR c1, c2, c3;
    int array1[] = {5}, array2[] = {9}, array3[] = {13};
    c1.compress(1,1,array1,1);
    c2.compress(1,1,array2,1);
    c3.compress(1,1,array3,1);
    list.insertAtHead(c1);
    list.insertAtHead(c2);
    list.insertAtHead(c3);
 
    bool result = (list.getAt(0,0,0) == 13) && (list.getAt(1,0,0) == 9) && (list.getAt(2,0,0) == 5);
 
    if (!result){
        cout << FAIL_STATEMENT << "testListGetAtForAllSpots" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testListGetAtForAllSpots" << endl;
    return true;
}
 
//testListGetAtNegativeIndex
//Case: calls getAt() with CSRIndex thats less than 0
//Expected: throws runtime_error
bool Tester::testListGetAtNegativeIndex(){
    CSRList list;
    CSR c1;
    int array1[] = {5};
    c1.compress(1,1,array1,1);
    list.insertAtHead(c1);
 
    bool threw = false;
    try{
        list.getAt(-1,0,0);
    }
    catch (runtime_error &error){
        threw = true;
    }
 
    if (!threw){
        cout << FAIL_STATEMENT << "testListGetAtNegativeIndex" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testListGetAtNegativeIndex" << endl;
    return true;
}
 
//testListGetAtIndexSameSize
//Case: calls getAt() with CSRIndex == m_size on a 2-node list
//Expected: throws runtime_error
bool Tester::testListGetAtIndexSameSize(){
    CSRList list;
    CSR c1, c2;
    int array1[] = {5}, array2[] = {9};
    c1.compress(1,1,array1,1);
    c2.compress(1,1,array2,1);
    list.insertAtHead(c1);
    list.insertAtHead(c2);
 
    bool threw = false;
    try{
        list.getAt(2,0,0);
    }
    catch (runtime_error &error){
        threw = true;
    }
 
    if (!threw){
        cout << FAIL_STATEMENT << "testListGetAtIndexSameSize" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testListGetAtIndexSameSize" << endl;
    return true;
}
 
//testListGetAtInvalidRowCol
//Case: valid CSRIndex but row/col is out of range for thae matrix
//Expected: throws runtime_error
bool Tester::testListGetAtInvalidRowCol(){
    CSRList list;
    CSR c1;
    int array[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
    c1.compress(4,6,array,24);
    list.insertAtHead(c1);
 
    bool threw = false;
    try{
        list.getAt(0,10,0);
    }
    catch (runtime_error &error){
        threw = true;
    }
 
    if (!threw){
        cout << FAIL_STATEMENT << "testListGetAtInvalidRowCol" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testListGetAtInvalidRowCol" << endl;
    return true;
}
 
//testListGetAtEmptyList
//Case: calls getAt() on an empty list
//Expected: throws runtime_error
bool Tester::testListGetAtEmptyList(){
    CSRList list;
 
    bool threw = false;
    try{
        list.getAt(0,0,0);
    }
    catch (runtime_error &error){
        threw = true;
    }
 
    if (!threw){
        cout << FAIL_STATEMENT << "testListGetAtEmptyList" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testListGetAtEmptyList" << endl;
    return true;
}
 
//testAverageOneMatrix
//Case: creates a list with a single matrix with a known sparseRatio of 75
//Expected: averageSparseRatio() equals that matrix sparseRatio()
bool Tester::testAverageOneMatrix(){
    CSRList list;
    CSR c1;
    int array[] = {0,1,0,0};
    c1.compress(2,2,array,4);
    list.insertAtHead(c1);
 
    bool result = (list.averageSparseRatio() == 75);
 
    if (!result){
        cout << FAIL_STATEMENT << "testAverageOneMatrix" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testAverageOneMatrix" << endl;
    return true;
}
 
//testAverageLotsOfMatrices
//Case: creates list with three matrices with sparse ratios 75, 50, and 0
//Expected: averageSparseRatio() returns the correct avgrage of 41
bool Tester::testAverageLotsOfMatrices(){
    CSRList list;
    CSR c1, c2, c3;
    int array1[] = {0,1,0,0};
    int array2[] = {0,1,0,2};
    int array3[] = {1,2,3,4};
    c1.compress(2,2,array1,4);
    c2.compress(2,2,array2,4);
    c3.compress(2,2,array3,4);
    list.insertAtHead(c1);
    list.insertAtHead(c2);
    list.insertAtHead(c3);
 
    bool result = (list.averageSparseRatio() == 41);
 
    if (!result){
        cout << FAIL_STATEMENT << "testAverageLotsOfMatrices" << endl;
        return false;
    }
    cout << PASS_STATEMENT << "testAverageLotsOfMatrices" << endl;
    return true;
}


int main() {
    Tester test;

    test.testCSRDefaultConstructor();
 
    test.testCompressNormalNotZero();
    test.testCompressGetAtZero();
 
    test.testCompressSquare();
    test.testCompressMoreRowsThanCols();
    test.testCompressMoreColsThanRows();
 
    test.testCompressAllZeros();
    test.testCompressOneNonzero();
    test.testCompressZeroRow();
    test.testCompressConsecutiveZeroRows();
    test.testCompressNotZeroInFirstSpot();
    test.testCompressNotZeroInLastSpot();
    test.testCompressNegativeNums();
 
    test.testCompressArraySizeLessThanMN();
    test.testCompressArraySizeGreaterThanMN();
    test.testCompressZeroByZeroWithNums();
    test.testCompressPositiveDimensionsNoNums();
    test.testCompressBadDimensions();
 
    test.testCopyConstructorNormal();
    test.testCopyConstructorEmpty();
    test.testCopyConstructorAllZero();
    test.testCopyIndependenceAfterChanges();
    test.testCompressSameObjectTwoTimes();
 
    test.testGetAtNegativeRow();
    test.testGetAtRowTooBig();
    test.testGetAtNegativeCol();
    test.testGetAtColumnTooBig();
 
    test.testCSREqualityNormal();
    test.testCSREqualityDifferentMatrices();
    test.testCSREqualityBothEmpty();
 
    test.testSparseRatioEmpty();
    test.testSparseRatioAllZero();
    test.testSparseRatioNormal();

    test.testCSRListDefaultConstructor();
 
    test.testInsertIntoEmptyList();
    test.testInsertCheckOrder();
    test.testInsertBigList();
 
    test.testListCopyConstructorNormal();
    test.testListCopyConstructorWithOneNode();
    test.testListCopyConstructorWithLotsOfNodes();
    test.testListCopyConstructorEmptyList();
 
    test.testAssignNormal();
    test.testSelfAssign();
    test.testAssignNotEmptyToNotEmpty(); 
    test.testAssignNotEmptyToEmpty();
    test.testAssignEmptyToNotEmpty();
 
    test.testListEqualitySameOrder();
    test.testListEqualityDifferentOrder();
    test.testListEqualityDifferentMatrix();
    test.testListEqualityBothEmpty();
    test.testListEqualityEmptyAndNotEmpty();
 
    test.testListGetAtForAllSpots();
    test.testListGetAtNegativeIndex(); 
    test.testListGetAtIndexSameSize();
    test.testListGetAtInvalidRowCol();
    test.testListGetAtEmptyList();
 
    test.testAverageOneMatrix();
    test.testAverageLotsOfMatrices();
 
    test.testClearListWithData();
    test.testClearEmptyListTwoTimes();
    test.testClearThenReuse();
}
