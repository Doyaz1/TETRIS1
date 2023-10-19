class GameTable{
private:
    int x; // ����
    int y; // ����
    Block* blockObject;
    std::vector<std::vector<int>> table;  // ���̺� �� 0 = " ", 1 = "��" , 2 = "��"
    // ���� ���� ����� table[y][x]

public:
    GameTable(int x, int y){  // ��Ʈ���� �� ���� ����
      this -> x = x;
      this -> y = y;

      for(int i = 0; i < y; i++){
        std::vector<int> temp;
        for(int j = 0; j < x; j++){
          temp.push_back(0);
        }
        table.push_back(temp);
      }

      // ���� �ڸ� ���� ��ĥ
      for(int i = 0; i < x; i++){
        table[0][i] = 1;
        table[y - 1][i] = 1;
      }

      for(int i = 1; i < y - 1; i++){
        table[i][0] = 1;
        table[i][x - 1] = 1;
      }      

      /* �� �عٴ� ������ 4 */
      for(int i = 1; i < x - 1; i++){
        table[y - 1][i] = 4;             // �� ���� ���� 4
      }

      for(int i = 1; i < x - 1; i++){
        table[END_Y][i] = 5;             // ���� ���� �� ���� 5
      }
    }


    // ������ �׸��� �Լ�
    void DrawGameTable(){
      for(int i = 0; i < y; i++){
        for(int j = 0; j < x; j++){
          if(table[i][j] == 1 || table[i][j] == 4){
            std::cout << "��";
          }
          else if(table[i][j] == 2 || table[i][j] == 3){
            std::cout << "��";
          }
          else if(table[i][j] == 5){
            std::cout << "��";
          }
          else{
            std::cout << "  ";
          }
        }
        std::cout << "\n";
      }
    }
    
    /* ���� ���� */
    void createBlock(){
        srand((unsigned int)time(NULL));
        int select = rand() % 5 + 1;        // 1 ~ 5 ����
        
        if(select == 1){
            blockObject = new Block1();     // 1�� ���� ����
        }
        else if(select == 2){
            blockObject = new Block2();     // 2�� ���� ����
        }
        else if(select == 3){
            blockObject = new Block3();     // 3�� ���� ����
        }    
        else if(select == 4){
            blockObject = new Block4();     // 4�� ���� ����
        }    
        else if(select == 5){
            blockObject = new Block5();     // 5�� ���� ����
        }    

        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                int Y = j + blockObject -> getY();
                int X = i + blockObject -> getX();
                
                if(Y < 0 || X < 0 || Y >= TABLE_Y || X >= TABLE_X){
                    continue;           // array out of range ����
                }

                table[Y][X] = blockObject -> getShape(blockObject -> getRotationCount(), i, j);
              // ������ ��ü ���̺��� ������Ʈ
            }
        }
    }


    /* ���� �̵� */
    void MoveBlock(int key){
        /* ��� */
        Block backupBlock;          // ����� Block ��ü
        vector<vector<int>> backupTable;        // ����� table vector
        backup::updateBlock(blockObject, backupBlock);  // block ���
        backup::updateTable(table, backupTable);        // table ���

        /* ���̺����� ���� ��ü ����� */
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                int Y = j + blockObject -> getY();
                int X = i + blockObject -> getX();
                    
                if(table[Y][X] == 2){
                    table[Y][X] = 0; // ���� �����̸� ���̺����� �����
                }
            }
        }

        /* ���� �̵� */
        if(key == DOWN) blockObject->down(); // ���� ���ڷ� ���� key�� �Ʒ� �����̸� ������ �Ʒ��� �̵�
        else if(key == LEFT) blockObject->left();  // ���� ���ڷ� ���� key�� ���� �����̸� ������ �������� �̵�
        else if (key == RIGHT) blockObject->right(); // ���� ���ڷ� ���� key�� ������ �����̸� ������ ���������� �̵�
 
        /*�̵��� ���� ���¸� ���̺��� ���� or ��� */
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int Y = j + blockObject->getY();
                int X = i + blockObject->getX();
                int blockValue = blockObject -> getShape(blockObject -> getRotationCount(), i, j);
                // ���� �迭�� ���

                if(Y < 0 || X < 0 || Y >= TABLE_Y || X >= TABLE_X){
                    continue;           // array out of range ����
                }
                

                if(blockValue != 2){
                    continue;           // ������ �ƴϸ� ���� (������ 2�� �̷���� �ִ�.)
                }

                if(table[Y][X] == 0){               // �� �����̸� (����)
                    table[Y][X] = blockValue;       // ������ �̵���Ų��.
                }      
                
                else if(table[Y][X] == 1) { //������� ��쿡 �̵��� ���� ������ ���̺��� ����
                    copy(backupTable.begin(), backupTable.end(), table.begin());    // table ���
                    blockObject -> setX(backupBlock.getX());        // ���� x ��ǥ ���
                    blockObject -> setY(backupBlock.getY());        // ���� y ��ǥ ���
                    return;    // ������Ʈ ���
                }
                else if(table[Y][X] == 3) { //������� ��쿡 �̵��� ���� ������ ���̺��� ����
                    copy(backupTable.begin(), backupTable.end(), table.begin());    // table ���
                    blockObject -> setX(backupBlock.getX());        // ���� x ��ǥ ���
                    blockObject -> setY(backupBlock.getY());        // ���� y ��ǥ ���

                    if(key == DOWN){  // ���� �Ʒ� ������ ���
                      BuildBlock();   // ������ �װ�  
                      createBlock();  // ���ο� ������ �����.
                    }
                    return;    // ������Ʈ ���
                }

                else if(table[Y][X] == 4) { //������� ��쿡 �̵��� ���� ������ ���̺��� ����
                    copy(backupTable.begin(), backupTable.end(), table.begin());    // table ���
                    blockObject -> setX(backupBlock.getX());        // ���� x ��ǥ ���
                    blockObject -> setY(backupBlock.getY());        // ���� y ��ǥ ���
                    
                    if(key == DOWN){
                      BuildBlock();
                      createBlock();
                    }
                    return;    // ������Ʈ ���
                }
            }
        }
    }
}

/* ���� ȸ�� */
void RotateBlock(){
    /* ��� */
    Block backupBlock;                                  // ����� Block ��ü
    vector<vector<int>> backupTable;                    // ����� table vector
    backup::updateBlock(blockObject, backupBlock);      // block ���
    backup::updateTable(table, backupTable);            // table ���

    /*���̺����� ���� ��ü �����*/
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int Y = j + blockObject->getY();
            int X = i + blockObject->getX();
            
            if (table[Y][X] == 2) {     // ���� �����̸�
                table[Y][X] = 0;        // ���̺����� �����
            }
        }
    }

    /* ���� ȸ�� */
    blockObject -> rotate();        //������ ȸ��

    /*ȸ���� ���� ���¸� ���̺��� ���� �� ��� */
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int Y = j + blockObject->getY();
            int X = i + blockObject->getX();
            int blockValue = blockObject -> getShape(blockObject -> getRotationCount(), i, j);  // ���� �迭 �� ���

            if(blockValue != 2){
                continue;               // ������ �ƴϸ� ���� (������ 2�� �̷���� �ִ�.)
            }
            if(table[Y][X] == 0){
                table[Y][X] = blockObject -> getShape(blockObject -> getRotationCount(), i, j);
            }

            else if(table[Y][X] == 1){
                copy(backupTable.begin(), backupTable.end(), table.begin());        // table ���
                blockObject -> setRotationCount(backupBlock.getRotationCount());    // ȸ���ϱ� �� ���·� ���
                return;     // ������Ʈ ���
            }
        }
    }
  
    /* ������ table�� �ױ� */
    void BuildBlock(){
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
          int Y = j + blockObject -> getY();
          int X = i + blockObject -> getX();

          if(Y < 0 || X < 0 || Y >= TABLE_Y || X >= TABLE_X){
            continue;           // array out of range ����
          }

          int blockValue = blockObject -> getShape(blockObject -> getRotationCount(), i, j); // ���� �迭 �� ���

          if(blockValue != 2){
            continue;
          }
          table[Y][X] = 3;
        }
      }
    }
    /* �����̽��� ���� �� �ٷ� �������� */
    void HardDropBlock() {
      /* ���̺����� ���� ��ü ����� */
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
          int Y = j + blockObject -> getY();
          int X = i + blockObject -> getX();

          if(Y < 0 || X < 0 || Y >= TABLE_Y || X >= TABLE_X){
            continue;           // array out of range ����
          }

          if(table[Y][X] == 2){  // ���� �����̸�
            table[Y][X] = 0;     // ���̺����� �����
          }
        }
      }
      while(true){
        for(int i = 0; i < 4; i++){
          for(int j = 0; j < 4; j++){
            int Y = j + blockObject -> getY();
            int X = i + blockObject -> getX();
            
            if(Y < 0 || X < 0 || Y >= TABLE_Y || X >= TABLE_X){
                continue;           // array out of range ����
            }
            
            int blockValue = blockObject -> getShape(blockObject -> getRotationCount(), i, j); // ���� �迭 �� ���
            
            if(blockValue != 2){
              continue;    // ������ �ƴϸ� ���� (������ 2�� �̷���� �ִ�.)
            }

            if(table[Y][X] == 3 || table[Y][X] == 4){   // �����̳� ���� ������
              blockObject -> up();                      // �� ĭ ���� �ø���
              BuildBlock();                             // ������ �װ�
              createBlock();                            // ���ο� ������ ����
              return;                                   // �Լ� ����
            }
          }
        }
        blockObject -> down();  // ������ �� ĭ �Ʒ��� �̵�
    }
}

/* ������ ���� */
void DeleteLinear(){
    for(int Y = END_Y + 1; Y < TABLE_Y - 1; Y++){
        bool isLinear = true;
        for(int X = 1; X < TABLE_X - 1; X++){
            if(table[Y][X] != 3){
                isLinear = false;
            }

            if(isLinear){
                for(int i = Y; i > END_Y - 1; i--){
                    for(int j = 1; j < TABLE_X - 1; j++){
                        table[i][j] = table[i - 1][j];
                    }
                }
            }
        }
    }
    bool isReachEnding(){
        for(int X = 1; X < TABLE_X - 1; X++){
            if(table[END_Y][X] == 3){
                return true;
            }
        }
        return false;
    }
};

/*���� ���� Ŭ����*/
class GamePlay {
private:
    GameTable* gt;
public:
    GamePlay() {
        gt = new GameTable(TABLE_X, TABLE_Y); //���� �� �׸��� ��ü ����
        gt -> createBlock();                  // �ʱ� ���� ����
        gt -> DrawGameTable();                // �������� �׸���.
        while (true) {                        // ����Ű �Է� �̺�Ʈ
            int nSelect;
            if (_kbhit()) {
                nSelect = _getch();
                if (nSelect == 224) {
                    nSelect = _getch();
                    switch (nSelect) {
                    case UP: // ȭ��ǥ �� ������ ��
                        gt -> RotateBlock(); // ������ 90�� ȸ�� 
                        gotoxy(0, 0); //system("cls") �Ⱦ��� (0, 0)���� Ŀ�� �̵� ��
                        gt -> DrawGameTable(); // ����� 
                        break;
                    
                    case DOWN: // ȭ��ǥ �Ʒ� ������ ��
                        gt -> MoveBlock(DOWN); // ������ �� ĭ �Ʒ��� �̵�
                        gotoxy(0, 0);
                        gt -> DrawGameTable();
                        break;
                    
                    case LEFT: // ȭ��ǥ ���� ������ ��
                        gt -> MoveBlock(LEFT); // ������ �� ĭ �������� �̵�
                        gotoxy(0, 0);
                        gt -> DrawGameTable();
                        break;
                    
                    case RIGHT: // ȭ��ǥ ������ ������ ��
                        gt -> MoveBlock(RIGHT); // ������ �� ĭ ���������� �̵�
                        gotoxy(0, 0);
                        gt -> DrawGameTable();
                        break;
                    
                    default:
                        break;
                    }
                }
            }
            gotoxy(0, 0);            // system("cls") �Ⱦ��� (0, 0) Ŀ�� �̵� ��
            gt -> DrawGameTable();   // �ٽ� �׸���
        }
    }
    ~GamePlay() { // ���� ���� �̺�Ʈ
        system("cls");
        gotoxy(10, 10);
        cout << "GAME OVER";
        delete gt;
    }
};
int main(void) {
    CursorView(false); // �ܼ� ȭ�� Ŀ�� ���� 
    system("mode con cols=100 lines=40 | title ��Ʈ���� ����"); // �ܼ�â ũ�� �� ���� ����
    GameTable gt(TABLE_X, TABLE_Y); 
    MainMenu(); // ���� �޴� �׸��� ������ ȣ��
    GamePlay(); // ���� �÷���
    getchar();
      
    return 0;
}