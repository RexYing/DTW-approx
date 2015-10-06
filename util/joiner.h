#include <iostream>
#include <initializer_list>

class Joiner
{
public:
	Joiner static on(string separator)
	{
		Joiner j(separator);
		return j;
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
	}

		
private:
	Joiner(string separator): separator_(separator) { };
		
	string separator_;
	
};