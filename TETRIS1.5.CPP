class GameTable{
private:
    int x; // 가로
    int y; // 세로
    Block* blockObject;
    std::vector<std::vector<int>> table;  // 테이블 판 0 = " ", 1 = "▦" , 2 = "▦"
    // 블럭 정보 저장시 table[y][x]

public:
    GameTable(int x, int y){  // 테트리스 판 뼈대 생성
      this -> x = x;
      this -> y = y;

      for(int i = 0; i < y; i++){
        std::vector<int> temp;
        for(int j = 0; j < x; j++){
          temp.push_back(0);
        }
        table.push_back(temp);
      }

      // 가장 자리 뼈대 색칠
      for(int i = 0; i < x; i++){
        table[0][i] = 1;
        table[y - 1][i] = 1;
      }

      for(int i = 1; i < y - 1; i++){
        table[i][0] = 1;
        table[i][x - 1] = 1;
      }      

      /* 맨 밑바닥 감지용 4 */
      for(int i = 1; i < x - 1; i++){
        table[y - 1][i] = 4;             // 맨 밑의 값을 4
      }

      for(int i = 1; i < x - 1; i++){
        table[END_Y][i] = 5;             // 게임 종료 선 값을 5
      }
    }


    // 게임판 그리는 함수
    void DrawGameTable(){
      for(int i = 0; i < y; i++){
        for(int j = 0; j < x; j++){
          if(table[i][j] == 1 || table[i][j] == 4){
            std::cout << "▦";
          }
          else if(table[i][j] == 2 || table[i][j] == 3){
            std::cout << "■";
          }
          else if(table[i][j] == 5){
            std::cout << "◎";
          }
          else{
            std::cout << "  ";
          }
        }
        std::cout << "\n";
      }
    }
    
    /* 블럭 생성 */
    void createBlock(){
        srand((unsigned int)time(NULL));
        int select = rand() % 5 + 1;        // 1 ~ 5 블럭
        
        if(select == 1){
            blockObject = new Block1();     // 1번 블럭 생성
        }
        else if(select == 2){
            blockObject = new Block2();     // 2번 블럭 생성
        }
        else if(select == 3){
            blockObject = new Block3();     // 3번 블럭 생성
        }    
        else if(select == 4){
            blockObject = new Block4();     // 4번 블럭 생성
        }    
        else if(select == 5){
            blockObject = new Block5();     // 5번 블럭 생성
        }    

        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                int Y = j + blockObject -> getY();
                int X = i + blockObject -> getX();
                
                if(Y < 0 || X < 0 || Y >= TABLE_Y || X >= TABLE_X){
                    continue;           // array out of range 방지
                }

                table[Y][X] = blockObject -> getShape(blockObject -> getRotationCount(), i, j);
              // 블럭을 객체 테이블에 업데이트
            }
        }
    }


    /* 블럭 이동 */
    void MoveBlock(int key){
        /* 백업 */
        Block backupBlock;          // 백업용 Block 객체
        vector<vector<int>> backupTable;        // 백업용 table vector
        backup::updateBlock(blockObject, backupBlock);  // block 백업
        backup::updateTable(table, backupTable);        // table 백업

        /* 테이블에서 블럭 객체 지우기 */
        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                int Y = j + blockObject -> getY();
                int X = i + blockObject -> getX();
                    
                if(table[Y][X] == 2){
                    table[Y][X] = 0; // 만약 블럭이면 테이블에서 지운다
                }
            }
        }

        /* 블럭 이동 */
        if(key == DOWN) blockObject->down(); // 만약 인자로 들어온 key가 아랫 방향이면 블럭을 아래로 이동
        else if(key == LEFT) blockObject->left();  // 만약 인자로 들어온 key가 왼쪽 방향이면 블럭을 왼쪽으로 이동
        else if (key == RIGHT) blockObject->right(); // 만약 인자로 들어온 key가 오른쪽 방향이면 블럭을 오른쪽으로 이동
 
        /*이동한 블럭 상태를 테이블에 갱신 or 취소 */
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int Y = j + blockObject->getY();
                int X = i + blockObject->getX();
                int blockValue = blockObject -> getShape(blockObject -> getRotationCount(), i, j);
                // 블럭 배열값 얻기

                if(Y < 0 || X < 0 || Y >= TABLE_Y || X >= TABLE_X){
                    continue;           // array out of range 방지
                }
                

                if(blockValue != 2){
                    continue;           // 블럭이 아니면 무시 (블럭은 2로 이루어져 있다.)
                }

                if(table[Y][X] == 0){               // 빈 공간이면 (갱신)
                    table[Y][X] = blockValue;       // 블럭을 이동시킨다.
                }      
                
                else if(table[Y][X] == 1) { //빈공간인 경우에 이동한 블럭 정보를 테이블에 갱신
                    copy(backupTable.begin(), backupTable.end(), table.begin());    // table 백업
                    blockObject -> setX(backupBlock.getX());        // 블럭 x 좌표 백업
                    blockObject -> setY(backupBlock.getY());        // 블럭 y 좌표 백업
                    return;    // 업데이트 취소
                }
                else if(table[Y][X] == 3) { //빈공간인 경우에 이동한 블럭 정보를 테이블에 갱신
                    copy(backupTable.begin(), backupTable.end(), table.begin());    // table 백업
                    blockObject -> setX(backupBlock.getX());        // 블럭 x 좌표 백업
                    blockObject -> setY(backupBlock.getY());        // 블럭 y 좌표 백업

                    if(key == DOWN){  // 만약 아랫 방향일 경우
                      BuildBlock();   // 블럭을 쌓고  
                      createBlock();  // 새로운 블럭을 만든다.
                    }
                    return;    // 업데이트 취소
                }

                else if(table[Y][X] == 4) { //빈공간인 경우에 이동한 블럭 정보를 테이블에 갱신
                    copy(backupTable.begin(), backupTable.end(), table.begin());    // table 백업
                    blockObject -> setX(backupBlock.getX());        // 블럭 x 좌표 백업
                    blockObject -> setY(backupBlock.getY());        // 블럭 y 좌표 백업
                    
                    if(key == DOWN){
                      BuildBlock();
                      createBlock();
                    }
                    return;    // 업데이트 취소
                }
            }
        }
    }
}

/* 블럭 회전 */
void RotateBlock(){
    /* 백업 */
    Block backupBlock;                                  // 백업용 Block 객체
    vector<vector<int>> backupTable;                    // 백업용 table vector
    backup::updateBlock(blockObject, backupBlock);      // block 백업
    backup::updateTable(table, backupTable);            // table 백업

    /*테이블에서 블럭 객체 지우기*/
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int Y = j + blockObject->getY();
            int X = i + blockObject->getX();
            
            if (table[Y][X] == 2) {     // 만약 블럭이면
                table[Y][X] = 0;        // 테이블에서 지운다
            }
        }
    }

    /* 블럭 회전 */
    blockObject -> rotate();        //블럭을 회전

    /*회전한 블럭 상태를 테이블에 갱신 및 취소 */
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int Y = j + blockObject->getY();
            int X = i + blockObject->getX();
            int blockValue = blockObject -> getShape(blockObject -> getRotationCount(), i, j);  // 블럭 배열 값 얻기

            if(blockValue != 2){
                continue;               // 블럭이 아니면 무시 (블럭은 2로 이루어져 있다.)
            }
            if(table[Y][X] == 0){
                table[Y][X] = blockObject -> getShape(blockObject -> getRotationCount(), i, j);
            }

            else if(table[Y][X] == 1){
                copy(backupTable.begin(), backupTable.end(), table.begin());        // table 백업
                blockObject -> setRotationCount(backupBlock.getRotationCount());    // 회전하기 전 상태로 백업
                return;     // 업데이트 취소
            }
        }
    }
  
    /* 블럭을 table에 쌓기 */
    void BuildBlock(){
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
          int Y = j + blockObject -> getY();
          int X = i + blockObject -> getX();

          if(Y < 0 || X < 0 || Y >= TABLE_Y || X >= TABLE_X){
            continue;           // array out of range 방지
          }

          int blockValue = blockObject -> getShape(blockObject -> getRotationCount(), i, j); // 블럭 배열 값 얻기

          if(blockValue != 2){
            continue;
          }
          table[Y][X] = 3;
        }
      }
    }
    /* 스페이스바 누를 시 바로 떨어진다 */
    void HardDropBlock() {
      /* 테이블에서 블럭 객체 지우기 */
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
          int Y = j + blockObject -> getY();
          int X = i + blockObject -> getX();

          if(Y < 0 || X < 0 || Y >= TABLE_Y || X >= TABLE_X){
            continue;           // array out of range 방지
          }

          if(table[Y][X] == 2){  // 만약 블럭이면
            table[Y][X] = 0;     // 테이블에서 지운다
          }
        }
      }
      while(true){
        for(int i = 0; i < 4; i++){
          for(int j = 0; j < 4; j++){
            int Y = j + blockObject -> getY();
            int X = i + blockObject -> getX();
            
            if(Y < 0 || X < 0 || Y >= TABLE_Y || X >= TABLE_X){
                continue;           // array out of range 방지
            }
            
            int blockValue = blockObject -> getShape(blockObject -> getRotationCount(), i, j); // 블럭 배열 값 얻기
            
            if(blockValue != 2){
              continue;    // 블럭이 아니면 무시 (블럭은 2로 이루어져 있다.)
            }

            if(table[Y][X] == 3 || table[Y][X] == 4){   // 블럭이나 벽을 만나면
              blockObject -> up();                      // 한 칸 위로 올리고
              BuildBlock();                             // 블럭을 쌓고
              createBlock();                            // 새로운 블럭을 만듦
              return;                                   // 함수 종료
            }
          }
        }
        blockObject -> down();  // 블럭을 한 칸 아래로 이동
    }
}

/* 일직선 삭제 */
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

/*게임 시작 클래스*/
class GamePlay {
private:
    GameTable* gt;
public:
    GamePlay() {
        gt = new GameTable(TABLE_X, TABLE_Y); //게임 판 그리기 객체 생성
        gt -> createBlock();                  // 초기 블럭 생성
        gt -> DrawGameTable();                // 게임판을 그린다.
        while (true) {                        // 방향키 입력 이벤트
            int nSelect;
            if (_kbhit()) {
                nSelect = _getch();
                if (nSelect == 224) {
                    nSelect = _getch();
                    switch (nSelect) {
                    case UP: // 화살표 위 눌렀을 때
                        gt -> RotateBlock(); // 블럭을 90도 회전 
                        gotoxy(0, 0); //system("cls") 안쓰고 (0, 0)으로 커서 이동 후
                        gt -> DrawGameTable(); // 덮어쓰기 
                        break;
                    
                    case DOWN: // 화살표 아래 눌렀을 때
                        gt -> MoveBlock(DOWN); // 블럭을 한 칸 아래로 이동
                        gotoxy(0, 0);
                        gt -> DrawGameTable();
                        break;
                    
                    case LEFT: // 화살표 왼쪽 눌렀을 때
                        gt -> MoveBlock(LEFT); // 블럭을 한 칸 왼쪽으로 이동
                        gotoxy(0, 0);
                        gt -> DrawGameTable();
                        break;
                    
                    case RIGHT: // 화살표 오른쪽 눌렀을 때
                        gt -> MoveBlock(RIGHT); // 블럭을 한 칸 오른쪽으로 이동
                        gotoxy(0, 0);
                        gt -> DrawGameTable();
                        break;
                    
                    default:
                        break;
                    }
                }
            }
            gotoxy(0, 0);            // system("cls") 안쓰고 (0, 0) 커서 이동 후
            gt -> DrawGameTable();   // 다시 그리기
        }
    }
    ~GamePlay() { // 게임 종료 이벤트
        system("cls");
        gotoxy(10, 10);
        cout << "GAME OVER";
        delete gt;
    }
};
int main(void) {
    CursorView(false); // 콘솔 화면 커서 제거 
    system("mode con cols=100 lines=40 | title 테트리스 게임"); // 콘솔창 크기 및 제목 설정
    GameTable gt(TABLE_X, TABLE_Y); 
    MainMenu(); // 메인 메뉴 그리기 생성자 호출
    GamePlay(); // 게임 플레이
    getchar();
      
    return 0;
}