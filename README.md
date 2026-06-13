ФИО: Хвостов Даниил Александрович
Тема: "Пошаговая RPG"
Описание: Вы играете за отряд, который проходит уровни. Система боя пошаговая. По мере прохождения игрок будет прогрессировать, получая улучшения. При смерти главного героя, игра проиграна.
- Создание героя с базовыми характеристиками
- Пошаговый бой: Игрок действует (хилит, бафает, атакует), враг действует
- Администратор создает уровень, выбирает какие сражения будут предложены игроку.

Список команд для игрока:
```
new-hero <name>
show-heroes
play-as <name>
show-hero <name>
delete-hero <name>
use-skill <character> <target>
show-bonuses
choose-bonus <bonus_number>
choose-path <path_number>
save
```
Список команд для администратора:
```
admin-mode
exit-admin-mode
add-path <stage_number>
add-monster <stage_number> <path_number> <monster_name> <monster_lvl>
```
1) new-hero <name>

Описание: Создаёт нового героя с именем <name>. Команда некорректна, если такой герой уже создан.
Ввод:
```
new-hero Arthas
```
Вывод:
```
HERO CREATED: Arthas
```
Ввод:
```
new-hero Arthas
```
Вывод:
```
<INVALID COMMAND> — герой Arthas уже существует
```
2) show-heroes
Описание: Показывает список всех созданных героев. Активный герой помечается *. Если героев нет — сообщает об этом.
Ввод:
```
show-heroes
```
Вывод:
```
* Arthas| HP:100/100 | LVL:1
  Jaina| HP:100/100 | LVL:1
```
Ввод:
```
show-heroes
```
Вывод:
```
<NO HEROES>
```
3) play-as <name>
Описание: Делает героя с именем <name> активным. Все последующие команды боя применяются к нему. Команда некорректна, если героя не существует.
Ввод:
```
play-as Arthas
```
Вывод:
```
ACTIVE HERO: Arthas
```
Ввод:
```
play-as Uther
```
Вывод:
```
<INVALID COMMAND> — герой Uther не найден
```
4) show-hero <name>
Описание: Показывает подробную информацию о герое. Если <name> не указан — показывает активного героя.
Ввод:
```
show-hero Arthas
```
Вывод:
```
HERO: Arthas
HP: 100/100 | STR:12 | INT:8 | DEF:7 | LVL:1 | XP:0/100
SKILLS: Base attack, Heavy attack
```
Ввод:
```
show-hero
```
Вывод:
```
<NO ACTIVE HERO>
```
5) delete-hero <name>
Описание: Удаляет героя с именем <name>. Команда некорректна, если героя не существует.
Ввод:
```
delete-hero Arthas
```
Вывод:
```
HERO DELETED: Arthas
```
Ввод:
```
delete-hero Arthas
```
Вывод:
```
<INVALID COMMAND> — герой Arthas не найден
```
6) use-skill <skill> <target>
Описание: Активный герой использует навык <skill> на цель <target>. Команда некорректна, если нет активного героя, нет боя или навык не изучен.
Ввод:
```
use-skill Base attack Goblin
```
Вывод:
```
Arthas attack Goblin!
Damage: 11 HP
Goblin HP: 9/20
```
7) show-bonuses
Описание: Показывает список доступных бонусов для выбора после победы над монстром.

Ввод:
```
show-bonuses
```
Вывод:
```
AVAILABLE BONUSES:
1. +10% HP
2. +5 STR
3. +5 INT
4. +3 DEF
5. Heal 30 HP
```
8) choose-bonus <bonus_number>
Описание: Выбирает бонус под указанным номером. Бонус применяется к активному герою. Команда некорректна, если нет активного героя или бонуса с таким номером.

Ввод:
```
choose-bonus 2
```
Вывод:
```
BONUS APPLIED: +5 STR
Arthas STR: 12 -> 17
```
9) choose-path <path_number>
Описание: Выбирает путь на развилке уровня(после завершения уровня выводятся пути). Каждый путь ведёт к разным монстрам и наградам. Команда некорректна, если нет активного героя или путь недоступен.

Ввод:
```
choose-path 1
```
Вывод:
```
PATH 1 CHOSEN
```
10) save
Описание: Сохраняет текущее состояние игры
Ввод:
```
save
```
Вывод:
```
GAME SAVED
```
11) admin-mode
Описание: Включает режим администратора. Игровые команды блокируются.

Ввод:
```
admin-mode
```
Вывод:
```
ADMIN MODE ENABLED
```
12) exit-admin-mode
Описание: Выходит из режима администратора.

Ввод:
```
exit-admin-mode
```
Вывод:
```
ADMIN MODE DISABLED
```
13) add-path <stage_number>
Описание: Добавляет новый путь (развилку) на указанном этапе <stage_number>. Команда доступна только в режиме администратора.

Ввод:
```
add-path 1
```
Вывод:
```
PATH ADDED: stage 1 now has 2 paths
```
14) add-monster <stage_number> <path_number> <monster_name> <monster_lvl>
Описание: Назначает
