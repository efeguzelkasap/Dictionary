#include <iostream>
#include "Dictionary.h"

using namespace Containers;

int main()
{
	
	Dictionary<std::string, std::string> dict;
	
	dict.insert("1", "Efe");
	dict.insert("2", "Efe Guz");
	dict.insert("3", "jannet");
	dict.insert("4", "bob");
	dict.insert("5", "Andrew");
	dict.insert("6", "Mike");
	dict.insert("7", "Dan");
	dict.insert("3", "peter");
	dict.insert("8", "Arron");

	std::cout <<"---Dict---" << std::endl;
	dict.displayDictionary();
	
	(dict.*dict.removeIf)("1");

	//dict.displayDictionary();
	
	Dictionary<std::string, std::string> dictTwo = dict;


	

	


}


