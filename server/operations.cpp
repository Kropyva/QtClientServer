#include "operations.h"

#include <string>
#include <sstream>
#include <cmath>

QVector<double> dataToNumberVector(QByteArray& message) {
    std::istringstream stream(message.toStdString());
    QVector<double> numbers {};

    std::string word {};
    while (std::getline(stream, word, ' ')) {
        numbers.push_back(std::stod(word));
    }

    return numbers;
}

double arithmeticMean(QVector<double>& numbers) {
    double value {};

    for (auto& number : numbers) {
        value += number;
    }

    return value / numbers.size();
}

double squareDeviation(QVector<double>& numbers) {
    double value {};
    double average { arithmeticMean(numbers) };

    for (auto& number : numbers) {
        value += std::pow(number - average, 2);
    }

    return std::sqrt(value / numbers.size());
}
