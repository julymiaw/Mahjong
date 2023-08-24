#pragma once
#ifndef MAHJONG_H
#define MAHJONG_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <random>

enum class Suit { WAN, TONG, TIAO, FENG };//四种牌型，分别为万，筒，条，风

class Tile {
public:
    Tile(Suit _suit, int _value) : suit(_suit), value(_value) {}//构造函数，构造一张麻将牌

    Suit getSuit() const {
        return suit;//返回麻将牌的花色
    }

    int getValue() const {
        return value;//返回麻将牌的值
    }

    void display() const {
        std::string suitStr;
        switch (suit) {
        case Suit::WAN: suitStr = "萬"; break;
        case Suit::TONG: suitStr = "筒"; break;
        case Suit::TIAO: suitStr = "條"; break;
        case Suit::FENG: suitStr = "風"; break;//实际上风牌为“东南西北白发中”七种，这里按“1风”-“7风”表示
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

Tile Null = Tile(Suit::FENG, 0);//这是不存在的风0，用来表示异常状态

class Mahjong {
public:
    Mahjong() {
        //先将牌堆初始化为136张
        
        //万、筒和条牌各9种花色，各4张
        for (int i = 1; i <= 9; ++i) {
            for (int j = 0; j < 4; ++j) {
                tiles.push_back(Tile(Suit::WAN, i));
                tiles.push_back(Tile(Suit::TONG, i));
                tiles.push_back(Tile(Suit::TIAO, i));
            }
        }
        //风牌有7种花色，各4张
        for (int i = 1; i <= 7; ++i) {
            for (int j = 0; j < 4; ++j) {
                tiles.push_back(Tile(Suit::FENG, i));
            }
        }

        // 打乱牌堆
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(tiles.begin(), tiles.end(), gen);
    }//构造函数，构造一副日式麻将牌并打乱

    Tile drawTile() {
        if (n > 0) {//如果牌堆不为空则抽牌
            Tile tile = tiles.back();
            tiles.pop_back();
            n -= 1;
            return tile;
        }
        else return Null;
    }//从牌堆抽牌，剩余牌数-1

    bool checkN() {
        if (n <= 1)return true;
        else return false;
    }

private:
    std::vector<Tile> tiles;
    int n=136;//牌堆中剩余牌数
};

class Player {
public:
    Player(const std::string& _name,bool _x=0){
        name = _name;
        x = _x;
    }//构造函数，新建一个玩家，只需提供玩家名

    void drawTile(Mahjong& mahjong) {
        if (x) {
            //从牌堆摸一张牌
            Tile tile = mahjong.drawTile();
            std::cout << name<< "抽到了：";
            tile.display();
            std::cout << std::endl;
            hand.push_back(tile);
            sortHand();//抽牌后整理手牌
        }
        else{
            Tile tile = mahjong.drawTile();
            hand.push_back(tile);//机器人抽牌不会再显示出来
            sortHand();//抽牌后整理手牌
        }
        
    }//将一张牌加入手牌中

    void drawTile(Tile& tile) {
        hand.push_back(tile);//抽牌重载，用于开始游戏发牌
    }

    void displayHand() const {
        std::cout << getName() << "的手牌：";
        for (const auto& tile : hand) {
            tile.display();
        }
        std::cout << std::endl;
        std::cout << getName() << "的桌牌：";
        for (const auto& tile : table) {
            tile.display();
        }
        std::cout << std::endl;
    }//展示所有手牌

    Tile discardTile() {
        if (x) {
            //先展示所有手牌
            std::cout << name << "的手牌：";
            for (const auto& tile : hand) {
                tile.display();
            }
            std::cout << std::endl;
            //再选择要弃的牌
            unsigned int index;
            do {
                std::cout << "输入要弃掉的牌的索引（从0开始）：";
                std::cin >> index;
            } while (index < 0 || index >= hand.size());//如果输入索引非法则重新输入
            //最后弃掉那张牌
            Tile discardedTile = hand[index];
            hand.erase(hand.begin() + index);
            //展示弃牌堆
            std::cout << name << "的弃牌堆：";
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
            std::mt19937 gen(rd()); // 使用Mersenne Twister算法

            // 创建一个分布，指定要生成的范围
            int i = hand.size() - 1;//这里警报的原因同样是类型转换可能导致的数据丢失，就我看来，不太可能导致数据丢失
            std::uniform_int_distribution<int> distribution(0, i);//在当前版本，白板机器人也会执行吃碰杠操作

            // 生成随机数
            int index = distribution(gen);
            Tile discardedTile = hand[index];
            hand.erase(hand.begin() + index);
            //展示弃牌堆
            std::cout << getName() << "的弃牌堆：";
            discard.push_back(discardedTile);
            for (const auto& tile : discard) {
                tile.display();
            }
            std::cout << std::endl;
            return discardedTile;
        }
            
    }

    Tile discardTile(int i) {
        Tile discardedTile = hand[i];//这是弃牌的重载，为机器学习预留
        hand.erase(hand.begin() + i);
        //展示弃牌堆
        std::cout << getName() << "的弃牌堆：";
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
    }//整理手牌，使得同花色牌按从小到大排序

    bool checkHu() const {
        // 确认是否胡牌
        std::unordered_map<int, int> countMap;  // Count of each value
        for (const Tile& tile : hand) {
            countMap[tile.getValue()]++; //一个用于记录玩家手中每种牌值的数量的哈希表。通过遍历手中的每张牌，我们可以得知每种牌值的数量。
        }

        int setCount = 0; //这个变量用于记录刻子（三张相同的牌）的数量。我们通过遍历 countMap，将每种牌值的数量除以 3，即可得到刻子的数量。
        int sequenceCount = 0;//这个变量用于记录顺子（连续三张相邻牌值）的数量。我们通过遍历 countMap，对于每种牌值，检查是否存在相邻的两个牌值，并且这两个牌值的数量都大于等于 1。如果满足这个条件，就说明存在一个顺子。
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
        return isHu;//判断玩家是否胡牌
    }

    std::string getName() const {
        if (x)return name;//返回玩家姓名
        else {
            std::string robot = "(robot)";
            std::string _name = name + robot;
            return _name;
        }
        
    }

    Tile drawAndDiscard(Mahjong& mahjong,Tile& tile,Player& a) {//第一名玩家的上家弃牌设定为Null（风0），理论上这张牌不可能被吃，在吃牌规则中需要注意。
        //先展示自己的手牌
        displayHand();
        //再判断是否吃牌
        if (chowCheck(tile)) {
            //这里也需要插入胡牌判定，如果胡牌，选择语句是吃胡或跳过，如果不能胡牌才是普通的是否吃牌
            int n;
            do {
                std::cout << "请选择是否吃牌：" << std::endl;
                std::cin >> n;
            } while (n < 0 || n > 1);//如果输入选项非法则重新输入
            if (n) {
                Tile discardedTile = chow(tile,a);
                return discardedTile;
            }
        }
        //判断牌库是否只有<=1张牌
        if (mahjong.checkN()) {
            //只有1张牌时，进入抽牌，如果胡牌则游戏结束，否则返回Null
            drawTile(mahjong);
            /*if (checkHu()) {
                std::cout << name << " 胡牌！" << std::endl;
                isHu = true;
            }*/
            return Null;//发现返回Null时，检查是否已胡牌，如果已胡牌则游戏正常结束，若未胡牌则进入流局
        }
        //牌库还有>1张牌时，抽一张牌
        drawTile(mahjong);
        //检查是否胡牌（这里的代码还在完善中）
        /*if (checkHu()) {
            std::cout << name << " 胡牌！" << std::endl;
            isHu = true;
            return Null;
        }
        else
        */
        //若未胡牌，进入弃牌阶段
        Tile discardedTile = discardTile();

        return discardedTile;
    }

    bool chowCheck(Tile& tile) {
        //根据手牌和上家弃牌判断是否能吃牌
        Suit targetSuit = tile.getSuit();
        if (targetSuit == Suit::FENG)return false;//根据规则，风牌不能被吃
        int targetValue = tile.getValue();
        //吃牌达成一共有3种情况，被吃牌分别位于3个位置。其中，第一种情况限定编号1-7，第二种限定编号2-8，第三种限制编号3-9
        //情况1
        if (targetValue >= 1 && targetValue <= 7) {
            bool a = 0, b = 0;//依次判定两个位置
            for (const Tile& t : hand) {
                if (t.getSuit() == targetSuit) {//首先花色符合
                    if (t.getValue() == targetValue + 1)a = true;
                    if (t.getValue() == targetValue + 2)b = true;
                }
            }
            if (a && b)return true;
        }
        //情况2
        if (targetValue >= 2 && targetValue <= 8) {
            bool a = 0, b = 0;//依次判定两个位置
            for (const Tile& t : hand) {
                if (t.getSuit() == targetSuit) {//首先花色符合
                    if (t.getValue() == targetValue - 1)a = true;
                    if (t.getValue() == targetValue + 1)b = true;
                }
            }
            if (a && b)return true;
        }
        //情况3
        if (targetValue >= 3 && targetValue <= 9) {
            bool a = 0, b = 0;//依次判定两个位置
            for (const Tile& t : hand) {
                if (t.getSuit() == targetSuit) {//首先花色符合
                    if (t.getValue() == targetValue - 1)a = true;
                    if (t.getValue() == targetValue - 2)b = true;
                }
            }
            if (a && b)return true;
        }
        return false;
    }

    Tile chow(Tile& tile,Player& c) {
        //虽然这看上去很愚蠢，但为了找出手牌中可配合吃牌的组合，我必须重复chowCheck中的大部分代码。

        Suit targetSuit = tile.getSuit();
        int targetValue = tile.getValue();
        //吃牌达成一共有3种情况，被吃牌分别位于3个位置。我必须考虑到3种情况，当不止一种情况满足时，我需要提供不同的选项已确认通过哪一种情况实现吃牌
        int n = 0;//储存选择数目，若选择数为1则跳过选择
        //储存3种情况下手牌对应位置序号
        int A[3] = { 0,0,0 };
        int B[3] = { 0,0,0 };
        //情况1
        if (targetValue >= 1 && targetValue <= 7) {
            bool a = 0, b = 0;//依次判定两个位置
            int indexA = -1, indexB = -1;
            for (unsigned int index = 0; index < hand.size(); index++) {
                Tile t = hand[index];
                if (t.getSuit() == targetSuit) {//首先花色符合
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
                n++;//可选吃牌方案+1
            }
        }
        //情况2
        if (targetValue >= 2 && targetValue <= 8) {
            bool a = 0, b = 0;//依次判定两个位置
            int indexA = -1, indexB = -1;
            for (unsigned int index = 0; index < hand.size(); index++) {
                Tile t = hand[index];
                if (t.getSuit() == targetSuit) {//首先花色符合
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
                n++;//可选吃牌方案+1
            }
        }
        //情况3
        if (targetValue >= 3 && targetValue <= 9) {
            bool a = 0, b = 0;//依次判定两个位置
            int indexA = -1, indexB = -1;
            for (unsigned int index = 0; index < hand.size(); index++) {
                Tile t = hand[index];
                if (t.getSuit() == targetSuit) {//首先花色符合
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
                n++;//可选吃牌方案+1
            }
        }
        //选择只有一种
        if (n == 1) {
            Tile handTile = hand[A[0]];
            table.push_back(handTile);
            hand.erase(hand.begin() + A[0]);
            handTile = hand[B[0] - 1];//这里有一个警报，因为我们需要输入的是size_t类型，在64位电脑中为8字节，而-1的int运算结果为4字节。我不清楚4字节到8字节是否会产生算术溢出
            table.push_back(handTile);
            hand.erase(hand.begin() + B[0] - 1);//注意，在A擦除一个后，索引位置已改变
            handTile = tile;
            table.push_back(handTile);
            c.discard.pop_back();
            std::sort(table.end() - 3, table.end(), [](const Tile& lhs, const Tile& rhs) {return lhs.getValue() < rhs.getValue(); });//对刚放到桌上的牌排序
        }
        //如果选择大于一种
        if (n > 1) {
            std::cout << "上家弃置的牌是：";
            tile.display();
            std::cout << std::endl;
            std::cout << "请选择一种吃牌方案：" << std::endl;
            for (int i = 0; i < n; i++) {
                hand[A[i]].display();
                hand[B[i]].display();
                std::cout << std::endl;
            }
            int choice;
            do {
                std::cout << "输入方案的索引（从0开始）：";
                std::cin >> choice;
            } while (choice< 0 || choice >= n);//如果输入索引非法则重新输入

            Tile handTile = hand[A[choice]];
            table.push_back(handTile);
            hand.erase(hand.begin() + A[choice]);
            handTile = hand[B[choice] - 1];//这里有一个警报，因为我们需要输入的是size_t类型，在64位电脑中为8字节，而-1的int运算结果为4字节。我不清楚4字节到8字节是否会产生算术溢出
            table.push_back(handTile);
            hand.erase(hand.begin() + B[choice] - 1);//注意，在A擦除一个后，索引位置已改变
            handTile = tile;
            table.push_back(handTile);
            c.discard.pop_back();
            std::sort(table.end() - 3, table.end(), [](const Tile& lhs, const Tile& rhs) {return lhs.getValue() < rhs.getValue(); });//对刚放到桌上的牌排序
        }
        displayHand();
        Tile discardedTile = discardTile();//吃牌后弃牌，相当于取代正常的drawAndDiscard
        return discardedTile;
    }

    bool kongCheck(Tile& tile) {
        bool k = 0; //默认是不能碰
        //这里是根据手牌和任意一家弃牌判断是否能碰牌的判断语句，还没有想好
        return k;
    }

    Tile kong(Tile& tile) {
        //这里是碰场上任意一家的弃牌，并删去对应2张手牌，变成桌牌的语句
        Tile discardedTile = discardTile();//碰牌后弃牌，相当于取代正常的drawAndDiscard
        return discardedTile;
    }

    bool pungCheck(Tile& tile) {
        bool p = 0; //默认是不能杠
        //这里是根据手牌和任意一家弃牌判断是否能杠牌的判断语句，还没有想好(实际实现时可以把这一块和kongCheck合二为一）
        return p;
    }

    Tile pung(Tile& tile, Mahjong& mahjong) {//因为涉及到抽牌，所以需要输入牌堆
        //这里是杠场上任意一家的弃牌，并删去对应2张手牌，变成桌牌的语句（暂不考虑明杠，暗杠，加杠的区别）
        drawTile(mahjong);//杠牌后须先抽一张牌
        Tile discardedTile = discardTile();//杠牌后弃牌，相当于取代正常的drawAndDiscard
        return discardedTile;
    }

    bool operator==(Player& player) {
        if (name == player.name)return true;
        return false;
    }

    std::vector<Tile> table;//桌上的牌，为吃，碰，杠展示的牌
    std::vector<Tile> discard;//弃牌堆
private:
    std::string name;
    bool x;//判断是否为真实玩家，如果x为0，则只会执行摸牌并弃牌操作
    std::vector<Tile> hand;//手中的牌
    bool isHu = false;
};

Player Draw = Player(Draw);//这是不存在的玩家，用来表示流局，当这位玩家获胜时，说明无人获胜

#endif // !MAHJONG_H