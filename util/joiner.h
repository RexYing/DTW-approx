#ifndef JOINER
#define JOINER

#include <iostream>
#include <initializer_list>

class Joiner
{
public:
	static Joiner* on(std::string separator)
	{
		return new Joiner(separator);
	}
	
	std::string join(std::initializer_list<int> list)
	{
		std::stringstream sstm;
		auto it = list.begin();
		sstm << *it;
		it++;
    while (it != list.end())
    {
        sstm << separator_ << *it;
				it++;
    }
		return sstm.str();
	}

		
private:
	Joiner(std::string separator): separator_(separator) { };
		
	std::string separator_;
	
};

#endif
