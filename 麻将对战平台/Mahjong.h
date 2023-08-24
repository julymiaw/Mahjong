#pragma once
#ifndef MAHJONG_H
#define MAHJONG_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <random>

enum class Suit { WAN, TONG, TIAO, FENG };//�������ͣ��ֱ�Ϊ��Ͳ��������

class Tile {
public:
    Tile(Suit _suit, int _value) : suit(_suit), value(_value) {}//���캯��������һ���齫��

    Suit getSuit() const {
        return suit;//�����齫�ƵĻ�ɫ
    }

    int getValue() const {
        return value;//�����齫�Ƶ�ֵ
    }

    void display() const {
        std::string suitStr;
        switch (suit) {
        case Suit::WAN: suitStr = "�f"; break;
        case Suit::TONG: suitStr = "Ͳ"; break;
        case Suit::TIAO: suitStr = "�l"; break;
        case Suit::FENG: suitStr = "�L"; break;//ʵ���Ϸ���Ϊ�����������׷��С����֣����ﰴ��1�硱-��7�硱��ʾ
        }
        std::cout << value << suitStr << " ";
    }
    bool operator==(Tile& tile) {
        if (suit == tile.suit && value == tile.value)return true;
        return false;
    }

private:
    Suit suit;
    int value;
};

Tile Null = Tile(Suit::FENG, 0);//���ǲ����ڵķ�0��������ʾ�쳣״̬

class Mahjong {
public:
    Mahjong() {
        //�Ƚ��ƶѳ�ʼ��Ϊ136��
        
        //��Ͳ�����Ƹ�9�ֻ�ɫ����4��
        for (int i = 1; i <= 9; ++i) {
            for (int j = 0; j < 4; ++j) {
                tiles.push_back(Tile(Suit::WAN, i));
                tiles.push_back(Tile(Suit::TONG, i));
                tiles.push_back(Tile(Suit::TIAO, i));
            }
        }
        //������7�ֻ�ɫ����4��
        for (int i = 1; i <= 7; ++i) {
            for (int j = 0; j < 4; ++j) {
                tiles.push_back(Tile(Suit::FENG, i));
            }
        }

        // �����ƶ�
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(tiles.begin(), tiles.end(), gen);
    }//���캯��������һ����ʽ�齫�Ʋ�����

    Tile drawTile() {
        if (n > 0) {//����ƶѲ�Ϊ�������
            Tile tile = tiles.back();
            tiles.pop_back();
            n -= 1;
            return tile;
        }
        else return Null;
    }//���ƶѳ��ƣ�ʣ������-1

    bool checkN() {
        if (n <= 1)return true;
        else return false;
    }

private:
    std::vector<Tile> tiles;
    int n=136;//�ƶ���ʣ������
};

class Player {
public:
    Player(const std::string& _name,bool _x=0){
        name = _name;
        x = _x;
    }//���캯�����½�һ����ң�ֻ���ṩ�����

    void drawTile(Mahjong& mahjong) {
        if (x) {
            //���ƶ���һ����
            Tile tile = mahjong.drawTile();
            std::cout << name<< "�鵽�ˣ�";
            tile.display();
            std::cout << std::endl;
            hand.push_back(tile);
            sortHand();//���ƺ���������
        }
        else{
            Tile tile = mahjong.drawTile();
            hand.push_back(tile);//�����˳��Ʋ�������ʾ����
            sortHand();//���ƺ���������
        }
        
    }//��һ���Ƽ���������

    void drawTile(Tile& tile) {
        hand.push_back(tile);//�������أ����ڿ�ʼ��Ϸ����
    }

    void displayHand() const {
        std::cout << getName() << "�����ƣ�";
        for (const auto& tile : hand) {
            tile.display();
        }
        std::cout << std::endl;
        std::cout << getName() << "�����ƣ�";
        for (const auto& tile : table) {
            tile.display();
        }
        std::cout << std::endl;
    }//չʾ��������

    Tile discardTile() {
        if (x) {
            //��չʾ��������
            std::cout << name << "�����ƣ�";
            for (const auto& tile : hand) {
                tile.display();
            }
            std::cout << std::endl;
            //��ѡ��Ҫ������
            unsigned int index;
            do {
                std::cout << "����Ҫ�������Ƶ���������0��ʼ����";
                std::cin >> index;
            } while (index < 0 || index >= hand.size());//������������Ƿ�����������
            //�������������
            Tile discardedTile = hand[index];
            hand.erase(hand.begin() + index);
            //չʾ���ƶ�
            std::cout << name << "�����ƶѣ�";
            discard.push_back(discardedTile);
            for (const auto& tile : discard) {
                tile.display();
            }
            std::cout << std::endl;
            return discardedTile;
        }
        else
        {
            std::random_device rd;
            std::mt19937 gen(rd()); // ʹ��Mersenne Twister�㷨

            // ����һ���ֲ���ָ��Ҫ���ɵķ�Χ
            int i = hand.size() - 1;//���ﾯ����ԭ��ͬ��������ת�����ܵ��µ����ݶ�ʧ�����ҿ�������̫���ܵ������ݶ�ʧ
            std::uniform_int_distribution<int> distribution(0, i);//�ڵ�ǰ�汾���װ������Ҳ��ִ�г����ܲ���

            // ���������
            int index = distribution(gen);
            Tile discardedTile = hand[index];
            hand.erase(hand.begin() + index);
            //չʾ���ƶ�
            std::cout << getName() << "�����ƶѣ�";
            discard.push_back(discardedTile);
            for (const auto& tile : discard) {
                tile.display();
            }
            std::cout << std::endl;
            return discardedTile;
        }
            
    }

    Tile discardTile(int i) {
        Tile discardedTile = hand[i];//�������Ƶ����أ�Ϊ����ѧϰԤ��
        hand.erase(hand.begin() + i);
        //չʾ���ƶ�
        std::cout << getName() << "�����ƶѣ�";
        discard.push_back(discardedTile);
        for (const auto& tile : discard) {
            tile.display();
        }
        std::cout << std::endl;
        return discardedTile;
    }

    void sortHand() {
        std::sort(hand.begin(), hand.end(), [](const Tile& lhs, const Tile& rhs) {
            if (lhs.getSuit() == rhs.getSuit()) {
                return lhs.getValue() < rhs.getValue();
            }
            return static_cast<int>(lhs.getSuit()) < static_cast<int>(rhs.getSuit());
            });
    }//�������ƣ�ʹ��ͬ��ɫ�ư���С��������

    bool checkHu() const {
        // ȷ���Ƿ����
        std::unordered_map<int, int> countMap;  // Count of each value
        for (const Tile& tile : hand) {
            countMap[tile.getValue()]++; //һ�����ڼ�¼�������ÿ����ֵ�������Ĺ�ϣ��ͨ���������е�ÿ���ƣ����ǿ��Ե�֪ÿ����ֵ��������
        }

        int setCount = 0; //����������ڼ�¼���ӣ�������ͬ���ƣ�������������ͨ������ countMap����ÿ����ֵ���������� 3�����ɵõ����ӵ�������
        int sequenceCount = 0;//����������ڼ�¼˳�ӣ���������������ֵ��������������ͨ������ countMap������ÿ����ֵ������Ƿ�������ڵ�������ֵ��������������ֵ�����������ڵ��� 1��������������������˵������һ��˳�ӡ�
        for (const auto& pair : countMap) {
            int count = pair.second;
            setCount += count / 3;  // Count sets of 3 tiles
            if (count >= 3) {
                count -= 3 * (count / 3);
            }
            if (count > 0 && countMap.count(pair.first + 1) && countMap.count(pair.first + 2)) {
                sequenceCount++;
            }
        }

        return setCount + sequenceCount >= 4;  // Check if there are at least 4 sets or sequences
    }

    bool isHuPlayer() const {
        return isHu;//�ж�����Ƿ����
    }

    std::string getName() const {
        if (x)return name;//�����������
        else {
            std::string robot = "(robot)";
            std::string _name = name + robot;
            return _name;
        }
        
    }

    Tile drawAndDiscard(Mahjong& mahjong,Tile& tile,Player& a) {//��һ����ҵ��ϼ������趨ΪNull����0���������������Ʋ����ܱ��ԣ��ڳ��ƹ�������Ҫע�⡣
        //��չʾ�Լ�������
        displayHand();
        //���ж��Ƿ����
        if (chowCheck(tile)) {
            //����Ҳ��Ҫ��������ж���������ƣ�ѡ������ǳԺ���������������ܺ��Ʋ�����ͨ���Ƿ����
            int n;
            do {
                std::cout << "��ѡ���Ƿ���ƣ�" << std::endl;
                std::cin >> n;
            } while (n < 0 || n > 1);//�������ѡ��Ƿ�����������
            if (n) {
                Tile discardedTile = chow(tile,a);
                return discardedTile;
            }
        }
        //�ж��ƿ��Ƿ�ֻ��<=1����
        if (mahjong.checkN()) {
            //ֻ��1����ʱ��������ƣ������������Ϸ���������򷵻�Null
            drawTile(mahjong);
            /*if (checkHu()) {
                std::cout << name << " ���ƣ�" << std::endl;
                isHu = true;
            }*/
            return Null;//���ַ���Nullʱ������Ƿ��Ѻ��ƣ�����Ѻ�������Ϸ������������δ�������������
        }
        //�ƿ⻹��>1����ʱ����һ����
        drawTile(mahjong);
        //����Ƿ���ƣ�����Ĵ��뻹�������У�
        /*if (checkHu()) {
            std::cout << name << " ���ƣ�" << std::endl;
            isHu = true;
            return Null;
        }
        else
        */
        //��δ���ƣ��������ƽ׶�
        Tile discardedTile = discardTile();

        return discardedTile;
    }

    bool chowCheck(Tile& tile) {
        //�������ƺ��ϼ������ж��Ƿ��ܳ���
        Suit targetSuit = tile.getSuit();
        if (targetSuit == Suit::FENG)return false;//���ݹ��򣬷��Ʋ��ܱ���
        int targetValue = tile.getValue();
        //���ƴ��һ����3������������Ʒֱ�λ��3��λ�á����У���һ������޶����1-7���ڶ����޶����2-8�����������Ʊ��3-9
        //���1
        if (targetValue >= 1 && targetValue <= 7) {
            bool a = 0, b = 0;//�����ж�����λ��
            for (const Tile& t : hand) {
                if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                    if (t.getValue() == targetValue + 1)a = true;
                    if (t.getValue() == targetValue + 2)b = true;
                }
            }
            if (a && b)return true;
        }
        //���2
        if (targetValue >= 2 && targetValue <= 8) {
            bool a = 0, b = 0;//�����ж�����λ��
            for (const Tile& t : hand) {
                if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                    if (t.getValue() == targetValue - 1)a = true;
                    if (t.getValue() == targetValue + 1)b = true;
                }
            }
            if (a && b)return true;
        }
        //���3
        if (targetValue >= 3 && targetValue <= 9) {
            bool a = 0, b = 0;//�����ж�����λ��
            for (const Tile& t : hand) {
                if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                    if (t.getValue() == targetValue - 1)a = true;
                    if (t.getValue() == targetValue - 2)b = true;
                }
            }
            if (a && b)return true;
        }
        return false;
    }

    Tile chow(Tile& tile,Player& c) {
        //��Ȼ�⿴��ȥ���޴�����Ϊ���ҳ������п���ϳ��Ƶ���ϣ��ұ����ظ�chowCheck�еĴ󲿷ִ��롣

        Suit targetSuit = tile.getSuit();
        int targetValue = tile.getValue();
        //���ƴ��һ����3������������Ʒֱ�λ��3��λ�á��ұ��뿼�ǵ�3�����������ֹһ���������ʱ������Ҫ�ṩ��ͬ��ѡ����ȷ��ͨ����һ�����ʵ�ֳ���
        int n = 0;//����ѡ����Ŀ����ѡ����Ϊ1������ѡ��
        //����3����������ƶ�Ӧλ�����
        int A[3] = { 0,0,0 };
        int B[3] = { 0,0,0 };
        //���1
        if (targetValue >= 1 && targetValue <= 7) {
            bool a = 0, b = 0;//�����ж�����λ��
            int indexA = -1, indexB = -1;
            for (unsigned int index = 0; index < hand.size(); index++) {
                Tile t = hand[index];
                if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                    if (t.getValue() == targetValue + 1) {
                        a = true;
                        indexA = index;
                    }
                    if (t.getValue() == targetValue + 2) {
                        b = true;
                        indexB = index;
                    }
                }
            }
            if (a && b) {
                A[n] = indexA;
                B[n] = indexB;
                n++;//��ѡ���Ʒ���+1
            }
        }
        //���2
        if (targetValue >= 2 && targetValue <= 8) {
            bool a = 0, b = 0;//�����ж�����λ��
            int indexA = -1, indexB = -1;
            for (unsigned int index = 0; index < hand.size(); index++) {
                Tile t = hand[index];
                if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                    if (t.getValue() == targetValue - 1) {
                        a = true;
                        indexA = index;
                    }
                    if (t.getValue() == targetValue + 1) {
                        b = true;
                        indexB = index;
                    }
                }
            }
            if (a && b) {
                A[n] = indexA;
                B[n] = indexB;
                n++;//��ѡ���Ʒ���+1
            }
        }
        //���3
        if (targetValue >= 3 && targetValue <= 9) {
            bool a = 0, b = 0;//�����ж�����λ��
            int indexA = -1, indexB = -1;
            for (unsigned int index = 0; index < hand.size(); index++) {
                Tile t = hand[index];
                if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                    if (t.getValue() == targetValue - 2) {
                        a = true;
                        indexA = index;
                    }
                    if (t.getValue() == targetValue - 1) {
                        b = true;
                        indexB = index;
                    }
                }
            }
            if (a && b) {
                A[n] = indexA;
                B[n] = indexB;
                n++;//��ѡ���Ʒ���+1
            }
        }
        //ѡ��ֻ��һ��
        if (n == 1) {
            Tile handTile = hand[A[0]];
            table.push_back(handTile);
            hand.erase(hand.begin() + A[0]);
            handTile = hand[B[0] - 1];//������һ����������Ϊ������Ҫ�������size_t���ͣ���64λ������Ϊ8�ֽڣ���-1��int������Ϊ4�ֽڡ��Ҳ����4�ֽڵ�8�ֽ��Ƿ������������
            table.push_back(handTile);
            hand.erase(hand.begin() + B[0] - 1);//ע�⣬��A����һ��������λ���Ѹı�
            handTile = tile;
            table.push_back(handTile);
            c.discard.pop_back();
            std::sort(table.end() - 3, table.end(), [](const Tile& lhs, const Tile& rhs) {return lhs.getValue() < rhs.getValue(); });//�Ըշŵ����ϵ�������
        }
        //���ѡ�����һ��
        if (n > 1) {
            std::cout << "�ϼ����õ����ǣ�";
            tile.display();
            std::cout << std::endl;
            std::cout << "��ѡ��һ�ֳ��Ʒ�����" << std::endl;
            for (int i = 0; i < n; i++) {
                hand[A[i]].display();
                hand[B[i]].display();
                std::cout << std::endl;
            }
            int choice;
            do {
                std::cout << "���뷽������������0��ʼ����";
                std::cin >> choice;
            } while (choice< 0 || choice >= n);//������������Ƿ�����������

            Tile handTile = hand[A[choice]];
            table.push_back(handTile);
            hand.erase(hand.begin() + A[choice]);
            handTile = hand[B[choice] - 1];//������һ����������Ϊ������Ҫ�������size_t���ͣ���64λ������Ϊ8�ֽڣ���-1��int������Ϊ4�ֽڡ��Ҳ����4�ֽڵ�8�ֽ��Ƿ������������
            table.push_back(handTile);
            hand.erase(hand.begin() + B[choice] - 1);//ע�⣬��A����һ��������λ���Ѹı�
            handTile = tile;
            table.push_back(handTile);
            c.discard.pop_back();
            std::sort(table.end() - 3, table.end(), [](const Tile& lhs, const Tile& rhs) {return lhs.getValue() < rhs.getValue(); });//�Ըշŵ����ϵ�������
        }
        displayHand();
        Tile discardedTile = discardTile();//���ƺ����ƣ��൱��ȡ��������drawAndDiscard
        return discardedTile;
    }

    bool kongCheck(Tile& tile) {
        bool k = 0; //Ĭ���ǲ�����
        //�����Ǹ������ƺ�����һ�������ж��Ƿ������Ƶ��ж���䣬��û�����
        return k;
    }

    Tile kong(Tile& tile) {
        //����������������һ�ҵ����ƣ���ɾȥ��Ӧ2�����ƣ�������Ƶ����
        Tile discardedTile = discardTile();//���ƺ����ƣ��൱��ȡ��������drawAndDiscard
        return discardedTile;
    }

    bool pungCheck(Tile& tile) {
        bool p = 0; //Ĭ���ǲ��ܸ�
        //�����Ǹ������ƺ�����һ�������ж��Ƿ��ܸ��Ƶ��ж���䣬��û�����(ʵ��ʵ��ʱ���԰���һ���kongCheck�϶�Ϊһ��
        return p;
    }

    Tile pung(Tile& tile, Mahjong& mahjong) {//��Ϊ�漰�����ƣ�������Ҫ�����ƶ�
        //�����Ǹܳ�������һ�ҵ����ƣ���ɾȥ��Ӧ2�����ƣ�������Ƶ���䣨�ݲ��������ܣ����ܣ��Ӹܵ�����
        drawTile(mahjong);//���ƺ����ȳ�һ����
        Tile discardedTile = discardTile();//���ƺ����ƣ��൱��ȡ��������drawAndDiscard
        return discardedTile;
    }

    bool operator==(Player& player) {
        if (name == player.name)return true;
        return false;
    }

    std::vector<Tile> table;//���ϵ��ƣ�Ϊ�ԣ�������չʾ����
    std::vector<Tile> discard;//���ƶ�
private:
    std::string name;
    bool x;//�ж��Ƿ�Ϊ��ʵ��ң����xΪ0����ֻ��ִ�����Ʋ����Ʋ���
    std::vector<Tile> hand;//���е���
    bool isHu = false;
};

Player Draw = Player(Draw);//���ǲ����ڵ���ң�������ʾ���֣�����λ��һ�ʤʱ��˵�����˻�ʤ

#endif // !MAHJONG_H