#include <iostream>
#include <initializer_list>

class Joiner
{
public:
	static Joiner* on(string separator)
	{
		return new Joiner(separator);
	}
	
	string join(std::initializer_list<int> list)
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
	Joiner(string separator): separator_(separator) { };
		
	string separator_;
	
};