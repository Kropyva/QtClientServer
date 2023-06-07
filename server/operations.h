#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <QByteArray>
#include <QVector>

QVector<double> dataToNumberVector(QByteArray& message);
double arithmeticMean(QVector<double>& numbers);
double squareDeviation(QVector<double>& numbers);

#endif // OPERATIONS_H
