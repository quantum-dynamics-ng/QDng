#include <iostream>

#include "Exception.h"

using namespace std;
using namespace QDLIB;


int main (int argc, char **argv)
{
	cout << "Let's throw an Exception\n";
	try {
           throw( EIOError("Aaaargh") );
        } catch (EIOError e) {
           cout << e.GetMessage() << endl;
        }
              
        
	cout << "Nothing happened\n";
}
