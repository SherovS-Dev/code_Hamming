#include "code_hamming.h"
#include <vector>

// Преобразование строки бинарного числа в целое число
int binaryStringToInt(const std::string& binaryStr) {
    /*пример работы:
     * ввод:  01101110
     * вывод: 110
     */
    return static_cast<int>(std::bitset<64>(binaryStr).to_ulong());
}

// Преобразование строки числа в бинарное представление
std::string toBinaryString(const std::string& numStr) {
    /*пример работы:
     * ввод:  42
     * вывод: 000000000000000000000000101010
     */
    int num = std::stoi(numStr);
    std::bitset<32> binary(num);
    return binary.to_string();
}

// Удаление лишних нулей слева от строки
std::string eraseZeros(const std::string& numStr) {
    /*пример работы:
     * ввод:  000000000000000000000000101010
     * вывод: 00101010
     */
    std::string newNum;
    bool isStart = false;

    for (char c : numStr) {
        if (isStart)
            newNum += c;
        else if (c != '0') {
            isStart = true;
            newNum += c;
        }
    }

    while (newNum.length() % 4)
        newNum = '0' + newNum; // Clang-Tidy: String concatenation results in allocation of
    // unnecessary temporary strings; consider using 'operator+=' or 'string::append()' instead

    return newNum;
}

// Разделение строки на вектор значений uint8_t
std::vector<uint8_t> splitStringToUint8(const std::string& binaryStr) {
    /*пример работы:
     * ввод:  00101010
     * вывод: [{0010},{1010}]
     */
    std::vector<uint8_t> result;
    for (size_t i = 0; i < binaryStr.length(); i += 4) {
        std::string byte = binaryStr.substr(i, 4);
        uint8_t value = static_cast<uint8_t>(std::bitset<4>(byte).to_ulong());
        result.push_back(value);
    }
    return result;
}

int main() {
    std::cout << "Добро пожаловать в программу \"кода Хэмминга\".";
    while (true) {
        std::cout << "\nВведите число (например 1234): ";
        std::string number;
        std::cin >> number;

        std::vector<uint8_t> encode = splitStringToUint8(eraseZeros(toBinaryString(number)));
        std::vector<uint8_t> decode;
        std::cout << "Число " << number << " была конвертировано " << eraseZeros(toBinaryString(number)) <<
        " в бинарную систему и была разделена на части:\n";

        for(int i = 0; i < encode.size(); i++)
            std::cout << "    " << i+1 << ' ' << std::bitset<4>(encode[i]) << '\n';
        std::cout << "\n";
        
        bool isContinuing = true;
        while (isContinuing) {
            std::cout << "Выберите ваше действие:\n" <<
                      "    1 - Изменить число (например 1234)\n" <<
                      "    2 - Кодировать число по Хэммингу\n" <<
                      "    3 - Декодировать число по Хэммингу\n" <<
                      "    4 - Инвертирование\n" <<
                      "    Любая клавиша - Выход из программы\n" <<
                      "    Ваше действие: ";
            char symbol;
            std::cin >> symbol;
            std::string str;
            switch (symbol) {
                case '1':
                    isContinuing = false;
                    break;

                case '2':
                    std::cout << "\n    Ваше число было закодировано по частям:\n";
                    for (int i = 0; i < encode.size(); i++) {
                        decode.push_back(static_cast<uint8_t>(encodeHamming(encode[i])));
                        std::cout << "      " << std::bitset<4>(encode[i]) << " -===> " << std::bitset<8>(decode[i]) << std::endl;
                    }   std::cout << std::endl;
                    break;

                case '3':
                    std::cout << "\n    Декодер восстанавливает ваше число:\n";
                    if (decode.empty()) {
                        std::cout << "      Кодирование еще не было произведено. Пожалуйста, сначала кодируйте ваше число.\n\n";
                        break;
                    }

                    decodestart: str = "";
                    for (int i = 0; i < decode.size(); i++) {
                        uint8_t tmp = decodeHamming(decode[i]);
                        if (static_cast<int>(tmp) == static_cast<int>(encode[i])) {
                            str += std::bitset<4>(encode[i]).to_string();
                            std::cout << "      " << std::bitset<8>(decode[i]) << " -===> " << std::bitset<4>(encode[i])
                                      << std::endl;
                        }
                    }
                    std::cout << std::endl;

                    std::cout << "      " << str << " (BIN) -===> " <<
                              binaryStringToInt(str) << " (DEC)\n\n";
                    break;

                case '4':
                    std::cout << "\n";
                    for (int i = 0; i < decode.size(); i++)
                        std::cout << "    " << i+1 << " " << std::bitset<8>(decode[i]) << std::endl;

                    std::cout << "\n    Выберите элемент для инвертирования (от 1 до " << decode.size() << "): ";
                    int bitNum;
                    std::cin >> bitNum;
                    if (bitNum < 1 || bitNum > decode.size()) {
                        std::cout << "    Выбран неправильный элемент вектора\n\n";
                    } else {
                        std::cout << "      Оригинальный байт "<<std::bitset<8>(decode[bitNum-1]) <<
                        " изменить на: ";
                        std::cin >> str;
                        decode[bitNum-1] = static_cast<uint8_t>(std::bitset<8>(str).to_ulong());
                        goto decodestart;
                    }
                    break;

                default:
                    std::cout << "\nСпасибо, что пользуетесь нашим сервисом. Оцените меня, пожалуйста. " <<
                              "Ваше мнение очень важно для нас!" << std::endl;
                    isContinuing = false;
                    break;
            }
        }
        if (!isContinuing) break;
    }
}
