#include "include/Input.hpp"

namespace Local{

    std::string UserInput::getUserInput(std::string msg, std::istream& input_stream){
        std::cout << msg;
        return UserInput::getUserInput(input_stream);
    }
    std::string UserInput::getUserInput(std::istream& input_stream){
        std::string input;
        std::getline(input_stream, input);
        input_stream.clear();

        return input;
    }
}
