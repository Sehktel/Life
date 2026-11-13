using System;
using System.Text;
using System.Threading;

// ================= Life.Core (без UI) =================

public sealed class Field
{
    private readonly bool[,] _cells;

    public int Height { get; }
    public int Width { get; }
    public bool IsToroidal { get; }

    public Field(int height, int width, bool isToroidal = true)
    {
        if (height <= 0) throw new ArgumentOutOfRangeException(nameof(height));
        if (width <= 0) throw new ArgumentOutOfRangeException(nameof(width));

        Height = height;
        Width = width;
        IsToroidal = isToroidal;
        _cells = new bool[height, width];
    }

    // Снаружи только чтение. Мутация — через Game.
    public bool this[int y, int x] => _cells[y, x];

    internal void SetCell(int y, int x, bool alive) => _cells[y, x] = alive;

    public int CountNeighbors(int y, int x)
    {
        int count = 0;

        for (int dy = -1; dy <= 1; dy++)
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dy == 0 && dx == 0) continue;

            int ny = y + dy;
            int nx = x + dx;

            if (IsToroidal)
            {
                ny = (ny + Height) % Height;
                nx = (nx + Width) % Width;
            }
            else if (ny < 0 || ny >= Height || nx < 0 || nx >= Width)
            {
                continue;
            }

            if (_cells[ny, nx]) count++;
        }

        return count;
    }

    public int AliveCount()
    {
        int count = 0;
        for (int y = 0; y < Height; y++)
        for (int x = 0; x < Width; x++)
            if (_cells[y, x]) count++;
        return count;
    }
}

public static class LifeRules
{
    // Чистая функция: один и тот же вход -> один и тот же выход.
    public static bool NextState(bool alive, int neighbors)
        => alive ? neighbors is 2 or 3 : neighbors is 3;
}

public sealed class LifeGame
{
    private readonly Random _random;

    public Field Field { get; private set; }
    public int Generation { get; private set; }

    public LifeGame(int height, int width, Random random, bool isToroidal = true)
    {
        _random = random;
        Field = new Field(height, width, isToroidal);
    }

    public void SetCell(int y, int x, bool alive) => Field.SetCell(y, x, alive);

    public void Randomize(double chance)
    {
        for (int y = 0; y < Field.Height; y++)
        for (int x = 0; x < Field.Width; x++)
            Field.SetCell(y, x, _random.NextDouble() < chance);

        Generation = 0;
    }

    public void PlaceGlider(int y, int x)
    {
        SetCell(y,     x + 1, true);
        SetCell(y + 1, x + 2, true);
        SetCell(y + 2, x,     true);
        SetCell(y + 2, x + 1, true);
        SetCell(y + 2, x + 2, true);
    }

    public void Step()
    {
        var next = new Field(Field.Height, Field.Width, Field.IsToroidal);

        for (int y = 0; y < Field.Height; y++)
        for (int x = 0; x < Field.Width; x++)
        {
            int neighbors = Field.CountNeighbors(y, x);
            bool alive = Field[y, x];
            next.SetCell(y, x, LifeRules.NextState(alive, neighbors));
        }

        Field = next; // двойная буферизация сохраняется
        Generation++;
    }
}

// ================= Life.Console (UI) =================

public sealed class ConsoleRenderer
{
    public void Draw(LifeGame game)
    {
        Field field = game.Field;
        Console.SetCursorPosition(0, 0);

        var sb = new StringBuilder();
        for (int y = 0; y < field.Height; y++)
        {
            sb.Clear();
            for (int x = 0; x < field.Width; x++)
                sb.Append(field[y, x] ? '█' : ' ');
            Console.WriteLine(sb.ToString());
        }

        Console.WriteLine($"Поколение: {game.Generation}  Живых: {field.AliveCount()}  (клавиша — выход)");
    }
}

internal static class Program
{
    private static void Main()
    {
        Console.OutputEncoding = Encoding.UTF8;
        Console.CursorVisible = false;
        Console.Clear();

        var game = new LifeGame(25, 60, Random.Shared, isToroidal: true);
        game.Randomize(0.2);
        // game.PlaceGlider(5, 5); // вместо случайного поля

        var renderer = new ConsoleRenderer();

        while (true)
        {
            renderer.Draw(game);
            if (Console.KeyAvailable) break;

            game.Step();
            Thread.Sleep(120);
        }

        Console.CursorVisible = true;
        Console.Clear();
        Console.WriteLine("Игра завершена.");
    }
}