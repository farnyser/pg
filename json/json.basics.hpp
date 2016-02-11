#include <string>
#include <vector>

template <> 
void serialize(std::ostream& os, const std::string& data)
{
    os << "\"";
    for(auto& c : data)
    {
    	if(c == '"')
    		os << '\\';

    	os << c;
    }
    os << "\"";
}

template <typename T, 
	typename std::enable_if<
		is_collection<T>::value
	>::type* = nullptr
>
void serialize(std::ostream& os, const T& data)
{		
    os << "[";

    for(auto it = data.begin() ; it != data.end() ; ++it) 
    {
    	if(it != data.begin())
    		os << ",";
    	serialize(os, *it);
    }
    
    os << "]";
}
