#include <iostream>
#include <vector>

using namespace std;

// Размер матрицы (количество заданий)
const int N = 6;

// Время переналадки между заданиями
//int time_matrix[N][N] = {
//    {0, 10, 15, 20},
//    {10, 0, 35, 25},
//    {15, 35, 0, 30},
//    {20, 25, 30, 0}
//};

int time_matrix[N][N] = {
    {INT_MAX, 21, 15, 27, 26, 24},
    {15, INT_MAX, 12, 8, 17, 22},
    {9, 18, INT_MAX, 23, 0, 24},
    {5, 10, 26, INT_MAX, 7, 20},
    {23, 17, 15, 27, INT_MAX, 18},
    {24, 27, 11, 3, 26, INT_MAX}
};

// Структура для хранения частичного решения
struct State {
    vector<int> sequence;  // последовательность заданий
    int time;              // время переналадки для текущей последовательности
};

// Функция для вычисления времени переналадки для частичной последовательности
int calculate_time(const vector<int>& seq) {
    int total_time = 0;
    for (int i = 1; i < seq.size(); i++) {
        total_time += time_matrix[seq[i - 1]][seq[i]];
    }
    return total_time;
}
int branch_and_bound() {
    // Начальное решение (пустая последовательность)
    State best_solution;
    best_solution.time = INT_MAX;  // Инициализируем минимальное время как бесконечность

    // Начальная последовательность
    vector<int> initial_sequence = { 0 };  // Начнем с задания 0
    vector<bool> visited(N, false);
    visited[0] = true;

    // Очередь для хранения подзадач (состояний)
    vector<State> queue;
    queue.push_back({ initial_sequence, 0 });

    // Обработка очереди подзадач
    while (!queue.empty()) {
        State current_state = queue.back();
        queue.pop_back();

        // Если последовательность содержит все задания, проверяем решение
        if (current_state.sequence.size() == N) {
            if (current_state.time < best_solution.time) {
                best_solution = current_state;
            }
            continue;
        }

        // Перебираем все возможные следующие задания
        for (int i = 0; i < N; i++) {
            if (!visited[i] && time_matrix[current_state.sequence.back()][i] != INT_MAX) {
                // Копируем текущую последовательность и добавляем новое задание
                vector<int> new_sequence = current_state.sequence;
                new_sequence.push_back(i);

                // Обновляем время переналадки
                int new_time = current_state.time + time_matrix[current_state.sequence.back()][i];

                // Если время переналадки на данный момент уже больше, чем лучшее найденное, отсеиваем
                if (new_time >= best_solution.time) {
                    continue;
                }

                // Помечаем задание как посещенное
                visited[i] = true;

                // Добавляем новое состояние в очередь
                queue.push_back({ new_sequence, new_time });

                // Возвращаем состояние "непосещенности"
                visited[i] = false;
            }
        }
    }

    return best_solution.time;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    int result = branch_and_bound();
    cout << "Минимальное время переналадки: " << result << endl;
    return 0;
}
