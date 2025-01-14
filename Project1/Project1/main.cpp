#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> // for rand()
#include <ctime>   // for time()
using namespace std;

void printTitle() {
    cout << "SSSSS  III  M   M  PPPP   L      EEEE    RRRR   PPPP   GGGG " << endl;
    cout << "S       I   MM MM  P   P  L      E       R   R  P   P  G    " << endl;
    cout << "SSSSS   I   M M M  PPPP   L      EEEE    RRRR   PPPP   G GG " << endl;
    cout << "    S   I   M   M  P      L      E       R  R   P      G   G" << endl;
    cout << "SSSSS  III  M   M  P      LLLLL  EEEE    R   R  P       GGGG" << endl;
    cout << "                                                            " << endl;
    cout << "                                                            " << endl;
    cout << "                                                            " << endl;
    cout << "                   1. 게임시작(Host)                         " << endl;
    cout << "                   2. 게임시작(Join)                         " << endl;
    cout << "                                                            " << endl;
    cout << "                                                            " << endl;
    cout << endl;
}

class Character {
public:
    string name;
    int attack;
    int defense;
    int health;
    int dodge;
    int luck;
    string characterClass;

    // 생성자
    Character() {};
    Character(string n, string c) : name(n), characterClass(c) {
        if (characterClass == "Warrior") {
            attack = 10;
            defense = 8;
            health = 100;
            dodge = 5;
            luck = 2;
        }
        else if (characterClass == "Mage") {
            attack = 5;
            defense = 4;
            health = 70;
            dodge = 8;
            luck = 10;
        }
        else { // Archer
            attack = 7;
            defense = 6;
            health = 85;
            dodge = 6;
            luck = 6;
        }
    }

    void printStats() {
        cout << "이름: " << name << endl;
        cout << "클래스: " << characterClass << endl;
        cout << "공격력: " << attack << endl;
        cout << "방어력: " << defense << endl;
        cout << "체력: " << health << endl;
        cout << "회피력: " << dodge << endl;
        cout << "행운: " << luck << endl;
    }

    bool isAlive() {
        return health > 0;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    void heal(int amount) {
        health += amount;
    }
};

Character characterCreation() {
    string name;
    string characterClass;

    cout << "캐릭터 이름을 입력하세요: ";
    cin >> name;

    cout << "클래스를 선택하세요 (전사, 마법사, 궁수): ";
    cin >> characterClass;

    Character player(name, characterClass);
    player.printStats();
    return player;
}

class Enemy {
public:
    string name;
    int attack;
    int defense;
    int health;

    Enemy(string n, int a, int d, int h) : name(n), attack(a), defense(d), health(h) {}

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    bool isAlive() {
        return health > 0;
    }
};

void fight(Character& player, Enemy& enemy) {
    srand(time(0)); // 랜덤 시드 초기화
    int playerDamage, enemyDamage;

    while (player.isAlive() && enemy.isAlive()) {
        // 플레이어 공격
        playerDamage = rand() % player.attack + 1;
        cout << player.name << "이(가) " << enemy.name << "을(를) 공격하여 " << playerDamage << "의 데미지를 입혔습니다!" << endl;
        enemy.takeDamage(playerDamage);

        if (!enemy.isAlive()) {
            cout << enemy.name << "을(를) 처치했습니다!" << endl;
            break;
        }

        // 적 공격
        enemyDamage = rand() % enemy.attack + 1;
        cout << enemy.name << "이(가) " << player.name << "을(를) 공격하여 " << enemyDamage << "의 데미지를 입혔습니다!" << endl;
        player.takeDamage(enemyDamage);

        if (!player.isAlive()) {
            cout << player.name << "이(가) 패배했습니다!" << endl;
            break;
        }
    }
}

void displayMap() {
    cout << "1. 마을" << endl;
    cout << "2. 던전 입구" << endl;
}

void moveToLocation(int choice) {
    if (choice == 1) {
        cout << "마을에 도착했습니다. 휴식을 취하거나 상점에서 물품을 구매할 수 있습니다." << endl;
    }
    else if (choice == 2) {
        cout << "던전에 입장했습니다. 몬스터에 주의하세요!" << endl;
    }
    else {
        cout << "잘못된 선택입니다!" << endl;
    }
}

void showStatus(Character& player) {
    cout << "=== 상태창 ===" << endl;
    player.printStats();
    cout << "=============" << endl;
}

void dungeon(Character& player) {
    cout << endl << endl;
    cout << "던전에 입장했습니다..." << endl;
    Enemy slime("슬라임", 5, 2, 30);
    Enemy minotaur("미노타우르스", 15, 5, 100);

    int floor = 1;
    while (player.isAlive() && floor <= 3) {
        cout << floor << "층" << endl;
        cout << endl << endl;
        // 각 층에 맞는 적 등장
        Enemy currentEnemy = (floor == 3) ? minotaur : slime;
        fight(player, currentEnemy);

        if (player.isAlive()) {
            cout << "다음 층으로 이동합니다!" << endl;
            cout << endl << endl;
            floor++;
        }
        else {
            cout << "게임 오버!" << endl;
            break;
        }
    }
}

int main() {
    printTitle();

    char i;
    cin >> i;

    Character c;
    switch (i)
    {
    case '1':
        break;
    case '2':
    {
        c = characterCreation();
        cout << endl << endl;
        int choice;
        while (true) {
            displayMap();
            cout << "다음 위치를 선택하세요 (1 또는 2): ";
            cin >> choice;

            moveToLocation(choice);
            cout << endl << endl;

            if (choice == 1) {
                cout << "휴식을 취합니다. 체력이 회복됩니다." << endl;
                c.heal(30); // 임시로 체력 회복
                showStatus(c);
                cout << endl << endl;
            }
            else if (choice == 2) {
                dungeon(c);
            }
        }
        break;
    }
    case 'q':
        return 0;
    default:
        break;
    }
}
