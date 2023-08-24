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

Tile Null = Tile(Suit::FENG, 0);//���ǲ����ڵķ�0��������ʾ�쳣״̬�������������齫���У�����Ϊ���Ʋ�����ԣ����Բ�����������ܵ����жϡ�����ʹ��

class Mahjong {
public:
    //���캯��������һ����ʽ�齫�Ʋ�����
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
        std::mt19937 gen(rd());// ʹ��Mersenne Twister�㷨�����������
        std::shuffle(tiles.begin(), tiles.end(), gen);
    }

    //���ƶѳ��ƣ�ʣ������-1
    Tile drawTile() {
        if (n > 0) {//����ƶѲ�Ϊ�������
            Tile tile = tiles.back();
            tiles.pop_back();
            n -= 1;
            return tile;
        }
        else return Null;
    }

    //�жϳ��ƶѵ�ǰ�����Ƿ�С�ڵ���1������Ϊ�˷�ֹ���ƷǷ����ʵ��µĳ������
    bool checkN() {
        if (n <= 1)return true;
        else return false;
    }

private:
    std::vector<Tile> tiles;
    int n=136;//�ƶ���ʣ��������ÿ��ִ�д��ƶѳ��Ʋ������-1
};

class Player {
public:
    //���캯�����½�һ����ң�ֻ���ṩ�����
    Player(const std::string& _name, bool _x = 0) {
        name = _name;
        x = _x;
    }

    //��һ���Ƽ���������
    Tile drawTile(Mahjong& mahjong) {
        if (x) {
            //���ƶ���һ����
            Tile tile = mahjong.drawTile();
            std::cout << name<< "�鵽�ˣ�";
            tile.display();
            std::cout << std::endl;
            hand.push_back(tile);
            sortHand();//���ƺ���������
            return tile;
        }
        else{
            Tile tile = mahjong.drawTile();
            hand.push_back(tile);//�����˳��Ʋ�������ʾ����
            sortHand();//���ƺ���������
            return tile;
        }
        
    }

    //�������أ����ڿ�ʼ��Ϸ����
    void drawTile(Tile& tile) {
        hand.push_back(tile);
    }

    //չʾ�������ƣ�������ʾ��������ͨ��ҺͰװ���������
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
    }

    //����һ�����ƣ��������ң�չʾ���Ʋ�ѯ�����������������������һ���ƣ�����ʾ���ƣ���ͬ����ʾ���ƺ�����ƶ�
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
            int i = hand.size() - 1;
            std::uniform_int_distribution<int> distribution(0, i);

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

    //�������Ƶ����أ�Ϊ����ѧϰԤ��
    Tile discardTile(int i) {
        Tile discardedTile = hand[i];
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

    //�������ƣ�ʹ��ͬ��ɫ�ư���С��������
    void sortHand() {
        std::sort(hand.begin(), hand.end(), [](const Tile& lhs, const Tile& rhs) {//�����õ���<algorithm>���sort������ǰ��2�������ֱ�Ϊ����������յ㣬���һ������������Ĺ���
            if (lhs.getSuit() == rhs.getSuit()) {
                return lhs.getValue() < rhs.getValue();//�����ɫһ�����Ƚϴ�С
            }
            return static_cast<int>(lhs.getSuit()) < static_cast<int>(rhs.getSuit());//�����õ��˾�̬ǿ������ת������4�ֻ�ɫǿ��ת����int�ͣ��Ҳ�����������ת���ģ�������ȷʵʵ������Ͳ��������˳�������
            });
    }

    //�жϺ��Ƶķ��������ش����⣬��Ҫ��д����ֻ���жϱ�Ҫ�ԣ������жϳ���ԣ����Ի�����������жϡ�����Ҫ���ǣ���û�п��ǵ����ϵ���
    bool checkHu() const {
        // ȷ���Ƿ���ƣ�ÿ�����˳��ƣ�����Ҫ����Ƿ��������ƣ����������غϿ�ʼ���߸��ƺ���Ҫ��������ж�����ÿ���������ƺ󣬳���������Ҷ������ж��Ƿ���ƣ�
        std::unordered_map<int, int> countMap;//�����㷨�о޴����⣬�����������Ϊ��ͳ�Ƶ�ʵ������ÿ����ֵ��������������ÿ���Ƶ����������������ʵ��û�����壬��˼·���ֿ��Բο�
        for (const Tile& tile : hand) {
            countMap[tile.getValue()]++;  // �����ͳ����ÿ����ֵ��Ӧ�Ƶ�������pair.first��������ֵ��pair.second������������Ҳ����˵�������ῼ�ǵ���ɫ
        }
        //����ֻ������2��������ʵ���Ͽ�����Ҫ5������������3�������ֱ������ϵĿ�������3��1�飩�����ϵĸ�������4��1�飩�����ϵ�˳����
        int setCount = 0; //����������ڼ�¼���ӣ�������ͬ���ƣ�������������ͨ������ countMap����ÿ����ֵ���������� 3�����ɵõ����ӵ�������
        int sequenceCount = 0;//����������ڼ�¼˳�ӣ���������������ֵ��������������ͨ������ countMap������ÿ����ֵ������Ƿ���ڱ�����1����2��������ֵ��������������ֵ�����������ڵ��� 1��������������������˵�������һ��˳�ӣ�������˵��������Ǻ��������˳�ӣ�����123456��234�������˳�ӣ���123��456�������2��
        for (const auto& pair : countMap) {
            int count = pair.second;//����ÿ���ƣ���count��¼�Ƶ�����
            setCount += count / 3;  // ֻ�е��������ڵ���3ʱ�����ӵ�����+1
            if (count >= 3) {
                count -= 3 * (count / 3);//��������Ƶ��������ڻ����3���ȼ���3�ţ���Ϊ�ղſ��ӵ�ͳ�����Ѿ����ǹ��ˣ����˼·���Ա���
            }
            if (count > 0 && countMap.count(pair.first + 1) && countMap.count(pair.first + 2)) {//��������ƺͱ�����1����2�Ķ����ڣ�˳����+1��ע�⣬�뿼�ǵ��ظ��������⣬��ǰ�����޷���������⣩
                sequenceCount++;
            }
        }

        return setCount + sequenceCount >= 4;  // Check if there are at least 4 sets or sequences
    }

    //�ж�����Ƿ����
    bool isHuPlayer() const {
        return isHu;
    }

    //�����������
    std::string getName() const {
        if (x)return name;
        else {
            std::string robot = "(robot)";
            std::string _name = name + robot;
            return _name;
        }
        
    }

    //�������������Ϸ���̣����͸�Ϊ�������̣�
    Tile drawAndDiscard(Mahjong& mahjong,Tile& discardedTile,Player& a) {

        //����Ƿ���ƣ���Ϊ����checkHu���������⣬��ʱ����һ��ע�͵������������ƺ���Իָ���������Ƶ�ԭ�����ϼ�������������������
        /*if (checkHu()) {
            std::cout << name << " ���ƣ�" << std::endl;
            isHu = true;
            return Null;//�����쳣��
        }
        else{
            //��δ���ƣ�����������Ĳ���
        }*/

        //չʾ�Լ�������
        displayHand();
        //�ж��Ƿ����
        if (chowCheck(discardedTile)) {
            int n;
            do {
                std::cout << "��ѡ���Ƿ���ƣ�" << std::endl;
                std::cin >> n;
            } while (n < 0 || n > 1);//�������ѡ��Ƿ�����������
            if (n) {
                discardedTile = chow(discardedTile,a);
                return discardedTile;
            }
        }
        //�ж��ƿ��Ƿ�ֻ��<=1����
        if (mahjong.checkN()) {
            //ֻ��1����ʱ�����������������Ϸ���������򷵻�Null�쳣����������
            drawTile(mahjong);
            //����Ƿ���ƣ���Ϊ����checkHu���������⣬��ʱ����һ��ע�͵������������ƺ���Իָ���������Ƶ�ԭ���������鵽�˺�������
            /*if (checkHu()) {
                std::cout << name << " ���ƣ�" << std::endl;
                isHu = true;
            }*/
            return Null;//���ַ���Nullʱ������Ƿ��Ѻ��ƣ�����Ѻ�������Ϸ������������δ�������������
        }
        //�ƿ⻹��>1����ʱ����һ����
        Tile tile = drawTile(mahjong);//���س鵽����

        //����Ƿ���ƣ���Ϊ����checkHu���������⣬��ʱ����һ��ע�͵������������ƺ���Իָ���������Ƶ�ԭ���������鵽�˺�������
        /*if (checkHu()) {
            std::cout << name << " ���ƣ�" << std::endl;
            isHu = true;
            return Null;//�����쳣��
        }
        else{
            //��δ���ƣ�����������Ĳ���
        }*/

        //����Ƿ��ܰ���
        if (kongHideCheck(tile)) {
            int n;
            do {
                std::cout << "��ѡ���Ƿ񰵸ܣ�" << std::endl;
                std::cin >> n;
            } while (n < 0 || n > 1);//�������ѡ��Ƿ�����������
            if (n) {
                discardedTile = kong(discardedTile, mahjong);
                if (discardedTile == Null)return Null;//��Ϊ����ͬ���漰�����ƣ����������ƺ����ֵĿ��ܣ���������ͬ����Ҫ�ж����������NUll�쳣�����������NULL�쳣
                return discardedTile;
            }
        }
        discardedTile = discardTile();

        return discardedTile;
    }

    //�������ƺ��ϼ������ж��Ƿ��ܳ��ơ������������Ժ��ж�����Ϊ֮ǰ�Ѿ��ж�����
    bool chowCheck(Tile& tile) {
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

    //�Ե��ϼ��������ơ������������Ժ��ж�����Ϊ֮ǰ�Ѿ��ж�����
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

    //�������ƺͳ�������һ�������ж��Ƿ�������
    bool pungCheck(Tile& tile) {
        Suit targetSuit = tile.getSuit();//Ŀ�껨ɫ
        int targetValue = tile.getValue();//Ŀ��ֵ
        int n = 0;//�������ֵ
        for (const Tile& t : hand) {
            if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                if (t.getValue() == targetValue) n++;//�����ɫ����ֵ�����ϣ������+1
            }
        }
        if (n >= 2) return true;
        return false;
    }

    //����������������һ�ҵ����ƣ���ɾȥ��Ӧ2�����ƣ�������Ƶ����
    Tile pung(Tile& tile, Player& c) {
        Suit targetSuit = tile.getSuit();//Ŀ�껨ɫ
        int targetValue = tile.getValue();//Ŀ��ֵ
        int index = 0;//��������ֵ
        for (unsigned int i = 0; i < hand.size(); i++) {
            Tile t = hand[i];
            if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                if (t.getValue() == targetValue) {
                    index = i;//�����ɫ����ֵ�����ϣ��򴢴�����ֵ
                }
            }
        }
        Tile handTile = hand[index];
        for (int i = 0; i < 2; i++){
            table.push_back(handTile);
            hand.erase(hand.begin() + index - i);//����ɾ����������
        }
        table.push_back(tile);
        c.discard.pop_back();//����ҵ����ƶ��Ƴ���������

        Tile discardedTile = discardTile();//���ƺ����ƣ��൱��ȡ��������drawAndDiscard
        return discardedTile;
    }

    //�������ƺ��������ж��Ƿ��ܸ���
    bool kongCheck(Tile& tile) {
        Suit targetSuit = tile.getSuit();//Ŀ�껨ɫ
        int targetValue = tile.getValue();//Ŀ��ֵ
        int n = 0;//�������ֵ
        for (const Tile& t : hand) {
            if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                if (t.getValue() == targetValue) n++;//�����ɫ����ֵ�����ϣ������+1
            }
        }
        if (n >= 3) return true;
        return false;
    }

    //�ж����ƺͳ鵽���Ƿ����㰵��Ҫ��
    bool kongHideCheck(Tile& tile) {
        Suit targetSuit = tile.getSuit();//Ŀ�껨ɫ
        int targetValue = tile.getValue();//Ŀ��ֵ
        int n = 0;//�������ֵ
        for (const Tile& t : hand) {
            if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                if (t.getValue() == targetValue) n++;//�����ɫ����ֵ�����ϣ������+1
            }
        }
        if (n >= 4) return true;//������Ҫ���ƺ�������ͬ���ƴﵽ4�ţ���Ϊ����
        return false;
    }

    //ÿ�γ��ƺ󶼱��뿼���ܷ�Ӹܣ����������ڲ��ĳ��ƣ���δ��ӵ���ʽ����У�ϣ�������ж�ʵ�ֺ�����ӣ�
    bool kongPlusCheck(Tile& tile) {
        Suit targetSuit = tile.getSuit();//Ŀ�껨ɫ
        int targetValue = tile.getValue();//Ŀ��ֵ
        int n = 0;//�������ֵ
        for (const Tile& t : table) {//������Ϊ�Ҷ��齫����ⲻȫ�棬��һ���ж���׼�����д�
            if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                if (t.getValue() == targetValue) n++;//�����ɫ����ֵ�����ϣ������+1
            }
        }
        if (n == 3) return true;
        return false;
    }

    //����������
    Tile kong(Tile& tile, Mahjong& mahjong, Player& c) {//��Ϊ�漰�����ƣ�������Ҫ�����ƶѣ���Ϊ�漰�����ƶѣ�������Ҫ�����������
        Suit targetSuit = tile.getSuit();//Ŀ�껨ɫ
        int targetValue = tile.getValue();//Ŀ��ֵ
        int index = 0;//��������ֵ
        for (unsigned int i = 0; i < hand.size(); i++) {
            Tile t = hand[i];
            if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                if (t.getValue() == targetValue) {
                    index = i;//�����ɫ����ֵ�����ϣ��򴢴�����ֵ
                }
            }
        }
        for (int i = 0; i < 3; i++) {
            table.push_back(tile);
            hand.erase(hand.begin() + index - i);//����ɾ����������
        }
        table.push_back(tile);
        c.discard.pop_back();//����ҵ����ƶ��Ƴ����ܵ���

        drawTile(mahjong);//���ƺ����ȳ�һ����
        Tile discardedTile = discardTile();//���ƺ����ƣ��൱��ȡ��������drawAndDiscard
        return discardedTile;
    }

    //�����ǰ��ܣ����أ�
    Tile kong(Tile& tile, Mahjong& mahjong) {//��Ϊ�漰�����ƣ�������Ҫ�����ƶ�

        Suit targetSuit = tile.getSuit();//Ŀ�껨ɫ
        int targetValue = tile.getValue();//Ŀ��ֵ
        int index = 0;//��������ֵ
        for (unsigned int i = 0; i < hand.size(); i++) {
            Tile t = hand[i];
            if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                if (t.getValue() == targetValue) {
                    index = i;//�����ɫ����ֵ�����ϣ��򴢴�����ֵ
                }
            }
        }
        //��Ϊ�ڰ���ǰ�Ѿ���ɳ��ƣ���������Ϊ14��
        for (int i = 0; i < 4; i++) {
            table.push_back(tile);//���������Ϸ���
            hand.erase(hand.begin() + index - i);//����ɾ����������
        }

        //���ƺ����ȳ�һ����
        
        //�ж��ƿ���ʣ�����Ƿ񹻳���
        if (mahjong.checkN()) {
            drawTile(mahjong);

            //����Ƿ���ƣ���Ϊ����checkHu���������⣬��ʱ����һ��ע�͵������������ƺ���Իָ���������Ƶ�ԭ���������鵽�˺�������
            /*if (checkHu()) {
                std::cout << name << " ���ƣ�" << std::endl;
                isHu = true;
                return Null;//�����쳣��
            }
            else{
                //��δ���ƣ�����������Ĳ��֣���return Null;
            }*/

            
            return Null;//��Ϊ���ƿ�ֻʣ���һ���Ƶ�����³���δ���������쳣����������
        }
        //����Ƿ���ƣ���Ϊ����checkHu���������⣬��ʱ����һ��ע�͵������������ƺ���Իָ���������Ƶ�ԭ���������鵽�˺�������
            /*if (checkHu()) {
                std::cout << name << " ���ƣ�" << std::endl;
                isHu = true;
                return Null;//�����쳣��
            }
            else{
                //��δ���ƣ�����������Ĳ���
            }*/

        Tile discardedTile = discardTile();//���ƺ����ƣ��൱��ȡ��������drawAndDiscard
        return discardedTile;
    }

    //�����ǼӸܣ�δע�͵������ж����֣�����ʹ�ã�
    Tile kongPlus(Tile& tile, Mahjong& mahjong) {
        Suit targetSuit = tile.getSuit();//Ŀ�껨ɫ
        int targetValue = tile.getValue();//Ŀ��ֵ
        int index = 0;//��������ֵ
        for (unsigned int i = 0; i < table.size(); i++) {//��δ����漰���ж���׼���ҵ���������壬Ⱥ������û������
            Tile t = table[i];
            if (t.getSuit() == targetSuit) {//���Ȼ�ɫ����
                if (t.getValue() == targetValue) {
                    index = i;//�����ɫ����ֵ�����ϣ��򴢴�����ֵ
                }
            }
        }
        for (int i = 0; i < 3; i++) {
            table.push_back(tile);//�������������Ҷ˷���
            hand.erase(hand.begin() + index - i);//����ɾ��ԭ������������
        }
        table.push_back(tile);//�Ѹճ鵽����Ҳ�ŵ�����

        //���ƺ����ȳ�һ����
        
        //�ж��ƿ���ʣ�����Ƿ񹻳���
        if (mahjong.checkN()) {
            drawTile(mahjong);

            //����Ƿ���ƣ���Ϊ����checkHu���������⣬��ʱ����һ��ע�͵������������ƺ���Իָ���������Ƶ�ԭ���������鵽�˺�������
            if (checkHu()) {
                std::cout << name << " ���ƣ�" << std::endl;
                isHu = true;
                return Null;//�����쳣��
            }
            else{
                return Null;//��Ϊ���ƿ�ֻʣ���һ���Ƶ�����³���δ���������쳣����������
            }
        }

        //����Ƿ���ƣ���Ϊ����checkHu���������⣬��ʱ����һ��ע�͵������������ƺ���Իָ���������Ƶ�ԭ���������鵽�˺�������
            if (checkHu()) {
                std::cout << name << " ���ƣ�" << std::endl;
                isHu = true;
                return Null;//�����쳣��
            }
            else{
                Tile discardedTile = discardTile();//���ƺ����ƣ��൱��ȡ��������drawAndDiscard
                return discardedTile;
            }
    }
    bool operator==(Player& player) {//������������ж�����
        if (name == player.name)return true;
        return false;
    }

    //���ϵ��ƣ�Ϊ�ԣ�������չʾ���ơ�����public������Ϊ�Ӹܲ���ʱ�����������Ȩ�����ⲿ�ֵ������ں���
    std::vector<Tile> table;

    //���ƶѡ�����public������Ϊ�ԣ��������ܲ����У����������Ȩ�����ⲿ�ֵ������ں���
    std::vector<Tile> discard;
private:
    std::string name;
    bool x;//�ж��Ƿ�Ϊ��ʵ��ң����xΪ0����ֻ��ִ�����Ʋ����Ʋ���
    std::vector<Tile> hand;//���е��ƣ��κ������������Ҷ���Ȩ�޲�����鿴����
    bool isHu = false;//�������ԣ��������쳣ʱ�����ȼ�������
    //�������������������ԣ����ڼ�¼��ҵĸ������ݺ�״̬
    //���ݱ��磬�����������������������������������������������������������߿ɷ���public��������������ҷ����޸�
    //״̬���������������֣������ȷʵ�б�Ҫ�������������״̬����ǰ��״̬�ȣ���������ĳЩ��������ж�����Щ״̬��������Ϊ��ʾ�����������ҡ�
};

Player Draw = Player(Draw);//���ǲ����ڵ���ң�������ʾ���֣�����λ��һ�ʤʱ��˵�����˻�ʤ

#endif // !MAHJONG_H