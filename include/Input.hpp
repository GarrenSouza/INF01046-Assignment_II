#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>

namespace Local{

    class UserInput{
        public:
            std::string getUserInput(std::string msg, std::istream& input_stream);
            std::string getUserInput(std::istream& input_stream);
    };

}

#endif // INPUT_HPP
