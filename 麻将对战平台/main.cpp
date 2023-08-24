#include"Mahjong.h"

Player gameTurn(std::vector<Player> players, Mahjong mahjong,int gameTurns = 0, int playerIndex = 0,Tile discardedTile= Null) {//Ĭ�ϴӵ�0�֣���0����ҿ�ʼ���Բ�Ӱ�����������͸�ֱ�ӽ�����һ��
    std::cout << "��" << gameTurns << "����Ϸ��ʼ" << std::endl;
    for (int Index=0; Index < 4; Index++) {//ÿ�����������ʼ�غ�,��4��ΪһȦ����;���Ȧ��+1

        int gameIndex = playerIndex + Index;//playerIndex��ʾ��һ����Ϸ��˭��ʼ����������£���һ�ֻ���������ҿ�ʼ;gameIndex������forѭ����Ӧ���
        if (gameIndex > 3)gameIndex -= 4;
        int lastIndex = gameIndex - 1;//��ʾ������ҵ��ϼ�
        if (lastIndex < 0)lastIndex += 4;
        discardedTile = players[gameIndex].drawAndDiscard(mahjong,discardedTile,players[lastIndex]);//��ǰ��ҽ���1.�жϳ��� 2.�Բ��˾ͳ��� 3.�жϺ��ƣ����������ƽ׶Σ��������������ơ�����discardedTile�ܹؼ����غ������ܵ��ж�
        if (players[gameIndex].isHuPlayer()) {
            std::cout << "����" << gameTurns << "�ֳַ�ʤ��" << std::endl;
            return players[gameIndex];//�����Һ��ƣ��Ժ�������������Ϸ���������ػ�ʤ�������
        }
        if (discardedTile == Null)return Draw;//���discardTile����Null��û����Һ��ƣ�����������ҡ����֡�

        //�����Ϸû�н���������ʣ��3����Ҽ���Ƿ��������߸�
        for (int i = 1; i <= 3; i++) {
            int checkIndex = gameIndex + i;
            if (checkIndex > 3)checkIndex -= 4;
            if (players[checkIndex].kongCheck(discardedTile)) {
                int n;
                do {
                    std::cout << "��ѡ���Ƿ���ƣ�" << std::endl;
                    players[checkIndex].displayHand();
                    std::cin >> n;
                } while (n < 0 || n > 1);//�������ѡ��Ƿ�����������
                if (n) {
                    discardedTile = players[checkIndex].kong(discardedTile,mahjong, players[gameIndex]);//����ͬ���漰���ƣ���Ҫ�ж��ƿ��Ƿ�Ϊ�գ����Ϊ�����������
                    if (players[checkIndex].isHuPlayer()) {
                        std::cout << "����" << gameTurns << "�ֳַ�ʤ��" << std::endl;
                        return players[checkIndex];//�����Һ��ƣ��ܺ�������Ϸ���������ػ�ʤ�������
                    }
                    if (discardedTile == Null)return Draw;//���discardTile����Null��û����Һ��ƣ�����������ҡ����֡�

                    return gameTurn(players, mahjong, gameTurns + 1, checkIndex + 1, discardedTile);//���������û��������+1���ӵ�ǰ��ҵ��¼ҿ�ʼ�µ�һ��
                }
            }
            if (players[checkIndex].pungCheck(discardedTile)) {
                int n;
                do {
                    std::cout << "��ѡ���Ƿ����ƣ�" << std::endl;
                    players[checkIndex].displayHand();
                    std::cin >> n;
                } while (n < 0 || n > 1);//�������ѡ��Ƿ�����������
                if (n) {
                    discardedTile = players[checkIndex].pung(discardedTile, players[gameIndex]);//��Ҫ���뵱ǰ�����ţ��Ա�ɾ�������ƶ��ж�Ӧ��
                    if (players[checkIndex].isHuPlayer()) {
                        std::cout << "����" << gameTurns << "�ֳַ�ʤ��" << std::endl;
                        return players[checkIndex];//�����Һ��ƣ�����������Ϸ���������ػ�ʤ�������
                    }
                    return gameTurn(players, mahjong, gameTurns + 1, checkIndex+1, discardedTile);//���������û��������+1���ӵ�ǰ��ҵ��¼ҿ�ʼ�µ�һ��
                }
            }
        }
        //���û����Ҹ��ƻ����ƣ������ֵ���һ��ң������ж�����drawAndDiscard����
    }
    return gameTurn(players, mahjong, gameTurns + 1, playerIndex, discardedTile);
}

int main() {
    Mahjong mahjong;//�½�һ���齫
    std::vector<Player> players;//�½�4�����
    players.emplace_back("���1",1);//��ʵ����뱸ע��1����Ĭ��Ϊ�װ�
    players.emplace_back("���2");
    players.emplace_back("���3");
    players.emplace_back("���4");
    //ÿ������������ƶ����ƣ���13Ȧ
    for (int i = 0; i < 13; ++i) {
        for (auto& player : players) {
            Tile tile=mahjong.drawTile();
            player.drawTile(tile);
        }
    }
    //ÿ����������Լ�������
    for (auto& player : players) {
        player.sortHand();
    }
    Player winner = gameTurn(players, mahjong);//�����ĸ���Һ�һ���齫���ɿ�ʼ��Ϸ
    if (winner == Draw)std::cout << "���֣����˻�ʤ";
    return 0;
}