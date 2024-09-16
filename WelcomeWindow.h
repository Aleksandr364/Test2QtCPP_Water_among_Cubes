#pragma once

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include <vector>

using namespace std;

class WelcomeWindow : public QWidget
{
private:

    // Метка для отображения приветственного текста.
    QLabel* textLabel = nullptr;
    // Кнопки.
    QPushButton* scanButton = nullptr;
    QPushButton* startButton = nullptr;
    QPushButton* exitButton = nullptr;

public:

    explicit WelcomeWindow(QWidget* parent = nullptr) : QWidget(parent)
    {
        setWindowTitle("Water among Cubes");
        setFixedSize(300, 240);
        // Устанавливаем метку текста.
        textLabel = new QLabel("Hello!\nEnter the input data in input.txt.\nThen press Scan.", this);
        textLabel->setFixedSize(200, 70);
        textLabel->move(45, 20);
        // Устанавливаем кнопку "Сканировать".
        scanButton = new QPushButton("Scan", this);
        scanButton->setFixedSize(130, 30);
        scanButton->move(80, 110);
        // Устанавливаем кнопку "Старт".
        startButton = new QPushButton("Start", this);
        startButton->setFixedSize(130, 30);
        startButton->move(80, 150);
        startButton->setEnabled(false);
        // Устанавливаем кнопку "Выход".
        exitButton = new QPushButton("Exit", this);
        exitButton->setFixedSize(130, 30);
        exitButton->move(80, 190);
        // Устанавливаем коннекты между кнопками и методами.
        connect(scanButton, &QPushButton::clicked, this, &WelcomeWindow::scanClicked);
        connect(startButton, &QPushButton::clicked, this, &WelcomeWindow::startClicked);
        connect(exitButton, &QPushButton::clicked, this, &WelcomeWindow::exitClicked);
    }

private:

    // Для хранения матриц из файла.
    vector<vector<vector<size_t>>> allMatrices;
    
    // Читаем файл input.txt.
    //***********************
    // Считываем t.
    // Функция проверяет, что t введено корректно:
    // строка начинается с положительного целого ненулевого числа без начальных пробелов и без каких-либо символов и пробелов после числа.
    void read_t(char* line, size_t& t)
    {
        char* ptr = line;
        // Проверяем, что строка не начинается с пробела.
        if (isspace((unsigned char)*ptr)) return;
        // Попытка извлечь одно целое положительное число t из строки.
        if (sscanf_s(ptr, "%zu", &t) == 1 && t > 0)
        {
            // Перемещаем указатель дальше числа t.
            while (isdigit((unsigned char)*ptr)) ptr++;
            // Проверяем, что строка заканчивается сразу после числа t.
            if (*ptr == '\0' || *ptr == '\n')
                // Корректная запись числа t.
                return;
        }
        // Если строка заполнена неверно.
        t = 0;
    }
    // Считываем n и m.
    // Функция проверяет, что n и m введено корректно:
    // строка начинается с положительного целого ненулевого числа без начальных пробелов и без каких-либо символов после числа,
    // далее через пробел следует второе такое же число, на этом строка заканчивается.
    // Если числа получены, проверка, что они >= 1 и <= 100.
    void read_n_m(char* line, size_t& n, size_t& m)
    {
        char* ptr = line;
        // Проверяем, что строка не начинается с пробела.
        if (isspace((unsigned char)*ptr)) return;
        // Проверяем, что строка содержит ровно два положительных числа, разделенных одним пробелом.
        if (sscanf_s(line, "%zu %zu", &n, &m) == 2 && n > 0 && m > 0 && n <= 100 && m <= 100)
        {
            // Перемещаем указатель дальше числа n.
            while (isdigit((unsigned char)*ptr)) ptr++;
            // Проходим пробел.
            while (isspace((unsigned char)*ptr)) ptr++;
            // Перемещаем указатель дальше числа m.
            while (isdigit((unsigned char)*ptr)) ptr++;
            // Проверяем, что строка заканчивается сразу после числа m.
            if (*ptr == '\0' || *ptr == '\n')
                // Корректная запись чисел n и m.
                return;
        }
        // Некорректная запись чисел n и m.
        n = 0;
        m = 0;
    }
    // Считываем матрицу.
    // Функция проверяет, что матрица введена корректно:
    // строка начинается с положительного целого ненулевого числа без начальных пробелов и без каких-либо символов после числа,
    // числа разделены пробелами, после последнего числа в строке строка сразу заканчивается.
    // Значение чисел от 1 до 10000.
    bool read_matrix(FILE* file, char* line, size_t& n, size_t& m, vector<vector<size_t>>& matrix)
    {
        matrix.resize(n, vector<size_t>(m));
        char* ptr = nullptr;
            for (size_t i = 0; i < n; i++)
            {
                if (!fgets(line, 1023, file))
                {
                    textLabel->setText("ERROR: failed to read matrix line.");
                    return false;
                }
                ptr = line;
                // Проверяем, что строка не начинается с пробела.
                if (isspace((unsigned char)*ptr))
                {
                    textLabel->setText("ERROR: matrix line starts with a space.");
                    return false;
                }
                for (size_t j = 0; j < m; j++)
                {
                    // Попытка извлечь одно целое положительное число из строки.
                    if (sscanf_s(ptr, "%zu", &matrix[i][j]) == 1 && matrix[i][j] > 0 && matrix[i][j] <= 10000)
                    {
                        // Перемещаем указатель дальше числа matrix[i][j].
                        while (isdigit((unsigned char)*ptr)) ptr++;
                        // Проходим пробел.
                        while (isspace((unsigned char)*ptr)) ptr++;
                    }
                    else
                    {
                        textLabel->setText("ERROR: invalide matrix element.");
                        return false;
                    }
                }
                // Проверяем, что строка заканчивается сразу после последнего числа.
                if (*ptr != '\0' && *ptr != '\n')
                {
                    textLabel->setText("ERROR: invalide matrix line ending.");
                    return false;
                }
            }
        return true;
    }
    // Считываем данные из файла.
    void scanClicked()
    {
        size_t t = 0;
        size_t n = 0;
        size_t m = 0;
        char line[1024];
        vector<vector<size_t>> matrix; // Для записи текущей матрицы из файла.
        FILE* file = nullptr;
        // Открываем файл.
        if (fopen_s(&file, "input.txt", "r"))
        {
            textLabel->setText("ERROR: can not open input.txt.");
            return;
        }
        // Проверяем строку с t.
        if (!fgets(line, 1023, file))
        {
            textLabel->setText("ERROR: failed to read t.");
            fclose(file);
            return;
        }
        // Считываем t.
        read_t(line, t);
        // Проверяем t.
        if (!t)
        {
            textLabel->setText("ERROR: invalide t.");
            fclose(file);
            return;
        }
        // Считываем матрицы.
        for (size_t i = 0; i < t; i++)
        {
            // Проверяем строку n и m.
            if (!fgets(line, 1023, file))
            {
                textLabel->setText("ERROR: failed to read n m.");
                fclose(file);
                return;
            }
            // Считываем n m.
            read_n_m(line, n, m);
            // Проверяем n и m.
            if (!n || !m)
            {
                textLabel->setText("ERROR: invalide n m.");
                fclose(file);
                return;
            }
            // Считываем матрицу.
            if (!read_matrix(file, line, n, m, matrix))
            {
                fclose(file);
                return;
            }
            // Добавляем матрицу.
            allMatrices.push_back(matrix);
            // Очищаем текущую матрицу.
            matrix.clear();
            // Очищаем n и m.
            n = 0;
            m = 0;
            // Проверяем наличие отступа между соседними матрицами, если по условию они должны ещё быть.
            if (i < (t - 1))
            {
                if (!fgets(line, 1023, file))
                {
                    textLabel->setText("ERROR: indention does not exist.");
                    fclose(file);
                    return;
                }
            }
        }
        // Закрыть файл.
        fclose(file);
        // Поменять активные кнопки.
        scanButton->setEnabled(false);
        startButton->setEnabled(true);
        textLabel->setText("The file was read correctly.\nTo get the results press Start.");
    }
    // Запуск решения задачи и вывод результатов в output.txt.
    //********************************************************
    void startClicked()
    {
        FILE* file = nullptr;
        if (fopen_s(&file, "output.txt", "w"))
        {
            textLabel->setText("ERROR: can not open output.txt.");
            return;
        }
        // Объём воды.
        size_t V = 0;
        // Кубоид.
        struct Cuboid
        {
            size_t h = 0; // height.
            enum class CuboidType { unknown, start, waterfall, previous };
            enum class CuboidDirection { unknown, up, right, down, left };
            CuboidType status = CuboidType::unknown; // default.
            CuboidDirection flow = CuboidDirection::unknown; // default.
        };
        // Вектор кубоидов.
        vector<vector<Cuboid>> CuboidsArray;
        // Вектор для хранения указателей на выборочные элементы CuboidsArray.
        vector<Cuboid*> cuboidPtrs;
        // Вспомогательные координаты.
        //*****************************
        // Координаты текущего места воды.
        size_t iCur = 0;
        size_t jCur = 0;
        // Координаты промежуточной точки начала.
        size_t iNewS = 0;
        size_t jNewS = 0;
        // Координаты предыдущего места воды.
        size_t iPrev = 0;
        size_t jPrev = 0;
        // Размер текущей матрицы n m.
        size_t n = 0;
        size_t m = 0; 
        // Основной цикл, проходящий по количеству матриц k.
        // Цикл проходящий по текущей матрице i j.
        for (size_t k = 0, i = 0, j = 0; k < allMatrices.size(); k++)
        {
            n = allMatrices[k].size();
            m = allMatrices[k][i].size();
            // Проверяем количество строк и столбцов текущей матрицы.
            if (n < 3 || m < 3)
            {
                V = 0;
                // Ввывод результата в файл.
                fprintf(file, "%zu\n", V);
                continue;
            }
            // Заполняем вектор пустыми кубоидами.
            CuboidsArray.resize(n, vector<Cuboid>(m));
            // Задаём высоты кубоидов и статус waterfall для граничных.
            for (; i < n; i++)
            {
                for (; j < m; j++)
                {
                    CuboidsArray[i][j].h = allMatrices[k][i][j];
                    // Граничный кубоид = waterfall.
                    if (i == 0 || i == (n - 1) || j == 0 || j == (m - 1))
                        CuboidsArray[i][j].status = Cuboid::CuboidType::waterfall;
                }
                j = 0;
            }
            i = 0;
            // Вычисление объёма воды и вывод результата в файл.
            // **************************************************
            // Проходим массив кубоидов построчно.
            for (; i < n; i++)
            {
                for (; j < m;)
                {
                    // Проверяем статус кубоида.
                    if (CuboidsArray[i][j].status == Cuboid::CuboidType::waterfall)
                    {
                        // Если waterfall, переходим к следующему.
                        j++;
                        continue;
                    }
                    // Если текущий кубоид[i][j] != waterfall, тогда CuboidsArray[i][j] - новая точка начала движения воды.
                    // Устанавливаем вспомогательные координаты в точку начала.
                    iCur = i;
                    jCur = j;
                    iNewS = i;
                    jNewS = j;
                    // Устанавливаем новый старт.
                    CuboidsArray[iNewS][jNewS].status = Cuboid::CuboidType::start;
                    // Логика движения воды.
                    //***********************
                    // Вода ищет путь, пока не попадёт на waterfall.
                    while (1)
                    {
                        // Сканируем клетки вокруг.
                        // При нахождении первой подходящей, вода устремляется туда.
                        // Вода не может течь туда, где выше, и на предыдущую клетку.
                        // В первую очередь вода ищет путь согласно своего flow.
                        //******************************************************
                        if (CuboidsArray[iCur][jCur].flow == Cuboid::CuboidDirection::up && // Если поток вверх.
                            CuboidsArray[iCur][jCur].h >= CuboidsArray[iCur - 1][jCur].h && // Если текущий кубо больше или равен следующему на пути.
                            CuboidsArray[iCur - 1][jCur].status != Cuboid::CuboidType::previous) // Если следующий на пути ещё не был пройден.
                        {
                            // Смещаем координаты кубоида.
                            // Кубоид, где вода была только что, делаем предыдущим.
                            iPrev = iCur;
                            jPrev = jCur;
                            if (CuboidsArray[iPrev][jPrev].status != Cuboid::CuboidType::start) // Проверяем, что Кубоид не был стартовым (на стартовый можно вернуться).
                            {
                                CuboidsArray[iPrev][jPrev].status = Cuboid::CuboidType::previous; // Устанавливаем статус предыдущего.
                                cuboidPtrs.push_back(&CuboidsArray[iPrev][jPrev]); // Сохраняем указатель на кубоид со статусом previous.
                            }
                            CuboidsArray[iPrev][jPrev].flow = Cuboid::CuboidDirection::unknown; // Сбрасываем направление потока для предыдущего.
                            iCur -= 1; // Находим новые координаты для текущего кубоида.
                            if (CuboidsArray[iCur][jCur].status == Cuboid::CuboidType::waterfall) break; // Если текущий кубоид оказался waterfall, выходим из цикла поиска пути.
                            CuboidsArray[iCur][jCur].flow = Cuboid::CuboidDirection::up; // Если текущий !waterfall устанавливаем для него направление flow, согласно направлению смещения.
                            continue; // Продолжаем поиск пути.
                        }
                        if (CuboidsArray[iCur][jCur].flow == Cuboid::CuboidDirection::right &&
                            CuboidsArray[iCur][jCur].h >= CuboidsArray[iCur][jCur + 1].h &&
                            CuboidsArray[iCur][jCur + 1].status != Cuboid::CuboidType::previous)
                        {
                            iPrev = iCur;
                            jPrev = jCur;
                            if (CuboidsArray[iPrev][jPrev].status != Cuboid::CuboidType::start)
                            {
                                CuboidsArray[iPrev][jPrev].status = Cuboid::CuboidType::previous;
                                cuboidPtrs.push_back(&CuboidsArray[iPrev][jPrev]);
                            }
                            CuboidsArray[iPrev][jPrev].flow = Cuboid::CuboidDirection::unknown;
                            jCur += 1;
                            if (CuboidsArray[iCur][jCur].status == Cuboid::CuboidType::waterfall) break;
                            CuboidsArray[iCur][jCur].flow = Cuboid::CuboidDirection::right;
                            continue;
                        }
                        if (CuboidsArray[iCur][jCur].flow == Cuboid::CuboidDirection::down &&
                            CuboidsArray[iCur][jCur].h >= CuboidsArray[iCur + 1][jCur].h &&
                            CuboidsArray[iCur + 1][jCur].status != Cuboid::CuboidType::previous)
                        {
                            iPrev = iCur;
                            jPrev = jCur;
                            if (CuboidsArray[iPrev][jPrev].status != Cuboid::CuboidType::start)
                            {
                                CuboidsArray[iPrev][jPrev].status = Cuboid::CuboidType::previous;
                                cuboidPtrs.push_back(&CuboidsArray[iPrev][jPrev]);
                            }
                            CuboidsArray[iPrev][jPrev].flow = Cuboid::CuboidDirection::unknown;
                            iCur += 1;
                            if (CuboidsArray[iCur][jCur].status == Cuboid::CuboidType::waterfall) break;
                            CuboidsArray[iCur][jCur].flow = Cuboid::CuboidDirection::down;
                            continue;
                        }
                        if (CuboidsArray[iCur][jCur].flow == Cuboid::CuboidDirection::left &&
                            CuboidsArray[iCur][jCur].h >= CuboidsArray[iCur][jCur - 1].h &&
                            CuboidsArray[iCur][jCur - 1].status != Cuboid::CuboidType::previous)
                        {
                            iPrev = iCur;
                            jPrev = jCur;
                            if (CuboidsArray[iPrev][jPrev].status != Cuboid::CuboidType::start)
                            {
                                CuboidsArray[iPrev][jPrev].status = Cuboid::CuboidType::previous;
                                cuboidPtrs.push_back(&CuboidsArray[iPrev][jPrev]);
                            }
                            CuboidsArray[iPrev][jPrev].flow = Cuboid::CuboidDirection::unknown;
                            jCur -= 1;
                            if (CuboidsArray[iCur][jCur].status == Cuboid::CuboidType::waterfall) break;
                            CuboidsArray[iCur][jCur].flow = Cuboid::CuboidDirection::left;
                            continue;
                        }
                        // Если flow == unknown или вода не может течь согласно flow.
                        //************************************************************
                        if (CuboidsArray[iCur][jCur].h >= CuboidsArray[iCur - 1][jCur].h &&
                            CuboidsArray[iCur - 1][jCur].status != Cuboid::CuboidType::previous)
                        {
                            iPrev = iCur;
                            jPrev = jCur;
                            if (CuboidsArray[iPrev][jPrev].status != Cuboid::CuboidType::start)
                            {
                                CuboidsArray[iPrev][jPrev].status = Cuboid::CuboidType::previous;
                                cuboidPtrs.push_back(&CuboidsArray[iPrev][jPrev]);
                            }
                            CuboidsArray[iPrev][jPrev].flow = Cuboid::CuboidDirection::unknown;
                            iCur -= 1;
                            if (CuboidsArray[iCur][jCur].status == Cuboid::CuboidType::waterfall) break;
                            CuboidsArray[iCur][jCur].flow = Cuboid::CuboidDirection::up;
                            continue;
                        }
                        if (CuboidsArray[iCur][jCur].h >= CuboidsArray[iCur][jCur + 1].h &&
                            CuboidsArray[iCur][jCur + 1].status != Cuboid::CuboidType::previous)
                        {
                            iPrev = iCur;
                            jPrev = jCur;
                            if (CuboidsArray[iPrev][jPrev].status != Cuboid::CuboidType::start)
                            {
                                CuboidsArray[iPrev][jPrev].status = Cuboid::CuboidType::previous;
                                cuboidPtrs.push_back(&CuboidsArray[iPrev][jPrev]);
                            }
                            CuboidsArray[iPrev][jPrev].flow = Cuboid::CuboidDirection::unknown;
                            jCur += 1;
                            if (CuboidsArray[iCur][jCur].status == Cuboid::CuboidType::waterfall) break;
                            CuboidsArray[iCur][jCur].flow = Cuboid::CuboidDirection::right;
                            continue;
                        }
                        if (CuboidsArray[iCur][jCur].h >= CuboidsArray[iCur + 1][jCur].h &&
                            CuboidsArray[iCur + 1][jCur].status != Cuboid::CuboidType::previous)
                        {
                            iPrev = iCur;
                            jPrev = jCur;
                            if (CuboidsArray[iPrev][jPrev].status != Cuboid::CuboidType::start)
                            {
                                CuboidsArray[iPrev][jPrev].status = Cuboid::CuboidType::previous;
                                cuboidPtrs.push_back(&CuboidsArray[iPrev][jPrev]);
                            }
                            CuboidsArray[iPrev][jPrev].flow = Cuboid::CuboidDirection::unknown;
                            iCur += 1;
                            if (CuboidsArray[iCur][jCur].status == Cuboid::CuboidType::waterfall) break;
                            CuboidsArray[iCur][jCur].flow = Cuboid::CuboidDirection::down;
                            continue;
                        }
                        if (CuboidsArray[iCur][jCur].h >= CuboidsArray[iCur][jCur - 1].h &&
                            CuboidsArray[iCur][jCur - 1].status != Cuboid::CuboidType::previous)
                        {
                            iPrev = iCur;
                            jPrev = jCur;
                            if (CuboidsArray[iPrev][jPrev].status != Cuboid::CuboidType::start)
                            {
                                CuboidsArray[iPrev][jPrev].status = Cuboid::CuboidType::previous;
                                cuboidPtrs.push_back(&CuboidsArray[iPrev][jPrev]);
                            }
                            CuboidsArray[iPrev][jPrev].flow = Cuboid::CuboidDirection::unknown;
                            jCur -= 1;
                            if (CuboidsArray[iCur][jCur].status == Cuboid::CuboidType::waterfall) break;
                            CuboidsArray[iCur][jCur].flow = Cuboid::CuboidDirection::left;
                            continue;
                        }
                        // Вода оказалась в тупике.
                        // Сбрасываем все previous на unknown.
                        for (size_t l = 0; l < cuboidPtrs.size(); l++)
                            cuboidPtrs[l]->status = Cuboid::CuboidType::unknown;
                        // Обнуляем вектор cuboidPtrs.
                        cuboidPtrs.clear();
                        // Сбрасываем текущий flow на unknown.
                        CuboidsArray[iCur][jCur].flow = Cuboid::CuboidDirection::unknown;
                        // Увеличиваем высоту тупикового кубоида на 1.
                        CuboidsArray[iCur][jCur].h++;
                        // Увеличиваем объём воды на 1.
                        V++;
                        // Обновляем координаты старта. Тупиковый кубоид становится новым стартом.
                        CuboidsArray[iNewS][jNewS].status = Cuboid::CuboidType::unknown;
                        iNewS = iCur;
                        jNewS = jCur;
                        CuboidsArray[iNewS][jNewS].status = Cuboid::CuboidType::start;
                    }
                    // Вода вытекла за пределы поля (попала на waterfall).
                    // Сбрасываем все previous на unknown.
                    for (size_t l = 0; l < cuboidPtrs.size(); l++)
                        cuboidPtrs[l]->status = Cuboid::CuboidType::unknown;
                    // Обнуляем вектор cuboidPtrs.
                    cuboidPtrs.clear();
                    // Сбрасываем текущий flow на unknown.
                    CuboidsArray[iCur][jCur].flow = Cuboid::CuboidDirection::unknown;
                    // Стартовый кубоид становится waterfall.
                    CuboidsArray[iNewS][jNewS].status = Cuboid::CuboidType::waterfall;
                    // Если координаты стартового кубоида,
                    // путь с которого привёл на waterfall,
                    // равны координатам выбранного i j,
                    // смещаемся на следующий (увеличиваем j на 1).
                    // Если нет, - выбранный i j не меняется, снова начинаем поиск пути с него.
                    if ( i != iNewS || j != jNewS) continue;
                    j++;
                }
                j = 0;
            }
            i = 0;
            // Результат записывается в файл.
            fprintf(file, "%zu\n", V);
            // Обнуляем вектор кубоидов.
            CuboidsArray.clear();
            // Обнуляем переменные.
            V = 0;
            iCur = 0;
            jCur = 0;
            iNewS = 0;
            jNewS = 0;
            iPrev = 0;
            jPrev = 0;
            n = 0;
            m = 0;
        }
        fclose(file);
        startButton->setEnabled(false);
        textLabel->setText("The results are saved in output.txt.");
    }
    // Выход из программы.
    //********************
    void exitClicked() { QApplication::quit(); }
};