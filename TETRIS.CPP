class GameTable{
private:
    int x; // 가로
    int y; // 세로
    std::vector<std::vector<int>> table;  // 테이블 판 0 = " ", 1 = "▦" , 2 = "▦"
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
    }
    // 게임판 그리는 함수
    void DrawGameTable(){
      for(int i = 0; i < y; i++){
        for(int j = 0; j < x; j++){
          if(table[i][j] == 1){
            std::cout << "▦";
          }
          else{
            std::cout << "  ";
          }
        }
        std::cout << "\n";
      }
    }
};

int main(void){
  system("mode con cols = 100 lines = 40 | TITLE 테트리스 게임"); // 콘솔창 크기 및 제목 설정
  GameTable gt(Table_x, Table_y); // 게임 판 그리기 객체 생성
  MainMenu();  // 메인 메뉴 그리기 생성자 호출
  gt.DrawGameTable();  // 게임판 그리기
  getchar();

  return 0;
}


/*
#define -> 매크로를 정의할 때 앞에 써주는 키워드
매크로는 프로그램 내에서 컴파일러가 매크로를 만났을 때 대체할 문자열을 정의해주는 것을 말한다.
이러한 매크로는 컴파일러가 컴파일을 시작하기 전에, 전처리가 #define으로 정의된 기호상수 등을 확장 문자열로 변환자시키는 동작을 한다.

#define[매크로 상수명][매크로 확장 문자열]
#define[매크로 함수명(전달인자)][매크로 확장 문자열]

크게 두 가지로 매크로 상수와 매크로 함수가 있다.
매크로 상수 : 미리 정의한 매크로 상수명이 쓰이면, 매크로 확장 문자열로 치환한다.
매크로 함수 : 매크로 함수는 전달인자의 형태를 고려해서 치환하는 동작을 한다.

매크로는 프로그램의 가독성을 높여주고, 유지보수도 용이하게 해준다. 또한, 변수를 사용하는 것보다 처리속도가 빠르다.

getchar(); -> standard input(표준입력)으로부터 받은 문자를 반환하는 함수이다.

system("cls"); -> 콘솔 창 지우기
system("pause"); -> 자동으로 창이 종료되는 것을 막기 위해 대기 상태를 만들어 종료를 잠시 멈추게 함. 
*/