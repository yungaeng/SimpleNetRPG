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
    cout << "                   1. ���ӽ���(Host)                         " << endl;
    cout << "                   2. ���ӽ���(Join)                         " << endl;
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

    // ������
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
        cout << "�̸�: " << name << endl;
        cout << "Ŭ����: " << characterClass << endl;
        cout << "���ݷ�: " << attack << endl;
        cout << "����: " << defense << endl;
        cout << "ü��: " << health << endl;
        cout << "ȸ�Ƿ�: " << dodge << endl;
        cout << "���: " << luck << endl;
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

    cout << "ĳ���� �̸��� �Է��ϼ���: ";
    cin >> name;

    cout << "Ŭ������ �����ϼ��� (����, ������, �ü�): ";
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
    srand(time(0)); // ���� �õ� �ʱ�ȭ
    int playerDamage, enemyDamage;

    while (player.isAlive() && enemy.isAlive()) {
        // �÷��̾� ����
        playerDamage = rand() % player.attack + 1;
        cout << player.name << "��(��) " << enemy.name << "��(��) �����Ͽ� " << playerDamage << "�� �������� �������ϴ�!" << endl;
        enemy.takeDamage(playerDamage);

        if (!enemy.isAlive()) {
            cout << enemy.name << "��(��) óġ�߽��ϴ�!" << endl;
            break;
        }

        // �� ����
        enemyDamage = rand() % enemy.attack + 1;
        cout << enemy.name << "��(��) " << player.name << "��(��) �����Ͽ� " << enemyDamage << "�� �������� �������ϴ�!" << endl;
        player.takeDamage(enemyDamage);

        if (!player.isAlive()) {
            cout << player.name << "��(��) �й��߽��ϴ�!" << endl;
            break;
        }
    }
}

void displayMap() {
    cout << "1. ����" << endl;
    cout << "2. ���� �Ա�" << endl;
}

void moveToLocation(int choice) {
    if (choice == 1) {
        cout << "������ �����߽��ϴ�. �޽��� ���ϰų� �������� ��ǰ�� ������ �� �ֽ��ϴ�." << endl;
    }
    else if (choice == 2) {
        cout << "������ �����߽��ϴ�. ���Ϳ� �����ϼ���!" << endl;
    }
    else {
        cout << "�߸��� �����Դϴ�!" << endl;
    }
}

void showStatus(Character& player) {
    cout << "=== ����â ===" << endl;
    player.printStats();
    cout << "=============" << endl;
}

void dungeon(Character& player) {
    cout << endl << endl;
    cout << "������ �����߽��ϴ�..." << endl;
    Enemy slime("������", 5, 2, 30);
    Enemy minotaur("�̳�Ÿ�츣��", 15, 5, 100);

    int floor = 1;
    while (player.isAlive() && floor <= 3) {
        cout << floor << "��" << endl;
        cout << endl << endl;
        // �� ���� �´� �� ����
        Enemy currentEnemy = (floor == 3) ? minotaur : slime;
        fight(player, currentEnemy);

        if (player.isAlive()) {
            cout << "���� ������ �̵��մϴ�!" << endl;
            cout << endl << endl;
            floor++;
        }
        else {
            cout << "���� ����!" << endl;
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
            cout << "���� ��ġ�� �����ϼ��� (1 �Ǵ� 2): ";
            cin >> choice;

            moveToLocation(choice);
            cout << endl << endl;

            if (choice == 1) {
                cout << "�޽��� ���մϴ�. ü���� ȸ���˴ϴ�." << endl;
                c.heal(30); // �ӽ÷� ü�� ȸ��
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
