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

Tile Null = Tile(Suit::FENG, 0);//这是不存在的风0，用来表示异常状态，它不存在于麻将堆中，又因为风牌不允许吃，所以不会产生吃碰杠的误判断。谨慎使用

class Mahjong {
public:
    //构造函数，构造一副日式麻将牌并打乱
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
        std::mt19937 gen(rd());// 使用Mersenne Twister算法，计算随机数
        std::shuffle(tiles.begin(), tiles.end(), gen);
    }

    //从牌堆抽牌，剩余牌数-1
    Tile drawTile() {
        if (n > 0) {//如果牌堆不为空则抽牌
            Tile tile = tiles.back();
            tiles.pop_back();
            n -= 1;
            return tile;
        }
        else return Null;
    }

    //判断抽牌堆当前牌数是否小于等于1，这是为了防止抽牌非法访问导致的程序崩溃
    bool checkN() {
        if (n <= 1)return true;
        else return false;
    }

private:
    std::vector<Tile> tiles;
    int n=136;//牌堆中剩余牌数，每次执行从牌堆抽牌操作后会-1
};

class Player {
public:
    //构造函数，新建一个玩家，只需提供玩家名
    Player(const std::string& _name, bool _x = 0) {
        name = _name;
        x = _x;
    }

    //将一张牌加入手牌中
    Tile drawTile(Mahjong& mahjong) {
        if (x) {
            //从牌堆摸一张牌
            Tile tile = mahjong.drawTile();
            std::cout << name<< "抽到了：";
            tile.display();
            std::cout << std::endl;
            hand.push_back(tile);
            sortHand();//抽牌后整理手牌
            return tile;
        }
        else{
            Tile tile = mahjong.drawTile();
            hand.push_back(tile);//机器人抽牌不会再显示出来
            sortHand();//抽牌后整理手牌
            return tile;
        }
        
    }

    //抽牌重载，用于开始游戏发牌
    void drawTile(Tile& tile) {
        hand.push_back(tile);
    }

    //展示所有手牌，并在显示中区分普通玩家和白板机器人玩家
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
    }

    //弃置一张手牌，如果是玩家，展示手牌并询问弃牌索引，否则随机弃置一张牌，不显示手牌，但同样显示弃牌后的弃牌堆
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
            int i = hand.size() - 1;
            std::uniform_int_distribution<int> distribution(0, i);

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

    //这是弃牌的重载，为机器学习预留
    Tile discardTile(int i) {
        Tile discardedTile = hand[i];
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

    //整理手牌，使得同花色牌按从小到大排序
    void sortHand() {
        std::sort(hand.begin(), hand.end(), [](const Tile& lhs, const Tile& rhs) {//这里用到了<algorithm>库的sort函数，前面2个参数分别为排序的起点和终点，最后一个参数是排序的规则
            if (lhs.getSuit() == rhs.getSuit()) {
                return lhs.getValue() < rhs.getValue();//如果花色一样，比较大小
            }
            return static_cast<int>(lhs.getSuit()) < static_cast<int>(rhs.getSuit());//这里用到了静态强制类型转换，将4种花色强制转换成int型，我不清楚这是如何转换的，但这样确实实现了万，筒，条，风顺序的排列
            });
    }

    //判断胡牌的方法存在重大问题，需要重写！它只能判断必要性，不能判断充分性，所以会产生大量误判断。更重要的是，它没有考虑到桌上的牌
    bool checkHu() const {
        // 确认是否胡牌（每次有人抽牌，都需要检查是否自摸胡牌，所以正常回合开始或者杠牌后都需要插入胡牌判定，而每次有人弃牌后，场上所有玩家都必须判断是否胡牌）
        std::unordered_map<int, int> countMap;//这种算法有巨大问题，必须更换，因为它统计的实际上是每种牌值的数量，而不是每种牌的数量，所以下面的实现没有意义，但思路部分可以参考
        for (const Tile& tile : hand) {
            countMap[tile.getValue()]++;  // 这个表统计了每种牌值对应牌的数量，pair.first返回这种值，pair.second返回其数量。也就是说，它不会考虑到花色
        }
        //这里只定义了2个变量，实际上可能需要5个变量，还有3个变量分别是桌上的刻子数（3张1组），桌上的杠牌数（4张1组），桌上的顺子数
        int setCount = 0; //这个变量用于记录刻子（三张相同的牌）的数量。我们通过遍历 countMap，将每种牌值的数量除以 3，即可得到刻子的数量。
        int sequenceCount = 0;//这个变量用于记录顺子（连续三张相邻牌值）的数量。我们通过遍历 countMap，对于每种牌值，检查是否存在比它大1，大2的两个牌值，并且这两个牌值的数量都大于等于 1。如果满足这个条件，就说明能组成一个顺子（但不能说明这真的是胡牌所需的顺子，例如123456，234可以组成顺子，但123和456可以组成2组
        for (const auto& pair : countMap) {
            int count = pair.second;//对于每种牌，用count记录牌的数量
            setCount += count / 3;  // 只有当牌数大于等于3时，刻子的数量+1
            if (count >= 3) {
                count -= 3 * (count / 3);//如果这种牌的数量大于或等于3，先减少3张，因为刚才刻子的统计中已经考虑过了，这个思路可以保留
            }
            if (count > 0 && countMap.count(pair.first + 1) && countMap.count(pair.first + 2)) {//如果这种牌和比它大1，大2的都存在，顺子数+1（注意，须考虑到重复计算问题，当前方法无法解决该问题）
                sequenceCount++;
            }
        }

        return setCount + sequenceCount >= 4;  // Check if there are at least 4 sets or sequences
    }

    //判断玩家是否胡牌
    bool isHuPlayer() const {
        return isHu;
    }

    //返回玩家姓名
    std::string getName() const {
        if (x)return name;
        else {
            std::string robot = "(robot)";
            std::string _name = name + robot;
            return _name;
        }
        
    }

    //这个是正常的游戏流程（碰和杠为特殊流程）
    Tile drawAndDiscard(Mahjong& mahjong,Tile& discardedTile,Player& a) {

        //检查是否胡牌（因为函数checkHu存在误报问题，暂时将这一段注释掉，当功能完善后可以恢复）这里胡牌的原因是上家弃牌正好是听牌所需
        /*if (checkHu()) {
            std::cout << name << " 胡牌！" << std::endl;
            isHu = true;
            return Null;//返回异常牌
        }
        else{
            //若未胡牌，进入接下来的部分
        }*/

        //展示自己的手牌
        displayHand();
        //判断是否吃牌
        if (chowCheck(discardedTile)) {
            int n;
            do {
                std::cout << "请选择是否吃牌：" << std::endl;
                std::cin >> n;
            } while (n < 0 || n > 1);//如果输入选项非法则重新输入
            if (n) {
                discardedTile = chow(discardedTile,a);
                return discardedTile;
            }
        }
        //判断牌库是否只有<=1张牌
        if (mahjong.checkN()) {
            //只有1张牌时，抽牌如果胡牌则游戏结束，否则返回Null异常，进入流局
            drawTile(mahjong);
            //检查是否胡牌（因为函数checkHu存在误报问题，暂时将这一段注释掉，当功能完善后可以恢复）这里胡牌的原因是自摸抽到了胡牌所需
            /*if (checkHu()) {
                std::cout << name << " 胡牌！" << std::endl;
                isHu = true;
            }*/
            return Null;//发现返回Null时，检查是否已胡牌，如果已胡牌则游戏正常结束，若未胡牌则进入流局
        }
        //牌库还有>1张牌时，抽一张牌
        Tile tile = drawTile(mahjong);//返回抽到的牌

        //检查是否胡牌（因为函数checkHu存在误报问题，暂时将这一段注释掉，当功能完善后可以恢复）这里胡牌的原因是自摸抽到了胡牌所需
        /*if (checkHu()) {
            std::cout << name << " 胡牌！" << std::endl;
            isHu = true;
            return Null;//返回异常牌
        }
        else{
            //若未胡牌，进入接下来的部分
        }*/

        //检查是否能暗杠
        if (kongHideCheck(tile)) {
            int n;
            do {
                std::cout << "请选择是否暗杠：" << std::endl;
                std::cin >> n;
            } while (n < 0 || n > 1);//如果输入选项非法则重新输入
            if (n) {
                discardedTile = kong(discardedTile, mahjong);
                if (discardedTile == Null)return Null;//因为杠牌同样涉及到抽牌，有自摸胡牌和流局的可能，所以这里同样需要判定，如果返回NUll异常，则继续返回NULL异常
                return discardedTile;
            }
        }
        discardedTile = discardTile();

        return discardedTile;
    }

    //根据手牌和上家弃牌判断是否能吃牌。这里无须加入吃胡判定，因为之前已经判定过了
    bool chowCheck(Tile& tile) {
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

    //吃掉上家所弃的牌。这里无须加入吃胡判定，因为之前已经判定过了
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

    //根据手牌和场上任意一家弃牌判断是否能碰牌
    bool pungCheck(Tile& tile) {
        Suit targetSuit = tile.getSuit();//目标花色
        int targetValue = tile.getValue();//目标值
        int n = 0;//储存计数值
        for (const Tile& t : hand) {
            if (t.getSuit() == targetSuit) {//首先花色符合
                if (t.getValue() == targetValue) n++;//如果花色和数值都符合，则计数+1
            }
        }
        if (n >= 2) return true;
        return false;
    }

    //这里是碰场上任意一家的弃牌，并删去对应2张手牌，变成桌牌的语句
    Tile pung(Tile& tile, Player& c) {
        Suit targetSuit = tile.getSuit();//目标花色
        int targetValue = tile.getValue();//目标值
        int index = 0;//储存索引值
        for (unsigned int i = 0; i < hand.size(); i++) {
            Tile t = hand[i];
            if (t.getSuit() == targetSuit) {//首先花色符合
                if (t.getValue() == targetValue) {
                    index = i;//如果花色和数值都符合，则储存索引值
                }
            }
        }
        Tile handTile = hand[index];
        for (int i = 0; i < 2; i++){
            table.push_back(handTile);
            hand.erase(hand.begin() + index - i);//依次删除两张手牌
        }
        table.push_back(tile);
        c.discard.pop_back();//从玩家的弃牌堆移除被碰的牌

        Tile discardedTile = discardTile();//碰牌后弃牌，相当于取代正常的drawAndDiscard
        return discardedTile;
    }

    //根据手牌和输入牌判断是否能杠牌
    bool kongCheck(Tile& tile) {
        Suit targetSuit = tile.getSuit();//目标花色
        int targetValue = tile.getValue();//目标值
        int n = 0;//储存计数值
        for (const Tile& t : hand) {
            if (t.getSuit() == targetSuit) {//首先花色符合
                if (t.getValue() == targetValue) n++;//如果花色和数值都符合，则计数+1
            }
        }
        if (n >= 3) return true;
        return false;
    }

    //判断手牌和抽到牌是否满足暗杠要求
    bool kongHideCheck(Tile& tile) {
        Suit targetSuit = tile.getSuit();//目标花色
        int targetValue = tile.getValue();//目标值
        int n = 0;//储存计数值
        for (const Tile& t : hand) {
            if (t.getSuit() == targetSuit) {//首先花色符合
                if (t.getValue() == targetValue) n++;//如果花色和数值都符合，则计数+1
            }
        }
        if (n >= 4) return true;//暗杠需要抽牌后手上相同种牌达到4张，且为该牌
        return false;
    }

    //每次抽牌后都必须考虑能否加杠，包括杠牌内部的抽牌（还未添加到正式语句中，希望胡牌判定实现后再添加）
    bool kongPlusCheck(Tile& tile) {
        Suit targetSuit = tile.getSuit();//目标花色
        int targetValue = tile.getValue();//目标值
        int n = 0;//储存计数值
        for (const Tile& t : table) {//这里因为我对麻将的理解不全面，这一段判定标准可能有错
            if (t.getSuit() == targetSuit) {//首先花色符合
                if (t.getValue() == targetValue) n++;//如果花色和数值都符合，则计数+1
            }
        }
        if (n == 3) return true;
        return false;
    }

    //这里是明杠
    Tile kong(Tile& tile, Mahjong& mahjong, Player& c) {//因为涉及到抽牌，所以需要输入牌堆；因为涉及到弃牌堆，所以需要输入其他玩家
        Suit targetSuit = tile.getSuit();//目标花色
        int targetValue = tile.getValue();//目标值
        int index = 0;//储存索引值
        for (unsigned int i = 0; i < hand.size(); i++) {
            Tile t = hand[i];
            if (t.getSuit() == targetSuit) {//首先花色符合
                if (t.getValue() == targetValue) {
                    index = i;//如果花色和数值都符合，则储存索引值
                }
            }
        }
        for (int i = 0; i < 3; i++) {
            table.push_back(tile);
            hand.erase(hand.begin() + index - i);//依次删除三张手牌
        }
        table.push_back(tile);
        c.discard.pop_back();//从玩家的弃牌堆移除被杠的牌

        drawTile(mahjong);//杠牌后须先抽一张牌
        Tile discardedTile = discardTile();//杠牌后弃牌，相当于取代正常的drawAndDiscard
        return discardedTile;
    }

    //这里是暗杠（重载）
    Tile kong(Tile& tile, Mahjong& mahjong) {//因为涉及到抽牌，所以需要输入牌堆

        Suit targetSuit = tile.getSuit();//目标花色
        int targetValue = tile.getValue();//目标值
        int index = 0;//储存索引值
        for (unsigned int i = 0; i < hand.size(); i++) {
            Tile t = hand[i];
            if (t.getSuit() == targetSuit) {//首先花色符合
                if (t.getValue() == targetValue) {
                    index = i;//如果花色和数值都符合，则储存索引值
                }
            }
        }
        //因为在暗杠前已经完成抽牌，手牌数量为14张
        for (int i = 0; i < 4; i++) {
            table.push_back(tile);//依次往桌上放牌
            hand.erase(hand.begin() + index - i);//依次删除四张手牌
        }

        //杠牌后须先抽一张牌
        
        //判断牌库中剩余牌是否够抽牌
        if (mahjong.checkN()) {
            drawTile(mahjong);

            //检查是否胡牌（因为函数checkHu存在误报问题，暂时将这一段注释掉，当功能完善后可以恢复）这里胡牌的原因是自摸抽到了胡牌所需
            /*if (checkHu()) {
                std::cout << name << " 胡牌！" << std::endl;
                isHu = true;
                return Null;//返回异常牌
            }
            else{
                //若未胡牌，进入接下来的部分，即return Null;
            }*/

            
            return Null;//因为在牌库只剩最后一张牌的情况下抽牌未胡，返回异常，进入流局
        }
        //检查是否胡牌（因为函数checkHu存在误报问题，暂时将这一段注释掉，当功能完善后可以恢复）这里胡牌的原因是自摸抽到了胡牌所需
            /*if (checkHu()) {
                std::cout << name << " 胡牌！" << std::endl;
                isHu = true;
                return Null;//返回异常牌
            }
            else{
                //若未胡牌，进入接下来的部分
            }*/

        Tile discardedTile = discardTile();//杠牌后弃牌，相当于取代正常的drawAndDiscard
        return discardedTile;
    }

    //这里是加杠（未注释掉胡牌判定部分，谨慎使用）
    Tile kongPlus(Tile& tile, Mahjong& mahjong) {
        Suit targetSuit = tile.getSuit();//目标花色
        int targetValue = tile.getValue();//目标值
        int index = 0;//储存索引值
        for (unsigned int i = 0; i < table.size(); i++) {//这段代码涉及到判定标准，我的理解有歧义，群里问了没人理我
            Tile t = table[i];
            if (t.getSuit() == targetSuit) {//首先花色符合
                if (t.getValue() == targetValue) {
                    index = i;//如果花色和数值都符合，则储存索引值
                }
            }
        }
        for (int i = 0; i < 3; i++) {
            table.push_back(tile);//依次往桌上最右端放牌
            hand.erase(hand.begin() + index - i);//依次删除原先桌上碰的牌
        }
        table.push_back(tile);//把刚抽到的牌也放到桌上

        //杠牌后须先抽一张牌
        
        //判断牌库中剩余牌是否够抽牌
        if (mahjong.checkN()) {
            drawTile(mahjong);

            //检查是否胡牌（因为函数checkHu存在误报问题，暂时将这一段注释掉，当功能完善后可以恢复）这里胡牌的原因是自摸抽到了胡牌所需
            if (checkHu()) {
                std::cout << name << " 胡牌！" << std::endl;
                isHu = true;
                return Null;//返回异常牌
            }
            else{
                return Null;//因为在牌库只剩最后一张牌的情况下抽牌未胡，返回异常，进入流局
            }
        }

        //检查是否胡牌（因为函数checkHu存在误报问题，暂时将这一段注释掉，当功能完善后可以恢复）这里胡牌的原因是自摸抽到了胡牌所需
            if (checkHu()) {
                std::cout << name << " 胡牌！" << std::endl;
                isHu = true;
                return Null;//返回异常牌
            }
            else{
                Tile discardedTile = discardTile();//杠牌后弃牌，相当于取代正常的drawAndDiscard
                return discardedTile;
            }
    }
    bool operator==(Player& player) {//这个重载用于判断流局
        if (name == player.name)return true;
        return false;
    }

    //桌上的牌，为吃，碰，杠展示的牌。放在public区是因为加杠操作时，其他玩家有权拿走这部分的牌用于胡牌
    std::vector<Tile> table;

    //弃牌堆。放在public区是因为吃，碰，明杠操作中，其他玩家有权拿走这部分的牌用于胡牌
    std::vector<Tile> discard;
private:
    std::string name;
    bool x;//判断是否为真实玩家，如果x为0，则只会执行摸牌并弃牌操作
    std::vector<Tile> hand;//手中的牌，任何情况下其他玩家都无权限操作或查看手牌
    bool isHu = false;//胡牌属性，在遇到异常时，优先检查该属性
    //后续或许会添加其他属性，用于记录玩家的各种数据和状态
    //数据比如，吃牌数，杠牌数，碰牌数，被吃牌数，被杠牌数，被碰牌数。后三者可放在public区，便于其他玩家访问修改
    //状态可由上述数据体现，但如果确实有必要，可以添加听牌状态，门前清状态等，便于跳过某些无意义的判定。这些状态还可以作为显示输出，提醒玩家。
};

Player Draw = Player(Draw);//这是不存在的玩家，用来表示流局，当这位玩家获胜时，说明无人获胜

#endif // !MAHJONG_H