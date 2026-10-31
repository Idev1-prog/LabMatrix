#include "TMatrix.h"
#include "TTriangleMatrix.h"

#include <iostream>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <string>
#include <locale>

// Весь интерфейс сгенерирован, я только редачил чтобы получить нормальный результат
using Matrix = TMatrix<double>;
using TriangleMatrix = TTriangleMatrix<double>;

// ─────────────────────────── утилиты ────────────────────────────

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void flushLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

double readDouble(const std::string& prompt) {
    double v = 0.0;
    std::cout << prompt;
    if (!(std::cin >> v)) { clearInput(); throw std::runtime_error("Ожидалось число"); }
    flushLine();
    return v;
}

Matrix readMatrix(const std::string& name) {
    std::cout << "Матрица " << name << " — <rows> <cols> <элементы...>\n> ";
    Matrix m;
    if (!(std::cin >> m)) { clearInput(); throw std::runtime_error("Ошибка чтения " + name); }
    flushLine();
    return m;
}

TriangleMatrix readTriangle(const std::string& name) {
    std::cout << "Треугольная " << name
        << " — <rows> <cols> <нижнетреугольные элементы...>\n> ";
    TriangleMatrix m;
    if (!(std::cin >> m)) { clearInput(); throw std::runtime_error("Ошибка чтения " + name); }
    flushLine();
    return m;
}

double determinant(const Matrix& m) {
    const size_t n = m.size();
    if (n == 0 || m[0].size() != n)
        throw std::invalid_argument("Определитель только для квадратной матрицы");
    if (n == 1) return m(0, 0);
    if (n == 2) return m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);

    double det = 0.0;
    for (size_t col = 0; col < n; ++col) {
        Matrix minor(n - 1, n - 1);
        for (size_t i = 1; i < n; ++i) {
            size_t k = 0;
            for (size_t j = 0; j < n; ++j) {
                if (j == col) continue;
                minor(i - 1, k++) = m(i, j);
            }
        }
        det += ((col % 2 == 0) ? 1.0 : -1.0) * m(0, col) * determinant(minor);
    }
    return det;
}

// ───────────────────── меню и подменю ───────────────────────────

void printMatrixMenu() {
    std::cout << "── Матрицы ──\n"
        << " 1.Ввести A     2.Ввести B     3.Показать A   4.Показать B\n"
        << " 5.A+B          6.A-B          7.A*B          8.A^T\n"
        << " 9.A*k         10.det(A)      11.Помощь      0.Назад\n";
}

void printTriangleMenu() {
    std::cout << "── Треугольные матрицы (нижние) ──\n"
        << " 1.Ввести T1    2.Ввести T2    3.Показать T1  4.Показать T2\n"
        << " 5.T1+T2        6.T1-T2        7.T1*T2        8.T1*k\n"
        << " 9.Помощь       0.Назад\n";
}

void matrixMenu() {
    Matrix A, B;
    bool hasA = false, hasB = false;

    std::cout << "\n";
    printMatrixMenu();

    int choice = -1;
    do {
        // короткий промпт: какие матрицы уже загружены
        std::cout << "\n[" << (hasA ? "A" : " ") << "][" << (hasB ? "B" : " ") << "]> ";

        if (!(std::cin >> choice)) { clearInput(); std::cout << "Ошибка: ожидалось число\n"; continue; }
        flushLine();

        try {
            switch (choice) {
            case 1:  A = readMatrix("A"); hasA = true; std::cout << "A =\n" << A; break;
            case 2:  B = readMatrix("B"); hasB = true; std::cout << "B =\n" << B; break;

            case 3:
                if (!hasA) throw std::runtime_error("A не задана");
                std::cout << "A =\n" << A; break;
            case 4:
                if (!hasB) throw std::runtime_error("B не задана");
                std::cout << "B =\n" << B; break;

            case 5:
                if (!hasA || !hasB) throw std::runtime_error("Нужны обе матрицы");
                std::cout << "A+B =\n" << (A + B); break;
            case 6:
                if (!hasA || !hasB) throw std::runtime_error("Нужны обе матрицы");
                std::cout << "A-B =\n" << (A - B); break;
            case 7:
                if (!hasA || !hasB) throw std::runtime_error("Нужны обе матрицы");
                std::cout << "A*B =\n" << (A * B); break;

            case 8:
                if (!hasA) throw std::runtime_error("A не задана");
                std::cout << "A^T =\n" << A.transpose(); break;

            case 9: {
                if (!hasA) throw std::runtime_error("A не задана");
                double s = readDouble("k = ");
                std::cout << "A*k =\n" << (A * s); break;
            }

            case 10:
                if (!hasA) throw std::runtime_error("A не задана");
                std::cout << "det(A) = " << std::setprecision(10) << determinant(A) << "\n";
                break;

            case 11: printMatrixMenu(); break;
            case 0:  break;
            default: std::cout << "Неизвестный пункт (11 — помощь)\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    } while (choice != 0);
}

void triangleMenu() {
    TriangleMatrix T1, T2;
    bool hasT1 = false, hasT2 = false;

    std::cout << "\n";
    printTriangleMenu();

    int choice = -1;
    do {
        std::cout << "\n[" << (hasT1 ? "T1" : "  ") << "][" << (hasT2 ? "T2" : "  ") << "]> ";

        if (!(std::cin >> choice)) { clearInput(); std::cout << "Ошибка: ожидалось число\n"; continue; }
        flushLine();

        try {
            switch (choice) {
            case 1:  T1 = readTriangle("T1"); hasT1 = true; std::cout << "T1 =\n" << T1; break;
            case 2:  T2 = readTriangle("T2"); hasT2 = true; std::cout << "T2 =\n" << T2; break;

            case 3:
                if (!hasT1) throw std::runtime_error("T1 не задана");
                std::cout << "T1 =\n" << T1; break;
            case 4:
                if (!hasT2) throw std::runtime_error("T2 не задана");
                std::cout << "T2 =\n" << T2; break;

            case 5:
                if (!hasT1 || !hasT2) throw std::runtime_error("Нужны обе матрицы");
                std::cout << "T1+T2 =\n" << (T1 + T2); break;
            case 6:
                if (!hasT1 || !hasT2) throw std::runtime_error("Нужны обе матрицы");
                std::cout << "T1-T2 =\n" << (T1 - T2); break;
            case 7:
                if (!hasT1 || !hasT2) throw std::runtime_error("Нужны обе матрицы");
                std::cout << "T1*T2 =\n" << (T1 * T2); break;

            case 8: {
                if (!hasT1) throw std::runtime_error("T1 не задана");
                double s = readDouble("k = ");
                std::cout << "T1*k =\n" << (T1 * s); break;
            }

            case 9: printTriangleMenu(); break;
            case 0: break;
            default: std::cout << "Неизвестный пункт (9 — помощь)\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << "\n";
        }
    } while (choice != 0);
}

int main() {
    setlocale(LC_ALL, "Rus");
    std::cout << "── Матричный калькулятор ──\n"
        << " 1.Обычные матрицы\n"
        << " 2.Треугольные матрицы\n"
        << " 0.Выход\n";

    int choice = -1;
    do {
        std::cout << "\n> ";
        if (!(std::cin >> choice)) { clearInput(); std::cout << "Ошибка: ожидалось число\n"; continue; }
        flushLine();
        switch (choice) {
        case 1: matrixMenu();   break;
        case 2: triangleMenu(); break;
        case 0: break;
        default: std::cout << "Неизвестный пункт\n";
        }
    } while (choice != 0);

    std::cout << "\nДо свидания!\n";
    return 0;
}