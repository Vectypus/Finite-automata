#include "readDisplay.h"

int main(){

    FA* fa = readAutomaton("FA example.txt");
    displayAutomaton(fa);
    system("pause");

    return 0;
}
