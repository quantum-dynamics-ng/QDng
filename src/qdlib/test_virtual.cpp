#include <iostream>

using namespace  std;

class baseV {
   public:
      virtual ~baseV() {}
      virtual void say() = 0;
};

class baseB  {
   public:
      void say() { cout << "deriv1 called" << endl; }
};

class deriv : public baseV, public baseB {
	public:
	virtual void say() { baseB::say(); }
};



int main(int argc, char **argv)
{
   deriv d;
   
   d.say();
   
}

