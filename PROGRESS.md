# Progress — Game of Life (C#)

**Цель:** процедурный консольный прототип → ООП-ядро → WPF-приложение.
**Схема разработки:** Model и ViewModel ведутся на Linux, View — в Visual Studio на Windows.
**Обязательные условия:** конвейер сборки (CI), автотесты, автособираемая документация.
**Стек:** .NET 10 (LTS), xUnit + coverlet, DocFX, GitHub Actions, WPF + CommunityToolkit.Mvvm.
**Обновлено:** 2026-11-13

---

## Легенда
`[x]` — сделано · `[ ]` — не сделано · `[~]` — в работе

## Сводка

| # | Этап | Статус | Прогресс |
|---|------|--------|----------|
| 0 | Процедурный прототип (консоль) | [~] | ~70% |
| 1 | Гигиена репозитория, структура, slnf | [ ] | 0% |
| 2 | ООП: Life.Core + Life.ViewModels | [ ] | 0% |
| 3 | Автотесты + архитектурные тесты | [ ] | 0% |
| 4 | Конвейер сборки (CI/CD) | [ ] | 0% |
| 5 | Автодокументация | [ ] | 0% |
| 6 | WPF View (Windows, Visual Studio) | [ ] | 0% |
| 7 | Полировка и релиз | [ ] | 0% |

---

## Архитектура и границы

```text
Life.Wpf (View, Windows)
  │  bindings / commands
  ▼
Life.ViewModels (адаптер, net10.0)
  │  вызовы методов / состояние
  ▼
Life.Core (Model, net10.0)

Life.Console (headless-клиент) → Life.Core
```

- **Model (`Life.Core`)** — предметная область: поле, правила B3/S23, `Game.Step()`, `Reset()`, `SetCell()`, поколение, число живых, RLE-парсер. Не «обработчик сигналов UI», не знает о кнопках, командах, формах и консоли.
- **ViewModel (`Life.ViewModels`)** — двусторонний адаптер: принимает команды от View (`ICommand`), вызывает Model, отдаёт состояние обратно через `INotifyPropertyChanged`. Не содержит правил игры.
- **View (`Life.Wpf`)** — XAML + биндинги + рендер. В code-behind только чисто UI-вещи.
- Направление зависимостей строго: `View → ViewModel → Model`. `Console → Core`.

| Проект | TFM | Роль | Где разрабатывается / работает |
|---|---|---|---|
| `Life.Core` | `net10.0` | Model | Linux / Windows / macOS |
| `Life.ViewModels` | `net10.0` | ViewModel | Linux / Windows / macOS |
| `Life.Console` | `net10.0` | headless-клиент | Linux / Windows / macOS |
| `Life.Wpf` | `net10.0-windows`, `UseWPF=true` | View | только Windows, Visual Studio |
| `tests/*` | `net10.0` | контракт между сторонами | Linux CI |
| `benchmarks/*` | `net10.0` | производительность | Linux |

**Запрещено в `Life.Core` и `Life.ViewModels`:**
`System.Windows.Controls`, `System.Windows.Media`, `System.Windows.Threading`, WPF-пакеты, `WriteableBitmap`, `Microsoft.Win32`-диалоги, `Console` (для Core; желательно и для VM).

**Разрешено:**
`CommunityToolkit.Mvvm`, `System.ObjectModel`, `Microsoft.Extensions.Logging.Abstractions`, `PeriodicTimer`, обычные интерфейсы сервисов.

**Платформенные абстракции (реализуются во View/клиенте):**
```csharp
IDialogService
IFileDialogService
IGameClock      // таймер через абстракцию или PeriodicTimer, не DispatcherTimer
```

**Workflow:**
- Linux: dotnet CLI / Rider / VS Code + C# Dev Kit. Разработка Core, ViewModels, Console, тестов, бенчмарков, документации.
- Windows: Visual Studio, полное решение `Life.sln`, startup project `Life.Wpf`.
- Один git-remote, monorepo. Не дроби на два репозитория без нужды; NuGet-пакетизация Core добавит версионирование и боль.
- CI — «место встречи»: Linux доказывает Core/ViewModel, Windows доказывает View.
- На Linux не собирай `Life.sln` целиком: споткнётся о `net10.0-windows`. Используй `Life.Linux.slnf`. `EnableWindowsTargeting` иногда позволяет кросс-сборку, но запустить WPF на Linux всё равно нельзя.

---

## Этап 0 — Процедурный прототип (консоль)

- [x] Поле `bool[,]`, двойная буферизация (новое поколение — новый массив)
- [x] `CountNeighbors` с тороидальным заворачиванием краёв
- [x] `NextGeneration` по правилам B3/S23
- [x] Отрисовка без мерцания (`SetCursorPosition` вместо `Clear`)
- [x] Выход по нажатию клавиши, задержка кадра
- [ ] `PlaceGlider` и другие фигуры для ручной проверки правил
- [ ] `CountAlive` + автоостановка при пустом/стабилизировавшемся поле
- [ ] Возраст клетки (`int[,]`) и раскраска по возрасту
- [ ] Пауза / шаг вперёд по клавише

## Этап 1 — Гигиена репозитория, структура, slnf

- [ ] `git init`, `dotnet new gitignore`, первый коммит, один remote
- [ ] `.editorconfig` + анализаторы (`EnableNETAnalyzers`, `AnalysisLevel=latest`)
- [ ] `Directory.Build.props`: `Nullable=enable`, `TreatWarningsAsErrors=true`, `GenerateDocumentationFile=true`, `ImplicitUsings=enable`
- [ ] Проекты и решение по дереву ниже
- [ ] `Life.Linux.slnf` — фильтр без WPF
- [ ] `README.md` (что это, как запустить на Linux/Windows, скриншот/gif), `LICENSE`, `CHANGELOG.md`
- [ ] Conventional Commits, защита `main` (PR + зелёный CI)

**Целевая структура:**
```text
Life/
  Life.sln              # полное, для VS на Windows
  Life.Linux.slnf       # фильтр без WPF, для Linux
  Directory.Build.props
  .editorconfig
  progress.md
  src/
    Life.Core/          # net10.0
    Life.ViewModels/    # net10.0
    Life.Console/       # net10.0
    Life.Wpf/           # net10.0-windows, UseWPF=true
  tests/
    Life.Core.Tests/
    Life.ViewModels.Tests/
    Life.Architecture.Tests/
  benchmarks/
    Life.Benchmarks/
  docs/
    docfx.json
    articles/
  .github/workflows/
    ci.yml
    release.yml
```

**`Life.Linux.slnf`:**
```json
{
  "solution": {
    "path": "Life.sln",
    "projects": [
      "src/Life.Core/Life.Core.csproj",
      "src/Life.ViewModels/Life.ViewModels.csproj",
      "src/Life.Console/Life.Console.csproj",
      "tests/Life.Core.Tests/Life.Core.Tests.csproj",
      "tests/Life.ViewModels.Tests/Life.ViewModels.Tests.csproj",
      "tests/Life.Architecture.Tests/Life.Architecture.Tests.csproj",
      "benchmarks/Life.Benchmarks/Life.Benchmarks.csproj"
    ]
  }
}
```

## Этап 2 — ООП: Life.Core + Life.ViewModels

> Порядок важен: сначала минимальные тесты-страховка на текущую логику (этап 3), потом рефакторинг. Временно сделай методы `public`/`internal` + `InternalsVisibleTo`, чтобы тесты могли дотянуться до процедурного кода.

**Life.Core:**
- [ ] `Field` — инкапсуляция массива: `Width`, `Height`, индексатор, `IsToroidal`
- [ ] `Field.CountNeighbors(y, x)` — переезд из статической функции
- [ ] `Game` — `Step()`, `Reset()`, `SetCell(x, y, alive)`, `Generation`, `AliveCount`
- [ ] `LifeRules` — чистая логика B3/S23, без UI и консоли
- [ ] `RleParser` — чистый парсер строк, без файлового I/O
- [ ] `Random` передаётся извне / seed для воспроизводимости, никаких статических состояний
- [ ] Неизменяемость: `Step()` возвращает новый `Field` или явный swap двух буферов
- [ ] События/снимки состояния для подписки ViewModel, без знания о UI

**Life.ViewModels:**
- [ ] `GameViewModel`: `IsRunning`, `Generation`, `AliveCount`, `Speed`, `StartCommand`, `PauseCommand`, `StepCommand`, `ResetCommand`
- [ ] `CommunityToolkit.Mvvm`: `ObservableObject`, `RelayCommand`
- [ ] `IGameClock`, `IDialogService`, `IFileDialogService` — абстракции, не WPF-реализации
- [ ] Таймер через `PeriodicTimer`/абстракцию, не `DispatcherTimer`
- [ ] ViewModel не содержит правил игры, только оркестрацию и состояние для View
- [ ] Проверка: в Core/ViewModels нет WPF-типов и `Console`

## Этап 3 — Автотесты + архитектурные тесты (обязательно)

**Инструменты:** xUnit, coverlet, ReportGenerator, NetArchTest (или reflection-тесты).

**Страховка (до рефакторинга):**
- [ ] Block (2×2) — не меняется за 10 поколений
- [ ] Blinker — период 2, возвращается в исходное состояние
- [ ] Glider — через 4 поколения смещён на (+1, +1)
- [ ] Пустое поле остаётся пустым
- [ ] Одинокая клетка умирает
- [ ] Клетка с 4+ соседями умирает
- [ ] Мёртвая клетка с ровно 3 соседями оживает
- [ ] `Step()` не мутирует входное поле
- [ ] Тороидальный переход через край
- [ ] Одинаковый seed → одинаковая последовательность поколений

**Life.Core.Tests:**
- [ ] Теории `[Theory]/[InlineData]` на всю матрицу правил (0..8 соседей × жива/мертва)
- [ ] Граничные случаи: 1×1, 2×2, 3×3, неквадратное поле
- [ ] «Golden»-тест: N поколений от фиксированного старта → эталонный снимок
- [ ] RLE-парсер: корректные и битые строки
- [ ] Порог покрытия: **≥ 80% для `Life.Core`**

**Life.ViewModels.Tests (Linux, headless):**
- [ ] Fake `IGameClock`, `IDialogService`, `IFileDialogService`
- [ ] Команды меняют состояние и дергают Model
- [ ] `PropertyChanged` приходит на нужные свойства
- [ ] Pause/Step/Speed/Reset работают без WPF
- [ ] Порог покрытия: **≥ 80% для `Life.ViewModels`**

**Life.Architecture.Tests:**
- [ ] `Life.Core` не ссылается на WPF, `System.Windows.*`, `Console`
- [ ] `Life.ViewModels` не ссылается на WPF
- [ ] Зависимости только `View → ViewModel → Model`, `Console → Core`
- [ ] Падение CI при нарушении границ

## Этап 4 — Конвейер сборки (CI/CD)

- [ ] `ci.yml`: Linux-фильтр (restore → format → build → test + coverage) + Windows WPF + docs
- [ ] Матрица раннеров: **ubuntu** для `Life.Linux.slnf`, **windows** для `Life.sln`/WPF
- [ ] Отчёт покрытия как артефакт + бейдж в README
- [ ] PR-гейт: сборка + тесты + архитектурные тесты + покрытие обязательны для мержа
- [ ] `release.yml` по тегу `v*`: `dotnet publish src/Life.Wpf -c Release -r win-x64 --self-contained -p:PublishSingleFile=true` → zip → GitHub Release
- [ ] Кэш NuGet
- [ ] Версионирование: SemVer + MinVer/GitVersion, `CHANGELOG.md`

**Черновик `ci.yml`:**
```yaml
name: CI
on: [push, pull_request]
jobs:
  linux:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-dotnet@v4
        with: { dotnet-version: '10.0.x' }
      - run: dotnet restore Life.Linux.slnf
      - run: dotnet format Life.Linux.slnf --verify-no-changes
      - run: dotnet build Life.Linux.slnf -c Release -warnaserror
      - run: dotnet test Life.Linux.slnf -c Release --no-build --collect:"XPlat Code Coverage"
      - uses: actions/upload-artifact@v4
        with: { name: coverage, path: '**/coverage.cobertura.xml' }
  wpf:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-dotnet@v4
        with: { dotnet-version: '10.0.x' }
      - run: dotnet build Life.sln -c Release -warnaserror
  docs:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: actions/setup-dotnet@v4
        with: { dotnet-version: '10.0.x' }
      - run: dotnet tool update -g docfx
      - run: docfx docs/docfx.json
      - uses: actions/upload-pages-artifact@v3
        with: { path: docs/_site }
  deploy-pages:
    if: github.ref == 'refs/heads/main'
    needs: docs
    runs-on: ubuntu-latest
    permissions: { pages: write, id-token: write }
    environment: github-pages
    steps:
      - uses: actions/deploy-pages@v4
```

## Этап 5 — Автодокументация

- [ ] XML-комментарии `///` на всех публичных типах и членах
- [ ] `GenerateDocumentationFile=true` + CS1591 как ошибка (нет комментария — нет сборки)
- [ ] **DocFX** вместо Doxygen: нативная поддержка XML-документации .NET
- [ ] `docfx metadata` + `docfx build`, сайт в `docs/_site`
- [ ] Статьи: правила игры, архитектура и границы, workflow Linux/Windows, как запустить, как контрибьютить
- [ ] Автопубликация на GitHub Pages из CI
- [ ] Бейджи в README: build, coverage, docs
- [ ] ADR — короткие записи «почему так решили»

## Этап 6 — WPF View (только после ООП, Windows, Visual Studio)

- [ ] `Life.Wpf`, `net10.0-windows`, `<UseWPF>true</UseWPF>`
- [ ] `Life.Core` и `Life.ViewModels` подключаются как есть, правила не переписываются
- [ ] MVVM: `DataContext = GameViewModel`, биндинги и команды, минимум code-behind
- [ ] Рендер через `WriteableBitmap`, не `Canvas` + по `Rectangle` на клетку
- [ ] Реализации `IGameClock` (`DispatcherTimer`), `IDialogService`, `IFileDialogService` только здесь
- [ ] Управление: Start / Pause / Step / Reset / ползунок скорости
- [ ] Рисование и стирание клеток мышью, зум и панорама
- [ ] Открытие/сохранение состояния и фигур через `IFileDialogService`
- [ ] Отображение поколения, числа живых клеток, FPS
- [ ] Настройки (размер поля, тор/границы, цвета) + сохранение между запусками
- [ ] UI-тесты (FlaUI) — опционально, только Windows, не блокер

## Этап 7 — Полировка и релиз

- [ ] Библиотека фигур RLE + файловый I/O через абстракции
- [ ] Сохранение/загрузка JSON (`System.Text.Json`)
- [ ] BenchmarkDotNet: замер `Step()` на 1000×1000 **до** оптимизаций
- [ ] Оптимизация при необходимости: плоский массив / `Span<T>` / bitset / обмен буферов
- [ ] Логирование (`Microsoft.Extensions.Logging`) и внятная обработка ошибок I/O
- [ ] Публикация: WPF self-contained single-file; Console cross-platform publish
- [ ] При желании установщик (MSIX / Inno Setup)
- [ ] Скриншоты/gif в README, финальный `CHANGELOG`, тег `v1.0.0`

---

## Что было забыто (и добавлено в план)

1. **Разделение Core / ViewModels / View** — ключ к Linux-разработке, тестам и WPF.
2. **ViewModel тоже на Linux** — если без WPF-типов, тестируется headless.
3. **`Life.Linux.slnf`** — чтобы не собирать WPF на Linux.
4. **Архитектурные тесты** — запрет WPF/Console в Core/VM, иначе границы потекут.
5. **Тесты до рефакторинга в ООП** — страховка правил.
6. **Git, .gitignore, .editorconfig, анализаторы, warnings-as-errors** — база.
7. **Порог покрытия и PR-гейт** — «тесты есть» ≠ «тесты работают».
8. **WPF только Windows** → CI-матрица, иначе конвейер красный с первого дня.
9. **Детерминизм (seed)** — иначе тесты флакают.
10. **`GenerateDocumentationFile` сразу** — иначе документация из пустоты.
11. **`WriteableBitmap` в WPF** — архитектурное решение для производительности.
12. **RLE** — совместимость с экосистемой «Жизни».
13. **Бенчмарки до оптимизации** — не оптимизировать наугад.
14. **Версионирование, CHANGELOG, релизные артефакты, лицензия, README, логирование, ADR.**
15. **Один monorepo, а не два репозитория** — меньше боли с версиями пакетов.

## Definition of Done (для каждого этапа)

- [ ] Сборка без предупреждений (`-warnaserror`)
- [ ] `dotnet format` проходит
- [ ] Все тесты зелёные, покрытие `Life.Core` и `Life.ViewModels` ≥ 80%
- [ ] Архитектурные тесты зелёные
- [ ] Документация собирается без предупреждений
- [ ] CI зелёный на PR: Linux-фильтр + Windows WPF
- [ ] В `Life.Core`/`Life.ViewModels` нет WPF и `Console`
- [ ] `progress.md` и `CHANGELOG.md` обновлены

## Метрики

| Метрика | Сейчас | Цель |
|---|---|---|
| Покрытие `Life.Core` | 0% | ≥ 80% |
| Покрытие `Life.ViewModels` | 0% | ≥ 80% |
| Предупреждений сборки | ? | 0 |
| Время `Step()` 1000×1000 | не замерено | < 50 мс |
| Время CI | — | < 5 мин |

## Журнал решений (ADR)

| Дата | Решение | Причина |
|---|---|---|
| 2026-11-13 | .NET 10 LTS | долгая поддержка, актуальный C# |
| 2026-11-13 | Monorepo + `Life.Linux.slnf` | наглядное разделение Linux/Windows без NuGet-боли |
| 2026-11-13 | `Life.Core` и `Life.ViewModels` на Linux | кроссплатформенный .NET, headless-тесты |
| 2026-11-13 | WPF только как View, Windows | WPF Windows-only, XAML не делает его кроссплатформенным |
| 2026-11-13 | Архитектурные тесты | границы Core/VM/View должны соблюдаться автоматически |
| 2026-11-13 | xUnit + coverlet | стандарт де-факто для .NET |
| 2026-11-13 | DocFX вместо Doxygen | нативная поддержка XML-документации .NET |
| 2026-11-13 | CommunityToolkit.Mvvm | минимум бойлерплейта, кроссплатформенный |
| 2026-11-13 | `WriteableBitmap` | производительный рендер тысяч клеток |
| 2026-11-13 | Двойная буферизация поля | корректность правил, отсутствие мутаций входа |

## Шпаргалка команд

**Linux:**
```bash
dotnet build Life.Linux.slnf -c Release -warnaserror
dotnet test  Life.Linux.slnf -c Release --collect:"XPlat Code Coverage"
dotnet format Life.Linux.slnf --verify-no-changes
dotnet run --project src/Life.Console
dotnet run --project benchmarks/Life.Benchmarks -c Release

dotnet tool install -g docfx
docfx docs/docfx.json --serve
```

**Windows / Visual Studio:**
```text
Открыть Life.sln
Startup project: Life.Wpf
```
```bash
dotnet build Life.sln -c Release -warnaserror
dotnet publish src/Life.Wpf -c Release -r win-x64 --self-contained -p:PublishSingleFile=true
```

## Ближайшие 3 шага

1. `git init` + `.gitignore` + `.editorconfig` + структура проектов + `Life.Linux.slnf`.
2. Вынести логику в `Life.Core`, написать 5 тестов-страховок: block, blinker, glider, пустое поле, одинокая клетка.
3. Поднять `ci.yml`: Linux-фильтр build+test, Windows WPF build, бейджи в README.