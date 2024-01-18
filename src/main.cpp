#include "./utils.hpp"
#include "./headers/QVM.hpp"

int main(int argc, char* argv[]) {
    std::string qasm_file_path = "";
    std::string input_flag = "-i";

    std::string byte_count_flag = "-b";
    unsigned int byte_count = 512;

    for (int i = 0; i < argc; i++) {
        if (argv[i] == input_flag) {
            qasm_file_path = argv[i + 1];
        }

        if (argv[i] == byte_count_flag) {
            byte_count = std::stoi(argv[i + 1]);
        }
    }

    if (qasm_file_path == "") {
        throw std::runtime_error("ERR: the path to the source code file must be specified");
    }

    std::string qasm_code = copy_qasm_code(qasm_file_path);
    
    QVM qvm(byte_count, qasm_code);
    qvm.run();

    return 0;
}
