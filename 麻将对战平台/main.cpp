#include"Mahjong.h"

Player gameTurn(std::vector<Player> players, Mahjong mahjong,int gameTurns = 0, int playerIndex = 0,Tile discardedTile= Null) {//默认从第0轮，第0名玩家开始，吃不影响轮数，碰和杠直接进入下一轮
    std::cout << "第" << gameTurns << "轮游戏开始" << std::endl;
    for (int Index=0; Index < 4; Index++) {//每个玩家轮流开始回合,走4下为一圈，中途打断圈数+1

        int gameIndex = playerIndex + Index;//playerIndex表示这一轮游戏由谁开始，正常情况下，下一轮还由这名玩家开始;gameIndex代表本次for循环对应玩家
        if (gameIndex > 3)gameIndex -= 4;
        int lastIndex = gameIndex - 1;//表示本轮玩家的上家
        if (lastIndex < 0)lastIndex += 4;
        discardedTile = players[gameIndex].drawAndDiscard(mahjong,discardedTile,players[lastIndex]);//当前玩家进入1.判断吃牌 2.吃不了就抽牌 3.判断胡牌，胡不了弃牌阶段，胡牌则跳过弃牌。变量discardedTile很关键，关乎吃碰杠的判断
        if (players[gameIndex].isHuPlayer()) {
            std::cout << "共用" << gameTurns << "轮分出胜负" << std::endl;
            return players[gameIndex];//如果玩家胡牌（吃胡或自摸），游戏结束，返回获胜玩家姓名
        }
        if (discardedTile == Null)return Draw;//如果discardTile出现Null但没有玩家胡牌，返回虚拟玩家“流局”

        //如果游戏没有结束，则场上剩下3名玩家检查是否能碰或者杠
        for (int i = 1; i <= 3; i++) {
            int checkIndex = gameIndex + i;
            if (checkIndex > 3)checkIndex -= 4;
            if (players[checkIndex].kongCheck(discardedTile)) {
                int n;
                do {
                    std::cout << "请选择是否杠牌：" << std::endl;
                    players[checkIndex].displayHand();
                    std::cin >> n;
                } while (n < 0 || n > 1);//如果输入选项非法则重新输入
                if (n) {
                    discardedTile = players[checkIndex].kong(discardedTile,mahjong, players[gameIndex]);//杠牌同样涉及摸牌，需要判定牌库是否为空，如果为空则进入流局
                    if (players[checkIndex].isHuPlayer()) {
                        std::cout << "共用" << gameTurns << "轮分出胜负" << std::endl;
                        return players[checkIndex];//如果玩家胡牌（杠胡），游戏结束，返回获胜玩家姓名
                    }
                    if (discardedTile == Null)return Draw;//如果discardTile出现Null但没有玩家胡牌，返回虚拟玩家“流局”

                    return gameTurn(players, mahjong, gameTurns + 1, checkIndex + 1, discardedTile);//如果杠牌且没胡，轮数+1，从当前玩家的下家开始新的一轮
                }
            }
            if (players[checkIndex].pungCheck(discardedTile)) {
                int n;
                do {
                    std::cout << "请选择是否碰牌：" << std::endl;
                    players[checkIndex].displayHand();
                    std::cin >> n;
                } while (n < 0 || n > 1);//如果输入选项非法则重新输入
                if (n) {
                    discardedTile = players[checkIndex].pung(discardedTile, players[gameIndex]);//需要传入当前玩家序号，以便删除其弃牌堆中对应牌
                    if (players[checkIndex].isHuPlayer()) {
                        std::cout << "共用" << gameTurns << "轮分出胜负" << std::endl;
                        return players[checkIndex];//如果玩家胡牌（碰胡），游戏结束，返回获胜玩家姓名
                    }
                    return gameTurn(players, mahjong, gameTurns + 1, checkIndex+1, discardedTile);//如果碰牌且没胡，轮数+1，从当前玩家的下家开始新的一轮
                }
            }
        }
        //如果没有玩家杠牌或碰牌，正常轮到下一玩家，吃牌判定放在drawAndDiscard环节
    }
    return gameTurn(players, mahjong, gameTurns + 1, playerIndex, discardedTile);
}

int main() {
    Mahjong mahjong;//新建一副麻将
    std::vector<Player> players;//新建4个玩家
    players.emplace_back("玩家1",1);//真实玩家须备注“1”，默认为白板
    players.emplace_back("玩家2");
    players.emplace_back("玩家3");
    players.emplace_back("玩家4");
    //每个玩家轮流中牌堆摸牌，摸13圈
    for (int i = 0; i < 13; ++i) {
        for (auto& player : players) {
            Tile tile=mahjong.drawTile();
            player.drawTile(tile);
        }
    }
    //每个玩家整理自己的手牌
    for (auto& player : players) {
        player.sortHand();
    }
    Player winner = gameTurn(players, mahjong);//输入四个玩家和一副麻将即可开始游戏
    if (winner == Draw)std::cout << "流局，无人获胜";
    return 0;
}