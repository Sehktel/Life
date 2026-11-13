using System;
using System.Text;
using System.Threading;

class Program
{
    // --- Параметры (позже станут настройками класса) ---
    const int Width = 60;
    const int Height = 25;
    const int DelayMs = 120;
    const double FillChance = 0.2;

    static readonly Random Rnd = new Random();

    static void Main()
    {
        Console.OutputEncoding = Encoding.UTF8; // для символа █
        Console.CursorVisible = false;
        Console.Clear();

        bool[,] field = CreateField(Height, Width);
        RandomFill(field, FillChance);
        // PlaceGlider(field, 5, 5); // вместо случайного заполнения — проверка правил

        int generation = 0;

        while (true)
        {
            Draw(field, generation);

            if (Console.KeyAvailable) break; // выход по любой клавише

            field = NextGeneration(field);
            generation++;
            Thread.Sleep(DelayMs);
        }

        Console.CursorVisible = true;
        Console.Clear();
        Console.WriteLine("Игра завершена.");
    }

    // --- Создание и заполнение ---

    static bool[,] CreateField(int height, int width)
    {
        return new bool[height, width];
    }

    static void RandomFill(bool[,] field, double chance)
    {
        int h = field.GetLength(0);
        int w = field.GetLength(1);
        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++)
                field[y, x] = Rnd.NextDouble() < chance;
    }

    static void PlaceGlider(bool[,] field, int y, int x)
    {
        field[y,     x + 1] = true;
        field[y + 1, x + 2] = true;
        field[y + 2, x    ] = true;
        field[y + 2, x + 1] = true;
        field[y + 2, x + 2] = true;
    }

    // --- Правила игры ---

    static int CountNeighbors(bool[,] field, int y, int x)
    {
        int h = field.GetLength(0);
        int w = field.GetLength(1);
        int count = 0;

        for (int dy = -1; dy <= 1; dy++)
            for (int dx = -1; dx <= 1; dx++)
            {
                if (dy == 0 && dx == 0) continue;

                // поле-тор: края «склеены», индексы заворачиваются
                int ny = (y + dy + h) % h;
                int nx = (x + dx + w) % w;

                if (field[ny, nx]) count++;
            }
        return count;
    }

    static bool[,] NextGeneration(bool[,] field)
    {
        int h = field.GetLength(0);
        int w = field.GetLength(1);
        bool[,] next = CreateField(h, w); // важно: новый массив, не меняем старый!

        for (int y = 0; y < h; y++)
            for (int x = 0; x < w; x++)
            {
                int n = CountNeighbors(field, y, x);
                // живая клетка: выживает при 2 или 3 соседях
                // мёртвая клетка: оживает ровно при 3 соседях
                next[y, x] = field[y, x] ? (n == 2 || n == 3) : (n == 3);
            }
        return next;
    }

    // --- Отрисовка ---

    static void Draw(bool[,] field, int generation)
    {
        int h = field.GetLength(0);
        int w = field.GetLength(1);

        Console.SetCursorPosition(0, 0); // без Clear() — нет мерцания

        var sb = new StringBuilder();
        for (int y = 0; y < h; y++)
        {
            sb.Clear();
            for (int x = 0; x < w; x++)
                sb.Append(field[y, x] ? '█' : ' ');
            Console.WriteLine(sb.ToString());
        }
        Console.WriteLine($"Поколение: {generation}   (любая клавиша — выход)");
    }
}